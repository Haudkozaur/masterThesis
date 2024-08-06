#ifndef DATABASESOLVERPREPARER_H
#define DATABASESOLVERPREPARER_H

#include "../Solver/Solver.h"
#include "DataBaseModelObjectsManager.h"
#include <map>
#include <string>
#include <vector>

namespace DataBaseManagers {

class DataBaseSolverPreparer : public DataBaseModelObjectsManager
{
public:
    explicit DataBaseSolverPreparer(const std::string &dateBaseName);

    void fetchAllData();

    const std::map<int, std::pair<int, int>> &getPoints() const;
    const std::map<int, SolverFEM::Node> &getNodes() const;
    const std::map<int, SolverFEM::Member> &getMembers() const;
    const std::map<int, SolverFEM::NodeLoad> &getNodeLoads() const;
    const std::map<int, SolverFEM::UniformLoad> &getUniformLoads() const;

private:
    void fetchPoints();
    void fetchLines();
    void fetchMaterials();
    void fetchCrossSections();
    void fetchSupports();
    void fetchNodalLoads();
    void fetchLineLoads();

    void createNodes();
    void createMembers();
    void createNodeLoads();
    void createUniformLoads();

    std::map<int, std::pair<int, int>> pointsMap;
    std::map<int, std::tuple<int, int, int, double>> linesMap;
    std::map<int, SolverFEM::Node> nodesMap;
    std::map<int, SolverFEM::Member> membersMap;
    std::map<int, SolverFEM::NodeLoad> nodeLoadsMap;
    std::map<int, SolverFEM::UniformLoad> uniformLoadsMap;
    std::map<int, std::tuple<std::string, double, double>> materialsMap;

    std::map<int, std::tuple<std::string, int, double, double>> crossSectionsMap;
    std::map<int, std::tuple<int, bool, bool, bool>> supportsMap;
    std::map<int, std::tuple<int, double, double, double>> nodalLoadsMap;
    std::map<int, std::tuple<int, double, double>> lineLoadsMap;
};

} // namespace DataBaseManagers

#endif // DATABASESOLVERPREPARER_H
