#ifndef SOLVER_H
#define SOLVER_H

#include "node.h"
#include "nodeload.h"
#include "nodesupport.h"
#include "member.h"
#include "uniformload.h"
#include "matrixoperations.h"
#include "MemberSupportConditions.h"
#include "../DataBaseManagers/DataBaseManagers.h"
#include <Eigen/Dense>
#include <map>

// Forward declaration of DataBaseSolverPreparer
namespace DataBaseManagers {
class DataBaseSolverPreparer;
class DataBaseResultsManager;
}

namespace SolverFEM {

class Solver
{
public:
    Solver(DataBaseManagers::DataBaseSolverPreparer* dbSolverPreparer);

    void solve();
    void saveResultsToDataBase(DataBaseManagers::DataBaseResultsManager *dbResultsManager);

private:
    std::map<int, SolverFEM::Node> nodes;
    std::map<int, SolverFEM::NodeLoad> nodeLoads;
    std::map<int, SolverFEM::NodeSupport> nodeSupports;
    std::map<int, SolverFEM::Member> members;
    std::map<int, SolverFEM::UniformLoad> uniformLoads;
    std::map<int, SolverFEM::MemberSupportConditions> memberSupportConditions;

    Eigen::MatrixXd K; // Global stiffness matrix
    Eigen::VectorXd loadVector; // Global load vector
    Eigen::VectorXd displacementVector; // Global displacement vector

    std::map<int, Eigen::MatrixXd> memberStiffnessMatrices;
    std::map<int, int> nodeIdToDofMap; // Map node IDs to DOF indices

    void createNodeIdToDofMap(); // Initializes the nodeIdToDofMap
    void calculateStiffnessMatrices();
    void initializeGlobalStiffnessMatrix();
    void aggregateStiffnessMatrix(Eigen::MatrixXd& globalMatrix, const Eigen::MatrixXd& localMatrix, int startNodeId, int endNodeId);
    void assembleGlobalStiffnessMatrix();
    void createForceVector();
    void applyBoundaryConditions();
    void solveSystemOfEquations();
    void calculateInternalForces();
};

} // namespace SolverFEM

#endif // SOLVER_H
