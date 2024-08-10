#include "Solver.h"
#include <iostream>
#include <algorithm>
#include <set>

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

        // Calculate stiffness matrix components, consistent with the MATLAB version
        double w1 = A * c * c + 12 * I * s * s / (L * L);
        double w2 = A * s * s + 12 * I * c * c / (L * L);
        double w3 = (A - 12 * I / (L * L)) * c * s;
        double w4 = 6 * I * s / L;
        double w5 = 6 * I * c / L;

        // Create stiffness matrix similar to the MATLAB version
        Eigen::MatrixXd k(6, 6);
        k << E / L * w1, E / L * w3, -E / L * w4, -E / L * w1, -E / L * w3, -E / L * w4,
            E / L * w3, E / L * w2, E / L * w5, -E / L * w3, -E / L * w2, E / L * w5,
            -E / L * w4, E / L * w5, 4 * I * E / L, E / L * w4, -E / L * w5, 2 * I * E / L,
            -E / L * w1, -E / L * w3, E / L * w4, E / L * w1, E / L * w3, E / L * w4,
            -E / L * w3, -E / L * w2, -E / L * w5, E / L * w3, E / L * w2, -E / L * w5,
            -E / L * w4, E / L * w5, 2 * I * E / L, E / L * w4, -E / L * w5, 4 * I * E / L;

        // Store the stiffness matrix in the map
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
}

void Solver::aggregateStiffnessMatrix(Eigen::MatrixXd& globalMatrix, const Eigen::MatrixXd& localMatrix, int startNodeId, int endNodeId)
{
    // Find the global degrees of freedom (DOF) indices for the start and end nodes
    int startIndex = (startNodeId - 1) * 3;
    int endIndex = (endNodeId - 1) * 3;

    // Debugging output: Print which nodes are being aggregated and their corresponding indices
    std::cout << "Aggregating local stiffness matrix for nodes: " << startNodeId << " to " << endNodeId << std::endl;
    std::cout << "Start index: " << startIndex << ", End index: " << endIndex << std::endl;

    // Get the total number of DOFs in the global stiffness matrix
    int numDOFs = globalMatrix.rows();

    // Check if the calculated indices are within the bounds of the global matrix
    if (startIndex + 2 >= numDOFs || endIndex + 2 >= numDOFs) {
        std::cerr << "Index out of bounds for nodes: " << startNodeId << " to " << endNodeId << std::endl;
        return; // Exit the function if indices are out of bounds
    }

    // Loop through the local stiffness matrix (6x6) to map its values into the global matrix
    for (int i = 0; i < 6; ++i) {
        // Determine the corresponding row in the global matrix
        // The first 3 rows/columns correspond to the start node, the next 3 to the end node
        int globalRow = (i < 3 ? startIndex : endIndex) + (i % 3);

        for (int j = 0; j < 6; ++j) {
            // Determine the corresponding column in the global matrix
            int globalCol = (j < 3 ? startIndex : endIndex) + (j % 3);

            // Ensure that the global indices are within bounds
            if (globalRow >= numDOFs || globalCol >= numDOFs) {
                std::cerr << "Index out of bounds: globalRow=" << globalRow << ", globalCol=" << globalCol << std::endl;
                continue; // Skip this iteration if indices are out of bounds
            }

            // Add the local matrix value to the global matrix at the calculated position
            globalMatrix(globalRow, globalCol) += localMatrix(i, j);
        }
    }
}

void Solver::assembleGlobalStiffnessMatrix()
{
    // Loop over all members in the structure
    for (const auto& memberPair : members) {
        int memberId = memberPair.first; // Get the member ID (not used directly in this method)
        const Member& member = memberPair.second; // Get the member object

        // Retrieve the local stiffness matrix for this member from the map
        const Eigen::MatrixXd& localMatrix = memberStiffnessMatrices[memberId];

        // Get the node IDs at the start and end of this member
        int startNodeId = member.getFirstNodeNumber();
        int endNodeId = member.getSecondNodeNumber();

        // Aggregate this local stiffness matrix into the global stiffness matrix
        aggregateStiffnessMatrix(K, localMatrix, startNodeId, endNodeId);

        // Debugging: Print the global stiffness matrix after each aggregation
        std::cout << "Global Stiffness Matrix after aggregating member " << memberId << ":\n" << K << std::endl;
    }
}

