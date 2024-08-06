#include "DataBaseSolverPreparer.h"
#include <iostream>
#include <tuple>

namespace DataBaseManagers {

DataBaseSolverPreparer::DataBaseSolverPreparer(const std::string &dateBaseName)
    : DataBaseModelObjectsManager(dateBaseName)
{}

void DataBaseSolverPreparer::fetchAllData()
{
    fetchPoints();
    fetchLines();
    fetchMaterials();
    fetchCrossSections();
    fetchSupports();
    fetchNodalLoads();
    fetchLineLoads();

    createNodes();
    createMembers();
    createNodeLoads();
    createUniformLoads();
}

void DataBaseSolverPreparer::fetchPoints()
{
    std::string query = "SELECT id, x_cord, z_cord FROM points";
    std::vector<std::vector<std::string>> results = executeQuery(query);

    pointsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 3) {
            int id = std::stoi(row[0]);
            double xCord = std::stod(row[1]);
            double zCord = std::stod(row[2]);
            pointsMap[id] = std::make_pair(xCord, zCord);
        }
    }
}

void DataBaseSolverPreparer::fetchLines()
{
    std::string query = "SELECT id, start_point, end_point, cross_section_id, length FROM lines";
    std::vector<std::vector<std::string>> results = executeQuery(query);

    linesMap.clear();
    for (const auto &row : results) {
        if (row.size() == 5) {
            int id = std::stoi(row[0]);
            int startPoint = std::stoi(row[1]);
            int endPoint = std::stoi(row[2]);
            int crossSectionId = std::stoi(row[3]);
            double length = std::stod(row[4]);
            linesMap[id] = std::make_tuple(startPoint, endPoint, crossSectionId, length);
        }
    }
}

void DataBaseSolverPreparer::fetchMaterials()
{
    std::string query = "SELECT id, name, E, v FROM materials";
    std::vector<std::vector<std::string>> results = executeQuery(query);

    materialsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 4) {
            int id = std::stoi(row[0]);
            std::string name = row[1];
            double E = std::stod(row[2]);
            double v = std::stod(row[3]);
            materialsMap[id] = std::make_tuple(name, E, v);
        }
    }
}

void DataBaseSolverPreparer::fetchCrossSections()
{
    std::string query = "SELECT id, name, material_id, A, I FROM cross_sections";
    std::vector<std::vector<std::string>> results = executeQuery(query);

    crossSectionsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 5) {
            int id = std::stoi(row[0]);
            std::string name = row[1];
            int materialId = std::stoi(row[2]);
            double A = std::stod(row[3]);
            double I = std::stod(row[4]);
            crossSectionsMap[id] = std::make_tuple(name, materialId, A, I);
        }
    }
}

void DataBaseSolverPreparer::fetchSupports()
{
    std::string query = "SELECT id, point_id, ry, tz, tx FROM supports";
    std::vector<std::vector<std::string>> results = executeQuery(query);

    supportsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 5) {
            int id = std::stoi(row[0]);
            int pointId = std::stoi(row[1]);
            bool ry = std::stoi(row[2]);
            bool tz = std::stoi(row[3]);
            bool tx = std::stoi(row[4]);
            supportsMap[id] = std::make_tuple(pointId, ry, tz, tx);
        }
    }
}

void DataBaseSolverPreparer::fetchNodalLoads()
{
    std::string query = "SELECT id, point_id, My, Fz, Fx FROM nodal_loads";
    std::vector<std::vector<std::string>> results = executeQuery(query);

    nodalLoadsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 5) {
            int id = std::stoi(row[0]);
            int pointId = std::stoi(row[1]);
            double My = std::stod(row[2]);
            double Fz = std::stod(row[3]);
            double Fx = std::stod(row[4]);
            nodalLoadsMap[id] = std::make_tuple(pointId, My, Fz, Fx);
        }
    }
}

void DataBaseSolverPreparer::fetchLineLoads()
{
    std::string query = "SELECT id, line_id, Fx, Fz FROM line_loads";
    std::vector<std::vector<std::string>> results = executeQuery(query);

    lineLoadsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 4) {
            int id = std::stoi(row[0]);
            int lineId = std::stoi(row[1]);
            double Fx = std::stod(row[2]);
            double Fz = std::stod(row[3]);
            lineLoadsMap[id] = std::make_tuple(lineId, Fx, Fz);
        }
    }
}

void DataBaseSolverPreparer::createNodes()
{
    nodesMap.clear();
    for (const auto &point : pointsMap) {
        int id = point.first;
        double xCord = point.second.first;
        double zCord = point.second.second;
        nodesMap.emplace(id, SolverFEM::Node(id, xCord, zCord));
    }

    // Print nodes for debugging
    for (const auto &node : nodesMap) {
        std::cout << "Node ID: " << node.first << ", X: " << node.second.getX()
                  << ", Z: " << node.second.getZ() << std::endl;
    }
}

