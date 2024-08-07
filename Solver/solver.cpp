#include "Solver.h"
#include <iostream>
#include <algorithm>

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

    // Initialize the global stiffness matrix and node to DOF map
    initializeGlobalStiffnessMatrix();
    createNodeIdToDofMap();
}

void Solver::createNodeIdToDofMap()
{
    int index = 0;
    for (const auto& nodePair : nodes) {
        int nodeId = nodePair.first;
        nodeIdToDofMap[nodeId] = index;
        index += 3; // Each node has 3 degrees of freedom (DOFs)
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

        // Calculate stiffness matrix components
        double w1 = A * c * c + 12 * I * s * s / (L * L);
        double w2 = A * s * s + 12 * I * c * c / (L * L);
        double w3 = (A - 12 * I / (L * L)) * c * s;
        double w4 = 6 * I * s / L;
        double w5 = 6 * I * c / L;
        double w6 = 4 * I;
        double w7 = 2 * I;

        Eigen::MatrixXd k(6, 6);
        k << E / L * w1, E / L * w3, -E / L * w4, -E / L * w1, -E / L * w3, -E / L * w4,
            E / L * w3, E / L * w2, E / L * w5, -E / L * w3, -E / L * w2, E / L * w5,
            -E / L * w4, E / L * w5, E / L * w6, E / L * w4, -E / L * w5, E / L * w7,
            -E / L * w1, -E / L * w3, E / L * w4, E / L * w1, E / L * w3, E / L * w4,
            -E / L * w3, -E / L * w2, -E / L * w5, E / L * w3, E / L * w2, -E / L * w5,
            -E / L * w4, E / L * w5, E / L * w7, E / L * w4, -E / L * w5, E / L * w6;

        // Store the stiffness matrix in the map
        memberStiffnessMatrices[memberPair.first] = k;
    }
}

void Solver::initializeGlobalStiffnessMatrix()
{
    int numNodes = nodes.size();
    int size = numNodes * 3; // Each node has 3 degrees of freedom (DOFs)
    K = Eigen::MatrixXd::Zero(size, size); // Initialize a zero matrix of the appropriate size
}

void Solver::aggregateStiffnessMatrix(Eigen::MatrixXd& globalMatrix, const Eigen::MatrixXd& localMatrix, int startNodeId, int endNodeId)
{
    // Determine the global DOF indices for the start and end nodes
    int startIndex = nodeIdToDofMap[startNodeId];
    int endIndex = nodeIdToDofMap[endNodeId];

    // Debugging: Print indices
    std::cout << "Aggregating local stiffness matrix for nodes: " << startNodeId << " to " << endNodeId << std::endl;
    std::cout << "Start index: " << startIndex << ", End index: " << endIndex << std::endl;

    int numDOFs = globalMatrix.rows();
    if (startIndex + 2 >= numDOFs || endIndex + 2 >= numDOFs) {
        std::cerr << "Index out of bounds for nodes: " << startNodeId << " to " << endNodeId << std::endl;
        return;
    }

    // Aggregate the local stiffness matrix into the global stiffness matrix
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
}

void Solver::applyBoundaryConditions()
{
    std::vector<int> constrainedDOFs;
    // Identify the constrained DOFs based on the nodeSupports
    for (const auto& supportPair : nodeSupports) {
        const NodeSupport& support = supportPair.second;
        int nodeId = supportPair.first;
        if (!support.getTx()) constrainedDOFs.push_back(nodeIdToDofMap[nodeId]);
        if (!support.getTz()) constrainedDOFs.push_back(nodeIdToDofMap[nodeId] + 1);
        if (!support.getRy()) constrainedDOFs.push_back(nodeIdToDofMap[nodeId] + 2);
    }

    int size = K.rows();
    Eigen::MatrixXd reducedK(size - constrainedDOFs.size(), size - constrainedDOFs.size());
    Eigen::VectorXd reducedF(size - constrainedDOFs.size());
    int reducedRow = 0, reducedCol = 0;

    // Debugging: Print constrained DOFs
    std::cout << "Constrained DOFs: ";
    for (int dof : constrainedDOFs) {
        std::cout << dof << " ";
    }
    std::cout << std::endl;

    // Create reduced stiffness matrix and force vector
    for (int i = 0; i < size; ++i) {
        if (std::find(constrainedDOFs.begin(), constrainedDOFs.end(), i) == constrainedDOFs.end()) {
            reducedCol = 0;
            for (int j = 0; j < size; ++j) {
                if (std::find(constrainedDOFs.begin(), constrainedDOFs.end(), j) == constrainedDOFs.end()) {
                    reducedK(reducedRow, reducedCol) = K(i, j);
                    ++reducedCol;
                }
            }
            reducedF(reducedRow) = loadVector(i);
            ++reducedRow;
        }
    }

    // Replace K and loadVector with reduced versions
    K = reducedK;
    loadVector = reducedF;
}

