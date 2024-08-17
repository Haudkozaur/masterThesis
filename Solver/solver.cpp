#include "solver.h"
#include <iostream>
#include <algorithm>
#include <set>
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
                    // std::cout << "Assigning K(" << reducedRow << ", " << reducedCol << ") = K(" << i << ", " << j << ")" << std::endl;
                    ++reducedCol;
                }
            }
            reducedF(reducedRow) = loadVector(i);
            // std::cout << "Assigning F(" << reducedRow << ") = F(" << i << ")" << std::endl;
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
        const Member& member = memberPair.second;

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

        // Calculate stiffness matrix terms for internal forces:
        double w1 = E * A / L;
        double w2 = 12 * E * I / (L * L * L);
        double w3 = 6 * E * I / (L * L);
        double w4 = 4 * E * I / L;
        double w5 = 2 * E * I / L;

        Eigen::MatrixXd kLocal(6, 6);
        kLocal << w1, 0, 0, -w1, 0, 0,
            0, w2, w3, 0, -w2, w3,
            0, w3, w4, 0, -w3, w5,
            -w1, 0, 0, w1, 0, 0,
            0, -w2, -w3, 0, w2, -w3,
            0, w3, w5, 0, -w3, w4;

        Eigen::MatrixXd T(6, 6);
        T << c, s, 0, 0, 0, 0,
            -s, c, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0,
            0, 0, 0, c, s, 0,
            0, 0, 0, -s, c, 0,
            0, 0, 0, 0, 0, 1;

        Eigen::MatrixXd kGlobal = T.transpose() * kLocal * T;
        Eigen::VectorXd fInternal = kGlobal * uLocal;

        // Store forces for the start node
        double Nx_start = fInternal(0);
        double Vz_start = fInternal(1);  // This should consider the sign convention
        double My_start = fInternal(2);

        // Correct the sign for Vz_start if needed
        if (startNodeId < endNodeId) {
            Vz_start = -Vz_start;  // Assuming positive direction is downward for the start node
        }

        double xCord_start = nodes.at(startNodeId).getX();
        double zCord_start = nodes.at(startNodeId).getZ();
        double deformation_start = std::sqrt(displacementVector(startDofIndex) * displacementVector(startDofIndex) +
                                             displacementVector(startDofIndex + 1) * displacementVector(startDofIndex + 1));

        // Debug output for the start node
        cout << "Member ID:" << memberId << "Start Node:" << startNodeId
                 << "Nx_start:" << Nx_start << "Vz_start:" << Vz_start << "My_start:" << My_start;

        dbResultsManager->addObjectToDataBase(startNodeId, xCord_start, zCord_start, Nx_start, Vz_start, My_start, deformation_start);

        // Store forces for the end node
        double Nx_end = fInternal(3);
        double Vz_end = fInternal(4);  // This should consider the sign convention
        double My_end = fInternal(5);

        double xCord_end = nodes.at(endNodeId).getX();
        double zCord_end = nodes.at(endNodeId).getZ();
        double deformation_end = std::sqrt(displacementVector(endDofIndex) * displacementVector(endDofIndex) +
                                           displacementVector(endDofIndex + 1) * displacementVector(endDofIndex + 1));

        // Debug output for the end node
        cout << "Member ID:" << memberId << "End Node:" << endNodeId
                 << "Nx_end:" << Nx_end << "Vz_end:" << Vz_end << "My_end:" << My_end;

        dbResultsManager->addObjectToDataBase(endNodeId, xCord_end, zCord_end, Nx_end, Vz_end, My_end, deformation_end);
    }
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



// void Solver::saveResultsToDataBase(DataBaseManagers::DataBaseResultsManager *dbResultsManager)
// {
//     std::cout << "Saving results to the database..." << std::endl;

//     // Iterate over each node
//     for (const auto& nodePair : nodes) {
//         int nodeId = nodePair.first;
//         double xCord = nodePair.second.getX();
//         double zCord = nodePair.second.getZ();