void Solver::applyBoundaryConditions()
{
    std::set<int> constrainedDOFs; // Use a set to avoid duplicates

    // Identify the constrained DOFs based on the nodeSupports
    for (const auto& supportPair : nodeSupports) {
        const NodeSupport& support = supportPair.second;
        int nodeId = supportPair.first;
        if (!support.getTx()) constrainedDOFs.insert(nodeId * 3 - 3);  // Tx DOF
        if (!support.getTz()) constrainedDOFs.insert(nodeId * 3 - 2);  // Tz DOF
        if (!support.getRy()) constrainedDOFs.insert(nodeId * 3 - 1);  // Ry DOF
    }

    // Debugging: Print constrained DOFs
    std::cout << "Constrained DOFs: ";
    for (int dof : constrainedDOFs) {
        std::cout << dof << " ";
    }
    std::cout << std::endl;

    int size = K.rows();
    if (constrainedDOFs.size() >= size) {
        std::cerr << "Error: Too many constrained DOFs! Check boundary conditions." << std::endl;
        return;
    }

    int reducedSize = size - constrainedDOFs.size();
    Eigen::MatrixXd reducedK(reducedSize, reducedSize);
    Eigen::VectorXd reducedF(reducedSize);

    int reducedRow = 0, reducedCol = 0;

    // Create reduced stiffness matrix and force vector
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

    // Replace K and loadVector with reduced versions
    K = reducedK;
    loadVector = reducedF;

    std::cout << "Boundary conditions applied successfully." << std::endl;
}

void Solver::createForceVector()
{
    std::cout << "Creating force vector..." << "\n";
    int numNodes = nodes.size();
    int size = numNodes * 3; // Each node has 3 degrees of freedom (DOFs)
    loadVector = Eigen::VectorXd::Zero(size); // Initialize a zero vector of the appropriate size

    // Debugging: Print number of nodes and size of load vector
    std::cout << "Number of nodes: " << numNodes << ", Load vector size: " << size << std::endl;

    // Add nodal loads to the load vector
    for (const auto& loadPair : nodeLoads) {
        const NodeLoad& load = loadPair.second;
        int nodeId = load.getNodeNumber();

        // Calculate DOF indices directly
        int dofIndex = (nodeId - 1) * 3;  // Assuming node IDs are 1-based and contiguous

        // Debugging: Print node ID and calculated DOF index
        std::cout << "Adding nodal load for node ID: " << nodeId << ", DOF index: " << dofIndex << std::endl;

        if (dofIndex >= size || dofIndex < 0) {
            std::cerr << "Error: DOF index out of bounds for node ID: " << nodeId << std::endl;
            continue; // Skip this node if the DOF index is out of bounds
        }

        loadVector(dofIndex) += load.getXLoad();
        loadVector(dofIndex + 1) += load.getZLoad();
        loadVector(dofIndex + 2) += load.getMLoad();

        // Debugging: Print the load values added
        std::cout << "Added loads: Fx=" << load.getXLoad() << ", Fz=" << load.getZLoad() << ", M=" << load.getMLoad() << std::endl;
    }

    // Add uniform loads to the load vector
    for (const auto& uniformLoadPair : uniformLoads) {
        const UniformLoad& uLoad = uniformLoadPair.second;
        int memberId = uLoad.getMemberId();

        // Debugging: Print member ID being processed
        std::cout << "Processing uniform load for member ID: " << memberId << std::endl;

        if (members.find(memberId) == members.end()) {
            std::cerr << "Error: memberId " << memberId << " not found in members!" << std::endl;
            continue; // Skip this load if memberId is not found
        }

        const Member& member = members.at(memberId);

        double qx = uLoad.getFx();
        double qz = uLoad.getFz();
        double L = member.getLength();

        // Equivalent nodal forces for a uniform load
        Eigen::VectorXd fLocal(6);
        fLocal << qx * L / 2, qz * L / 2, qz * L * L / 12, qx * L / 2, qz * L / 2, -qz * L * L / 12;

        int startNodeId = member.getFirstNodeNumber();
        int endNodeId = member.getSecondNodeNumber();

        // Calculate DOF indices directly
        int startDofIndex = (startNodeId - 1) * 3;  // Assuming node IDs are 1-based and contiguous
        int endDofIndex = (endNodeId - 1) * 3;      // Assuming node IDs are 1-based and contiguous

        // Debugging: Print node IDs and corresponding DOF indices
        std::cout << "Start node ID: " << startNodeId << ", Start DOF index: " << startDofIndex << std::endl;
        std::cout << "End node ID: " << endNodeId << ", End DOF index: " << endDofIndex << std::endl;

        if (startDofIndex >= size || startDofIndex < 0 || endDofIndex >= size || endDofIndex < 0) {
            std::cerr << "Error: DOF index out of bounds for member ID: " << memberId << std::endl;
            continue; // Skip this load if the DOF index is out of bounds
        }

        // Add the local forces to the global load vector
        loadVector.segment<3>(startDofIndex) += fLocal.segment<3>(0);
        loadVector.segment<3>(endDofIndex) += fLocal.segment<3>(3);

        // Debugging: Print the local forces added to the global vector
        std::cout << "Added uniform load to global vector: " << fLocal.transpose() << std::endl;
    }

    std::cout << "Force vector creation completed successfully." << std::endl;
}