void Solver::createForceVector()
{
    int numNodes = nodes.size();
    int size = numNodes * 3; // Each node has 3 degrees of freedom (DOFs)
    loadVector = Eigen::VectorXd::Zero(size); // Initialize a zero vector of the appropriate size

    // Add nodal loads to the load vector
    for (const auto& loadPair : nodeLoads) {
        const NodeLoad& load = loadPair.second;
        int nodeId = load.getNodeNumber();
        int dofIndex = nodeIdToDofMap[nodeId];
        loadVector[dofIndex] += load.getXLoad();
        loadVector[dofIndex + 1] += load.getZLoad();
        loadVector[dofIndex + 2] += load.getMLoad();
    }

    // Add uniform loads to the load vector
    for (const auto& uniformLoadPair : uniformLoads) {
        const UniformLoad& uLoad = uniformLoadPair.second;
        int memberId = uLoad.getMemberId();
        const Member& member = members.at(memberId);

        double qx = uLoad.getFx();
        double qz = uLoad.getFz();
        double L = member.getLength();

        // Equivalent nodal forces for a uniform load
        Eigen::VectorXd fLocal(6);
        fLocal << qx * L / 2, qz * L / 2, qz * L * L / 12, qx * L / 2, qz * L / 2, -qz * L * L / 12;

        int startNodeId = member.getFirstNodeNumber();
        int endNodeId = member.getSecondNodeNumber();
        int startDofIndex = nodeIdToDofMap[startNodeId];
        int endDofIndex = nodeIdToDofMap[endNodeId];

        // Add the local forces to the global load vector
        loadVector.segment<3>(startDofIndex) += fLocal.segment<3>(0);
        loadVector.segment<3>(endDofIndex) += fLocal.segment<3>(3);
    }
}

void Solver::solveSystemOfEquations()
{
    // Solve the system of equations K * U = F
    Eigen::VectorXd U = K.fullPivLu().solve(loadVector);

    // Initialize the global displacement vector
    int numNodes = nodes.size();
    int size = numNodes * 3; // Each node has 3 degrees of freedom (DOFs)
    displacementVector = Eigen::VectorXd::Zero(size);

    // Map the reduced displacements back to the global displacement vector
    std::vector<int> constrainedDOFs;
    for (const auto& supportPair : nodeSupports) {
        const NodeSupport& support = supportPair.second;
        int nodeId = supportPair.first;
        if (!support.getTx()) constrainedDOFs.push_back(nodeIdToDofMap[nodeId]);
        if (!support.getTz()) constrainedDOFs.push_back(nodeIdToDofMap[nodeId] + 1);
        if (!support.getRy()) constrainedDOFs.push_back(nodeIdToDofMap[nodeId] + 2);
    }

    int reducedIndex = 0;
    for (int i = 0; i < size; ++i) {
        if (std::find(constrainedDOFs.begin(), constrainedDOFs.end(), i) == constrainedDOFs.end()) {
            displacementVector(i) = U(reducedIndex);
            ++reducedIndex;
        }
    }
}

void Solver::calculateInternalForces()
{
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

        // Store or print the internal forces for debugging
        std::cout << "Member ID: " << memberId << ", Internal Forces: " << fInternal.transpose() << std::endl;
    }
}

void Solver::solve()
{
    calculateStiffnessMatrices();
    initializeGlobalStiffnessMatrix();
    assembleGlobalStiffnessMatrix();
    createForceVector();
    applyBoundaryConditions();
    solveSystemOfEquations();
    calculateInternalForces();
}

} // namespace SolverFEM