void DataBaseSolverPreparer::createMembers()
{
    membersMap.clear();
    for (const auto &line : linesMap) {
        int id = line.first;
        int startPoint = std::get<0>(line.second);
        int endPoint = std::get<1>(line.second);
        int crossSectionId = std::get<2>(line.second);
        double length = std::get<3>(line.second);

        auto crossSection = crossSectionsMap[crossSectionId];
        std::string crossSectionName = std::get<0>(crossSection);
        int materialId = std::get<1>(crossSection);
        double A = std::get<2>(crossSection);
        double I = std::get<3>(crossSection);

        auto material = materialsMap[materialId];
        std::string materialName = std::get<0>(material);
        double E = std::get<1>(material);
        double v = std::get<2>(material);

        double x1 = nodesMap.at(startPoint).getX();
        double z1 = nodesMap.at(startPoint).getZ();
        double x2 = nodesMap.at(endPoint).getX();
        double z2 = nodesMap.at(endPoint).getZ();

        membersMap
            .emplace(id,
                     SolverFEM::Member(startPoint, endPoint, x1, z1, x2, z2, E, v, I, A, length));
    }

    // Print members for debugging
    for (const auto &member : membersMap) {
        std::cout << "Member ID: " << member.first
                  << ", Start Node: " << member.second.getFirstNodeNumber()
                  << ", End Node: " << member.second.getSecondNodeNumber()
                  << ", E: " << member.second.getE() << ", v: " << member.second.getV()
                  << ", I: " << member.second.getI() << ", A: " << member.second.getA()
                  << ", Length: " << member.second.getLength() << ", X1: " << member.second.getX1()
                  << ", Z1: " << member.second.getZ1() << ", X2: " << member.second.getX2()
                  << ", Z2: " << member.second.getZ2() << std::endl;
    }
}

void DataBaseSolverPreparer::createNodeLoads()
{
    nodeLoadsMap.clear();
    for (const auto &nodalLoad : nodalLoadsMap) {
        int id = nodalLoad.first;
        int nodeId = std::get<0>(nodalLoad.second);
        double My = std::get<1>(nodalLoad.second);
        double Fz = std::get<2>(nodalLoad.second);
        double Fx = std::get<3>(nodalLoad.second);
        nodeLoadsMap.emplace(id, SolverFEM::NodeLoad(nodeId, Fx, Fz, My));
    }

    // Print node loads for debugging
    for (const auto &nodeLoad : nodeLoadsMap) {
        std::cout << "Node Load ID: " << nodeLoad.first
                  << ", Node ID: " << nodeLoad.second.getNodeNumber()
                  << ", Fx: " << nodeLoad.second.getXLoad()
                  << ", Fz: " << nodeLoad.second.getZLoad()
                  << ", My: " << nodeLoad.second.getMLoad() << std::endl;
    }
}

void DataBaseSolverPreparer::createUniformLoads()
{
    uniformLoadsMap.clear();
    for (const auto &lineLoad : lineLoadsMap) {
        int id = lineLoad.first;
        int lineId = std::get<0>(lineLoad.second);
        double Fx = std::get<1>(lineLoad.second);
        double Fz = std::get<2>(lineLoad.second);

        uniformLoadsMap.emplace(id, SolverFEM::UniformLoad(id, lineId, Fx, Fz));
    }
    for (const auto &uniformLoad : uniformLoadsMap) {
        std::cout << "Uniform Load ID: " << uniformLoad.first
                  << ", Member ID: " << uniformLoad.second.getMemberId()
                  << ", Fx: " << uniformLoad.second.getFx()
                  << ", Fz: " << uniformLoad.second.getFz() << std::endl;
    }
}

const std::map<int, std::pair<int, int>> &DataBaseSolverPreparer::getPoints() const
{
    return pointsMap;
}

const std::map<int, SolverFEM::Node> &DataBaseSolverPreparer::getNodes() const
{
    return nodesMap;
}

const std::map<int, SolverFEM::Member> &DataBaseSolverPreparer::getMembers() const
{
    return membersMap;
}

const std::map<int, SolverFEM::NodeLoad> &DataBaseSolverPreparer::getNodeLoads() const
{
    return nodeLoadsMap;
}

const std::map<int, SolverFEM::UniformLoad> &DataBaseSolverPreparer::getUniformLoads() const
{
    return uniformLoadsMap;
}

} // namespace DataBaseManagers