void Solver::solveSystemOfEquations()
{
    std::cout << "Entering solveSystemOfEquations..." << "\n";

    // Print sizes of K and loadVector for debugging
    std::cout << "K matrix size: " << K.rows() << "x" << K.cols() << std::endl;
    std::cout << "loadVector size: " << loadVector.size() << std::endl;

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
        if (!support.getTx()) constrainedDOFs.push_back(nodeId * 3 - 3);
        if (!support.getTz()) constrainedDOFs.push_back(nodeId * 3 - 2);
        if (!support.getRy()) constrainedDOFs.push_back(nodeId * 3 - 1);
    }

    int reducedIndex = 0;
    for (int i = 0; i < size; ++i) {
        if (std::find(constrainedDOFs.begin(), constrainedDOFs.end(), i) == constrainedDOFs.end()) {
            if (reducedIndex >= U.size()) {
                std::cerr << "Error: reducedIndex out of bounds while mapping displacements!" << std::endl;
                return;
            }
            displacementVector(i) = U(reducedIndex);
            ++reducedIndex;
        }
    }

    std::cout << "System solved and displacements mapped." << std::endl;
}

void Solver::calculateInternalForces()
{
    for (const auto& memberPair : members) {
        int memberId = memberPair.first;
        const Member& member = memberPair.second;

        int startNodeId = member.getFirstNodeNumber();
        int endNodeId = member.getSecondNodeNumber();

        int startDofIndex = (startNodeId - 1) * 3;
        int endDofIndex = (endNodeId - 1) * 3;

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

        // Print the internal forces with labels
        std::cout << "Member ID: " << memberId << ", Internal Forces: "
                  << "Nx1=" << fInternal(0) << " "
                  << "Vz1=" << fInternal(1) << " "
                  << "My1=" << fInternal(2) << " "
                  << "Nx2=" << fInternal(3) << " "
                  << "Vz2=" << fInternal(4) << " "
                  << "My2=" << fInternal(5) << std::endl;
    }
}


void Solver::solve()
{
    calculateStiffnessMatrices();

    initializeGlobalStiffnessMatrix();
    assembleGlobalStiffnessMatrix();
    std::cout << "Global stiffness matrix assembled" << std::endl;
    createForceVector();
    std::cout << "Force vector created" << std::endl;
    applyBoundaryConditions();
    std::cout << "Boundaries applied" << std::endl;
    solveSystemOfEquations();
    calculateInternalForces();
}

} // namespace SolverFEM
