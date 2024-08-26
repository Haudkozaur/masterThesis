#include "solver.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <Eigen/Dense>
#include "../DataBaseManagers/DataBaseResultsManager.h"

namespace SolverFEM {

Solver::Solver(DataBaseManagers::DataBaseSolverPreparer* dbSolverPreparer)
{
    // Fetching data from DataBaseSolverPreparer and storing in Solver class
    nodes = dbSolverPreparer->getNodes();
    nodeLoads = dbSolverPreparer->getNodeLoads();
    nodeSupports = dbSolverPreparer->getNodeSupports();
    members = dbSolverPreparer->getMembers();
    uniformLoads = dbSolverPreparer->getUniformLoads();
    memberSupportConditions = dbSolverPreparer->getMemberSupportConditions();
    lines = dbSolverPreparer->getLines();

    // Debug: Print fetched data
    std::cout << "Fetched Nodes:\n";
    for (const auto& node : nodes) {
        std::cout << "Node ID: " << node.first << ", X: " << node.second.getX() << ", Z: " << node.second.getZ() << std::endl;
    }

    std::cout << "Fetched Members:\n";
    for (const auto& member : members) {
        std::cout << "Member ID: " << member.first << ", Start Node: " << member.second.getFirstNodeNumber()
                  << ", End Node: " << member.second.getSecondNodeNumber() << std::endl;
    }

    // Initialize the node ID to DOF map and global stiffness matrix
    createNodeIdToDofMap();
    initializeGlobalStiffnessMatrix();
}

void Solver::createNodeIdToDofMap() {
    int index = 0;
    for (const auto& nodePair : nodes) {
        nodeIdToDofMap[nodePair.first] = index;
        index += 3; // Each node has 3 DOFs (Tx, Tz, Ry)
    }

    // Debug: Print Node ID to DOF mapping
    std::cout << "Node ID to DOF mapping:\n";
    for (const auto& mapPair : nodeIdToDofMap) {
        std::cout << "Node ID: " << mapPair.first << " -> DOF Index: " << mapPair.second << std::endl;
    }
}

void Solver::calculateStiffnessMatrices()
{
    for (const auto& memberPair : members) {
        const Member& member = memberPair.second;

        double E = member.getE();
        double A = member.getA();
        double I = member.getI();
        double L = member.getLength();
        double lambdaX = member.getLambdaX();
        double lambdaZ = member.getLambdaZ();

        double c = lambdaX;
        double s = lambdaZ;

        double w1 = A * c * c + 12 * I * s * s / (L * L);
        double w2 = A * s * s + 12 * I * c * c / (L * L);
        double w3 = (A - 12 * I / (L * L)) * c * s;
        double w4 = 6 * I * s / L;
        double w5 = 6 * I * c / L;

        Eigen::MatrixXd k(6, 6);
        k << E / L * w1, E / L * w3, -E / L * w4, -E / L * w1, -E / L * w3, -E / L * w4,
            E / L * w3, E / L * w2, E / L * w5, -E / L * w3, -E / L * w2, E / L * w5,
            -E / L * w4, E / L * w5, 4 * I * E / L, E / L * w4, -E / L * w5, 2 * I * E / L,
            -E / L * w1, -E / L * w3, E / L * w4, E / L * w1, E / L * w3, E / L * w4,
            -E / L * w3, -E / L * w2, -E / L * w5, E / L * w3, E / L * w2, -E / L * w5,
            -E / L * w4, E / L * w5, 2 * I * E / L, E / L * w4, -E / L * w5, 4 * I * E / L;

        memberStiffnessMatrices[memberPair.first] = k;

        // Debugging: Print stiffness matrix
        std::cout << "Member ID: " << memberPair.first << ", Stiffness Matrix:\n" << k << std::endl;
    }
}

void Solver::initializeGlobalStiffnessMatrix()
{
    int numNodes = nodes.size();
    int size = numNodes * 3; // Each node has 3 degrees of freedom (DOFs)
    K = Eigen::MatrixXd::Zero(size, size); // Initialize a zero matrix of the appropriate size

    // Debug: Print initial global stiffness matrix
    std::cout << "Initial Global Stiffness Matrix:\n" << K << std::endl;
}

void Solver::aggregateStiffnessMatrix(Eigen::MatrixXd& globalMatrix, const Eigen::MatrixXd& localMatrix, int startNodeId, int endNodeId)
{
    int startIndex = nodeIdToDofMap[startNodeId];
    int endIndex = nodeIdToDofMap[endNodeId];

    int numDOFs = globalMatrix.rows();

    for (int i = 0; i < 6; ++i) {
        int globalRow = (i < 3 ? startIndex : endIndex) + (i % 3);

        for (int j = 0; j < 6; ++j) {
            int globalCol = (j < 3 ? startIndex : endIndex) + (j % 3);

            if (globalRow >= numDOFs || globalCol >= numDOFs) {
                std::cerr << "Index out of bounds: globalRow=" << globalRow << ", globalCol=" << globalCol << std::endl;
                continue;
            }

            globalMatrix(globalRow, globalCol) += localMatrix(i, j);
        }
    }
}

void Solver::assembleGlobalStiffnessMatrix()
{
    for (const auto& memberPair : members) {
        int memberId = memberPair.first;
        const Member& member = memberPair.second;

        const Eigen::MatrixXd& localMatrix = memberStiffnessMatrices[memberId];

        int startNodeId = member.getFirstNodeNumber();
        int endNodeId = member.getSecondNodeNumber();

        aggregateStiffnessMatrix(K, localMatrix, startNodeId, endNodeId);
    }

    // Debug: Print global stiffness matrix after assembly
    std::cout << "Global Stiffness Matrix after assembly:\n" << K << std::endl;
}

void Solver::createForceVector() {
    int size = nodes.size() * 3;
    loadVector = Eigen::VectorXd::Zero(size);

    // Add uniform loads to the force vector
    for (const auto& uniformLoadPair : uniformLoads) {
        const UniformLoad& uLoad = uniformLoadPair.second;
        int memberId = uLoad.getMemberId();

        if (members.find(memberId) == members.end()) {
            std::cerr << "Error: memberId " << memberId << " not found in members!" << std::endl;
            continue;
        }

        const Member& member = members.at(memberId);
        double qx = uLoad.getFx(); // Should be zero for vertical load
        double qz = uLoad.getFz();
        double L = member.getLength();

        // For a uniformly distributed load:
        Eigen::VectorXd fLocal(6);
        fLocal << qx * L / 2, qz * L / 2, qz * L * L / 12,
            qx * L / 2, qz * L / 2, -qz * L * L / 12;

        int startDofIndex = nodeIdToDofMap[member.getFirstNodeNumber()];
        int endDofIndex = nodeIdToDofMap[member.getSecondNodeNumber()];

        loadVector.segment<3>(startDofIndex) += fLocal.segment<3>(0);
        loadVector.segment<3>(endDofIndex) += fLocal.segment<3>(3);
    }

    // Add nodal loads to the force vector
    for (const auto& nodeLoadPair : nodeLoads) {
        const NodeLoad& nLoad = nodeLoadPair.second;
        int nodeId = nLoad.getNodeNumber();

        if (nodeIdToDofMap.find(nodeId) == nodeIdToDofMap.end()) {
            std::cerr << "Error: nodeId " << nodeId << " not found in nodeIdToDofMap!" << std::endl;
            continue;
        }

        int dofIndex = nodeIdToDofMap[nodeId];

        loadVector(dofIndex) += nLoad.getXLoad();    // Tx
        loadVector(dofIndex + 1) += nLoad.getZLoad(); // Tz
        loadVector(dofIndex + 2) += nLoad.getMLoad(); // Ry
    }

    // Debug: Print force vector
    std::cout << "Force Vector:\n" << loadVector << std::endl;
}

void Solver::applyBoundaryConditions() {
    std::set<int> constrainedDOFs;

    std::cout << "Identifying constrained DOFs..." << std::endl;

    for (const auto& supportPair : nodeSupports) {
        const NodeSupport& support = supportPair.second;
        int nodeId = supportPair.first;
        int dofIndex = nodeIdToDofMap[nodeId];

        // Adjusted condition to check for '1', indicating a constrained DOF
        if (support.getTx()) {
            constrainedDOFs.insert(dofIndex);
            std::cout << "Constraining Tx DOF at index " << dofIndex << std::endl;
        }
        if (support.getTz()) {
            constrainedDOFs.insert(dofIndex + 1);
            std::cout << "Constraining Tz DOF at index " << dofIndex + 1 << std::endl;
        }
        if (support.getRy()) {
            constrainedDOFs.insert(dofIndex + 2);
            std::cout << "Constraining Ry DOF at index " << dofIndex + 2 << std::endl;
        }
    }

    int size = K.rows();
    int reducedSize = size - constrainedDOFs.size();
    std::cout << "Original matrix size: " << size << std::endl;
    std::cout << "Reduced matrix size: " << reducedSize << std::endl;

    Eigen::MatrixXd reducedK(reducedSize, reducedSize);
    Eigen::VectorXd reducedF(reducedSize);

    int reducedRow = 0, reducedCol = 0;

    std::cout << "Starting matrix reduction..." << std::endl;

    for (int i = 0; i < size; ++i) {
        if (constrainedDOFs.find(i) == constrainedDOFs.end()) {
            reducedCol = 0;
            for (int j = 0; j < size; ++j) {
                if (constrainedDOFs.find(j) == constrainedDOFs.end()) {
                    if (reducedRow >= reducedSize || reducedCol >= reducedSize) {
                        std::cerr << "Error: Index out of bounds when creating reduced stiffness matrix!" << std::endl;
                        return;
                    }
                    reducedK(reducedRow, reducedCol) = K(i, j);
                    ++reducedCol;
                }
            }
            reducedF(reducedRow) = loadVector(i);
            ++reducedRow;
        }
    }

    K = reducedK;
    loadVector = reducedF;

    // Debug: Print reduced stiffness matrix and force vector
    std::cout << "Reduced Global Stiffness Matrix:\n" << K << std::endl;
    std::cout << "Reduced Force Vector:\n" << loadVector << std::endl;

    std::cout << "Boundary conditions applied successfully." << std::endl;
}

void Solver::solveSystemOfEquations() {
    std::cout << "Entering solveSystemOfEquations..." << "\n";

    Eigen::VectorXd U = K.fullPivLu().solve(loadVector);

    int size = nodes.size() * 3;
    displacementVector = Eigen::VectorXd::Zero(size);

    std::vector<int> constrainedDOFs;
    for (const auto& supportPair : nodeSupports) {
        const NodeSupport& support = supportPair.second;
        int nodeId = supportPair.first;
        int dofIndex = nodeIdToDofMap[nodeId];
        if (support.getTx()) constrainedDOFs.push_back(dofIndex);
        if (support.getTz()) constrainedDOFs.push_back(dofIndex + 1);
        if (support.getRy()) constrainedDOFs.push_back(dofIndex + 2);
    }

    int reducedIndex = 0;
    for (int i = 0; i < size; ++i) {
        if (std::find(constrainedDOFs.begin(), constrainedDOFs.end(), i) == constrainedDOFs.end()) {
            if (reducedIndex >= U.size()) {
                std::cerr << "Error: reducedIndex out of bounds while mapping displacements! reducedIndex=" << reducedIndex << ", U.size()=" << U.size() << std::endl;
                return;
            }
            displacementVector(i) = U(reducedIndex);
            std::cout << "Mapping U(" << reducedIndex << ") to displacementVector(" << i << ")" << std::endl;
            ++reducedIndex;
        } else {
            std::cout << "Skipping constrained DOF at index " << i << std::endl;
        }
    }

    // Debug: Print displacement vector
    std::cout << "Displacement Vector:\n" << displacementVector << std::endl;

    std::cout << "System solved and displacements mapped." << std::endl;
}
void Solver::calculateInternalForces(DataBaseManagers::DataBaseResultsManager* dbResultsManager) {
    for (const auto& memberPair : members) {
        int memberId = memberPair.first;
        const SolverFEM::Member& member = memberPair.second;

        int startNodeId = member.getFirstNodeNumber();
        int endNodeId = member.getSecondNodeNumber();

        int startDofIndex = nodeIdToDofMap[startNodeId];
        int endDofIndex = nodeIdToDofMap[endNodeId];

        Eigen::VectorXd uLocal(6);
        uLocal.segment<3>(0) = displacementVector.segment<3>(startDofIndex);
        uLocal.segment<3>(3) = displacementVector.segment<3>(endDofIndex);

        double E = member.getE();
        double A = member.getA();
        double I = member.getI();
        double L = member.getLength();
        double lambdaX = member.getLambdaX();
        double lambdaZ = member.getLambdaZ();

        double c = lambdaX;
        double s = lambdaZ;

        // Calculate stiffness matrix terms for internal forces
        double w1 = E * A / L;
        double w2 = 12 * E * I / (L * L * L);
        double w3 = 6 * E * I / (L * L);
        double w4 = 4 * E * I / L;
        double w5 = 2 * E * I / L;

        Eigen::MatrixXd kLocal(6, 6);
        kLocal << w1, 0, 0, -w1, 0, 0,
            0, w2, w3, 0, -w2, w3,
            0, w3, w4, 0, -w3, w5,
            -w1, 0, 0,  w1, 0, 0,
            0, -w2, -w3, 0,  w2, -w3,
            0,  w3, w5, 0, -w3,  w4;

        Eigen::MatrixXd T(6, 6);
        T << c,  s, 0, 0, 0, 0,
            -s,  c, 0, 0, 0, 0,
            0,  0, 1, 0, 0, 0,
            0,  0, 0, c, s, 0,
            0,  0, 0, -s, c, 0,
            0,  0, 0, 0, 0, 1;

        Eigen::MatrixXd kGlobal = T.transpose() * kLocal * T;
        Eigen::VectorXd fInternal = kGlobal * uLocal;

        // Retrieve the inclination angle from the lines map
        int lineId = member.getLineId();
        double inclinationAngle = std::get<4>(lines[lineId]);
        double inclinationRadians = inclinationAngle * M_PI / 180.0; // Convert to radians

        // Recalculate the forces based on the inclination angle
        double Nx_start = fInternal(0) * std::cos(inclinationRadians) - fInternal(1) * std::sin(inclinationRadians);
        double Vz_start = fInternal(0) * std::sin(inclinationRadians) + fInternal(1) * std::cos(inclinationRadians);
        double My_start = fInternal(2);

        // Check if the inclination angle is within [90, 270) degrees
        if (inclinationAngle >= 90.0 && inclinationAngle < 270.0) {
            My_start = -My_start;  // Reverse the sign of My
        }

        bool isStartStartNode = (startNodeId == member.getFirstNodeNumber());
        double xCord_start = nodes.at(startNodeId).getX();
        double zCord_start = nodes.at(startNodeId).getZ();
        double deformation_start = std::sqrt(displacementVector(startDofIndex) * displacementVector(startDofIndex) +
                                             displacementVector(startDofIndex + 1) * displacementVector(startDofIndex + 1));

        dbResultsManager->addResultToDataBase(member.getLineId(), memberId, startNodeId, Nx_start, Vz_start, My_start, deformation_start, isStartStartNode);
        dbResultsManager->addOrUpdateNodeToDataBase(startNodeId, xCord_start, zCord_start);

        double Nx_end = fInternal(3) * std::cos(inclinationRadians) - fInternal(4) * std::sin(inclinationRadians);
        double Vz_end = fInternal(3) * std::sin(inclinationRadians) + fInternal(4) * std::cos(inclinationRadians);
        double My_end = fInternal(5);

        // Check if the inclination angle is within [90, 270) degrees
        if (inclinationAngle >= 90.0 && inclinationAngle < 270.0) {
            My_end = -My_end;  // Reverse the sign of My
        }

        bool isStartEndNode = (endNodeId == member.getFirstNodeNumber());
        double xCord_end = nodes.at(endNodeId).getX();
        double zCord_end = nodes.at(endNodeId).getZ();
        double deformation_end = std::sqrt(displacementVector(endDofIndex) * displacementVector(endDofIndex) +
                                           displacementVector(endDofIndex + 1) * displacementVector(endDofIndex + 1));

        dbResultsManager->addResultToDataBase(member.getLineId(), memberId, endNodeId,
                                              Nx_end, Vz_end, My_end, deformation_end, isStartEndNode);
        dbResultsManager->addOrUpdateNodeToDataBase(endNodeId, xCord_end, zCord_end);
    }

    // Iteracja przez tabelę i uzyskanie resultsMap
    dbResultsManager->iterateOverTable();
    std::map<int, std::vector<std::tuple<int, double, double, double, double, double, double, int, bool>>> resultsMap = dbResultsManager->getResultsMap();

    // Mapa przechowująca `deltaVz` dla każdej linii
    std::map<int, double> deltaVzMap;

    // Grupowanie wyników według `lineId`
    std::map<int, std::vector<std::tuple<int, double, double, double, double, double, double, int, bool>>> lineGroups;
    for (const auto& [nodeId, results] : resultsMap) {
        for (const auto& result : results) {
            int lineId = std::get<7>(result);
            lineGroups[lineId].push_back(result);
        }
    }

    // Obliczenie `deltaVz` dla każdej linii na podstawie środkowych wyników oddzielonych o jeden
    for (const auto& [lineId, group] : lineGroups) {
        if (group.size() >= 4) {  // Upewnij się, że mamy co najmniej cztery węzły, aby wybrać dwa z środka oddzielone o jeden
            auto middleIt1 = std::next(group.begin(), group.size() / 2 - 2);
            auto middleIt2 = std::next(middleIt1, 2);  // Oddzielone o jeden

            double Vz1 = std::get<4>(*middleIt1);
            double Vz2 = std::get<4>(*middleIt2);

            // Oblicz `deltaVz` na podstawie różnicy Vz
            double deltaVz = std::abs(Vz2 - Vz1) / 2.0;
            std::cout << "Line ID: " << lineId << ", deltaVz: " << deltaVz << std::endl;

            // Przechowaj `deltaVz` dla tej linii
            deltaVzMap[lineId] = deltaVz;
        } else if (group.size() == 3) {  // Jeśli tylko trzy węzły, wybierz środkowy i jeden z sąsiadów
            auto middleIt1 = std::next(group.begin(), 1);  // Środkowy
            auto middleIt2 = std::next(middleIt1, 1);  // Sąsiad

            double Vz1 = std::get<4>(*middleIt1);
            double Vz2 = std::get<4>(*middleIt2);

            double deltaVz = std::abs(Vz2 - Vz1) / 2.0;
            std::cout << "Line ID: " << lineId << ", deltaVz: " << deltaVz << std::endl;

            deltaVzMap[lineId] = deltaVz;
        } else if (group.size() == 2) {  // Jeśli tylko dwa węzły, wybierz je oba
            auto& result1 = group.front();
            auto& result2 = group.back();

            double Vz1 = std::get<4>(result1);
            double Vz2 = std::get<4>(result2);

            double deltaVz = std::abs(Vz2 - Vz1) / 2.0;
            std::cout << "Line ID: " << lineId << ", deltaVz: " << deltaVz << std::endl;

            deltaVzMap[lineId] = deltaVz;
        }
    }

    // Korekta wartości sił tnących w wynikach
    for (auto& [nodeId, results] : resultsMap) {
        for (auto& result : results) {
            int lineId = std::get<7>(result);
            double& Vz = std::get<4>(result);

            // Wypisanie wartości Vz przed korektą
            std::cout << "Node ID: " << nodeId << ", Line ID: " << lineId << ", Vz before correction: " << Vz << std::endl;

            // Jeśli obliczono `deltaVz` dla tej linii, zastosuj korektę
            if (deltaVzMap.find(lineId) != deltaVzMap.end()) {
                double deltaVz = deltaVzMap[lineId];
                Vz += deltaVz;
            }

            // Wypisanie wartości Vz po korekcie
            std::cout << "Node ID: " << nodeId << ", Line ID: " << lineId << ", Vz after correction: " << Vz << std::endl;
        }
    }

    // Aktualizacja wyników w bazie danych
    std::cout << "Aktualizacja wyników w bazie danych..." << std::endl;
    dbResultsManager->updateResultsInDataBase(resultsMap);
    std::cout << "Aktualizacja zakończona." << std::endl;
}


void Solver::solve()
{
    calculateStiffnessMatrices();
    assembleGlobalStiffnessMatrix();
    std::cout << "Global stiffness matrix assembled" << std::endl;
    createForceVector();
    std::cout << "Force vector created" << std::endl;
    applyBoundaryConditions();
    std::cout << "Boundaries applied" << std::endl;
    solveSystemOfEquations();
}

} // namespace SolverFEM
