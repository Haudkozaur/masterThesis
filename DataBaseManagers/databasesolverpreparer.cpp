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
    createNodeSupports();
    createMemberSupportConditions();
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
        double xCord = point.second.first / 1000; // for nodes we operate in [m]
        double zCord = point.second.second / 1000;
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
        double length = std::get<3>(line.second) / 1000; //for members we operate in [m]

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

        membersMap.emplace(id,
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

void DataBaseSolverPreparer::createNodeSupports()
{
    nodeSupportsMap.clear();
    for (const auto &support : supportsMap) {
        int id = support.first;
        int nodeId = std::get<0>(support.second);
        bool Ry = std::get<1>(support.second);
        bool Tz = std::get<2>(support.second);
        bool Tx = std::get<3>(support.second);
        nodeSupportsMap.emplace(id, SolverFEM::NodeSupport(id, nodeId, Tx, Tz, Ry));
    }

    // Print node supports for debugging
    for (const auto &nodeSupport : nodeSupportsMap) {
        std::cout << "Node Support ID: " << nodeSupport.first
                  << ", Node ID: " << nodeSupport.second.getNodeId()
                  << ", Tx: " << nodeSupport.second.getTx()
                  << ", Tz: " << nodeSupport.second.getTz()
                  << ", Ry: " << nodeSupport.second.getRy() << std::endl;
    }
}

void DataBaseSolverPreparer::createMemberSupportConditions()
{
    memberSupportConditionsMap.clear();
    for (const auto &member : membersMap) {
        int memberId = member.first;
        int startNodeId = member.second.getFirstNodeNumber();
        int endNodeId = member.second.getSecondNodeNumber();

        // Fetch support conditions for start node
        bool startTx = false, startTz = false, startRy = false;
        for (const auto &support : supportsMap) {
            if (std::get<0>(support.second) == startNodeId) {
                startTx = std::get<3>(support.second);
                startTz = std::get<2>(support.second);
                startRy = std::get<1>(support.second);
                break;
            }
        }

        // Fetch support conditions for end node
        bool endTx = false, endTz = false, endRy = false;
        for (const auto &support : supportsMap) {
            if (std::get<0>(support.second) == endNodeId) {
                endTx = std::get<3>(support.second);
                endTz = std::get<2>(support.second);
                endRy = std::get<1>(support.second);
                break;
            }
        }

        memberSupportConditionsMap.emplace(memberId, SolverFEM::MemberSupportConditions(
                                                         memberId, startNodeId, endNodeId, startTx, startTz, startRy, endTx, endTz, endRy));
    }

    // Print member support conditions for debugging
    for (const auto &memberSupportCondition : memberSupportConditionsMap) {
        std::cout << "Member ID: " << memberSupportCondition.first
                  << ", Start Node ID: " << memberSupportCondition.second.getStartNodeId()
                  << ", End Node ID: " << memberSupportCondition.second.getEndNodeId()
                  << ", Start Tx: " << memberSupportCondition.second.getStartTx()
                  << ", Start Tz: " << memberSupportCondition.second.getStartTz()
                  << ", Start Ry: " << memberSupportCondition.second.getStartRy()
                  << ", End Tx: " << memberSupportCondition.second.getEndTx()
                  << ", End Tz: " << memberSupportCondition.second.getEndTz()
                  << ", End Ry: " << memberSupportCondition.second.getEndRy() << std::endl;
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

const std::map<int, SolverFEM::NodeSupport> &DataBaseSolverPreparer::getNodeSupports() const
{
    return nodeSupportsMap;
}

const std::map<int, SolverFEM::MemberSupportConditions> &DataBaseSolverPreparer::getMemberSupportConditions() const
{
    return memberSupportConditionsMap;
}

} // namespace DataBaseManagers