//         // Retrieve node displacement (Tx, Tz, Ry)
//         int dofIndex = nodeIdToDofMap[nodeId];
//         double Tx = displacementVector(dofIndex);
//         double Tz = displacementVector(dofIndex + 1);
//         double Ry = displacementVector(dofIndex + 2);

//         // Initialize internal force variables
//         double Nx = 0.0, Vz = 0.0, My = 0.0;

//         std::cout << "Node ID: " << nodeId << ", Initial forces -> Nx: " << Nx << ", Vz: " << Vz << ", My: " << My << std::endl;

//         // Iterate over each member and accumulate forces
//         for (const auto& memberPair : members) {
//             const Member& member = memberPair.second;
//             int startNodeId = member.getFirstNodeNumber();
//             int endNodeId = member.getSecondNodeNumber();

//             if (startNodeId == nodeId || endNodeId == nodeId) {
//                 int startDofIndex = nodeIdToDofMap[startNodeId];
//                 int endDofIndex = nodeIdToDofMap[endNodeId];

//                 Eigen::VectorXd uLocal(6);
//                 uLocal.segment<3>(0) = displacementVector.segment<3>(startDofIndex);
//                 uLocal.segment<3>(3) = displacementVector.segment<3>(endDofIndex);

//                 double E = member.getE();
//                 double A = member.getA();
//                 double I = member.getI();
//                 double L = member.getLength();
//                 double lambdaX = member.getLambdaX();
//                 double lambdaZ = member.getLambdaZ();

//                 double c = lambdaX;
//                 double s = lambdaZ;

//                 // Transformation matrix for member local to global coordinates
//                 Eigen::MatrixXd T(6, 6);
//                 T << c, s, 0, 0, 0, 0,
//                     -s, c, 0, 0, 0, 0,
//                     0, 0, 1, 0, 0, 0,
//                     0, 0, 0, c, s, 0,
//                     0, 0, 0, -s, c, 0,
//                     0, 0, 0, 0, 0, 1;

//                 // Calculate internal forces in global coordinates
//                 Eigen::VectorXd fInternal = T.transpose() * memberStiffnessMatrices.at(memberPair.first) * uLocal;

//                 if (startNodeId == nodeId) {
//                     std::cout << "Start Node Force Contribution - Member ID: " << memberPair.first
//                               << ", fInternal(0): " << fInternal(0)
//                               << ", fInternal(1): " << fInternal(1)
//                               << ", fInternal(2): " << fInternal(2) << std::endl;

//                     Nx += fInternal(0);  // Axial force at the start node
//                     Vz += fInternal(1);  // Shear force at the start node
//                     My += fInternal(2);  // Bending moment at the start node
//                 } else if (endNodeId == nodeId) {
//                     std::cout << "End Node Force Contribution - Member ID: " << memberPair.first
//                               << ", fInternal(3): " << fInternal(3)
//                               << ", fInternal(4): " << fInternal(4)
//                               << ", fInternal(5): " << fInternal(5) << std::endl;

//                     Nx += fInternal(3);  // Axial force at the end node
//                     Vz += fInternal(4);  // Shear force at the end node
//                     My += fInternal(5);  // Bending moment at the end node
//                 }

//                 std::cout << "Updated Node ID: " << nodeId << ", Accumulated forces -> Nx: " << Nx << ", Vz: " << Vz << ", My: " << My << std::endl;
//             }
//         }

//         // Deformation can be computed as the resultant displacement (sqrt(Tx^2 + Tz^2))
//         double deformation = sqrt(Tx * Tx + Tz * Tz);

//         // Save the results to the database
//         dbResultsManager->addObjectToDataBase(nodeId, xCord, zCord, Nx, Vz, My, deformation);
//     }

//     std::cout << "Results saved to the database successfully." << std::endl;
// }


} // namespace SolverFEM


