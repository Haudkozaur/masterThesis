#include "DataBaseSolverPreparer.h"
#include <iostream>
#include <tuple>
#include <map>
#include <cmath>

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
    fetchMesh();

    createFiniteElements();
    createNodeLoads();
    createUniformLoads();
    createNodeSupports();
    createMemberSupportConditions();
    //print size of membersMap:
    std::cout << "MembersMap size: " << membersMap.size() << std::endl;

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

void DataBaseSolverPreparer::fetchMesh()
{
    std::string query = "SELECT id, line_id, node_x, node_z FROM mesh";
    std::vector<std::vector<std::string>> results = executeQuery(query);

    meshMap.clear();
    for (const auto &row : results) {
        if (row.size() == 4) {
            int id = std::stoi(row[0]);
            int lineId = std::stoi(row[1]);
            double nodeX = std::stod(row[2]);
            double nodeZ = std::stod(row[3]);

            meshMap[id] = std::make_pair(lineId, std::make_pair(nodeX, nodeZ));
        }
    }

    for (const auto &mesh : meshMap) {
        std::cout << "Mesh ID: " << mesh.first
                  << ", Line ID: " << mesh.second.first
                  << ", X: " << mesh.second.second.first
                  << ", Z: " << mesh.second.second.second << std::endl;
    }
}

void DataBaseSolverPreparer::createFiniteElements() {
    nodesMap.clear();
    membersMap.clear();

    // Check if pointsMap and linesMap are empty
    if (pointsMap.empty()) {
        std::cerr << "Error: pointsMap is empty. No points available to create finite elements." << std::endl;
        return; // Exit the function early
    }

    if (linesMap.empty()) {
        std::cerr << "Error: linesMap is empty. No lines available to create finite elements." << std::endl;
        return; // Exit the function early
    }

    // Determine the next available ID for new mesh nodes
    int maxPointId = pointsMap.rbegin()->first; // The maximum ID in pointsMap
    int nextAvailableId = maxPointId + 1;       // Start new IDs after the highest point ID

    for (const auto &line : linesMap) {
        int lineId = line.first;
        int startPointId = std::get<0>(line.second);
        int endPointId = std::get<1>(line.second);
        int crossSectionId = std::get<2>(line.second);

        std::vector<std::pair<int, SolverFEM::Node>> lineNodes;

        // Add the start point node
        if (nodesMap.find(startPointId) == nodesMap.end()) {
            double xCord = pointsMap.at(startPointId).first / 1000.0; // Assuming coordinates in meters
            double zCord = pointsMap.at(startPointId).second / 1000.0;
            nodesMap.emplace(startPointId, SolverFEM::Node(startPointId, xCord, zCord));
        }
        lineNodes.push_back({startPointId, nodesMap.at(startPointId)});
        std::cout << "Start point node ID: " << startPointId << std::endl;

        // Add mesh nodes associated with this line, ensuring unique IDs
        for (const auto &mesh : meshMap) {
            if (mesh.second.first == lineId) {
                double xCord = mesh.second.second.first / 1000.0; // Assuming coordinates in meters
                double zCord = mesh.second.second.second / 1000.0;

                // Assign a unique ID if the mesh node ID is already in use
                int meshNodeId = nextAvailableId++;
                nodesMap.emplace(meshNodeId, SolverFEM::Node(meshNodeId, xCord, zCord));
                lineNodes.push_back({meshNodeId, nodesMap.at(meshNodeId)});
            }
        }

        // Add the end point node
        if (nodesMap.find(endPointId) == nodesMap.end()) {
            double xCord = pointsMap.at(endPointId).first / 1000.0; // Assuming coordinates in meters
            double zCord = pointsMap.at(endPointId).second / 1000.0;
            nodesMap.emplace(endPointId, SolverFEM::Node(endPointId, xCord, zCord));
        }
        lineNodes.push_back({endPointId, nodesMap.at(endPointId)});
        std::cout << "End point node ID: " << endPointId << std::endl;

        // Sort nodes by their X-coordinate along the line
        std::sort(lineNodes.begin(), lineNodes.end(), [](const auto &a, const auto &b) {
            return a.second.getX() < b.second.getX();
        });

        std::cout << "Line ID: " << lineId << " has nodes: " << std::endl;
        for (const auto &node : lineNodes) {
            std::cout << "  Node ID: " << node.first << ", X: " << node.second.getX()
                      << ", Z: " << node.second.getZ() << std::endl;
        }

        // Create members between nodes
        for (size_t i = 0; i < lineNodes.size() - 1; ++i) {
            int nodeId1 = lineNodes[i].first;
            int nodeId2 = lineNodes[i + 1].first;

            double x1 = lineNodes[i].second.getX();
            double z1 = lineNodes[i].second.getZ();
            double x2 = lineNodes[i + 1].second.getX();
            double z2 = lineNodes[i + 1].second.getZ();

            double length = std::sqrt(std::pow(x2 - x1, 2) + std::pow(z2 - z1, 2));
            std::cout << "Calculated length between Node " << nodeId1 << " and Node " << nodeId2
                      << " is: " << length << std::endl;
            if (length == 0) {
                std::cerr << "Warning: Zero-length member between nodes " << nodeId1 << " and " << nodeId2 << std::endl;
                continue;
            }

            auto crossSection = crossSectionsMap[crossSectionId];
            std::string crossSectionName = std::get<0>(crossSection);
            int materialId = std::get<1>(crossSection);
            double A = std::get<2>(crossSection);
            double I = std::get<3>(crossSection);

            auto material = materialsMap[materialId];
            std::string materialName = std::get<0>(material);
            double E = std::get<1>(material);
            double v = std::get<2>(material);

            int memberId = membersMap.size() + 1;
            membersMap.emplace(memberId, SolverFEM::Member(nodeId1, nodeId2, x1, z1, x2, z2, E, v, I, A, length, lineId));

            std::cout << "Member ID " << memberId << " added: Start Node " << nodeId1 << " to End Node " << nodeId2 << std::endl;
        }
    }

    std::cout << "Total members created: " << membersMap.size() << std::endl;

    for (const auto &member : membersMap) {
        std::cout << "Member ID: " << member.first
                  << ", Start Node: " << member.second.getFirstNodeNumber()
                  << ", End Node: " << member.second.getSecondNodeNumber()
                  << ", E: " << member.second.getE()
                  << ", v: " << member.second.getV()
                  << ", I: " << member.second.getI()
                  << ", A: " << member.second.getA()
                  << ", Length: " << member.second.getLength()
                  << ", X1: " << member.second.getX1()
                  << ", Z1: " << member.second.getZ1()
                  << ", X2: " << member.second.getX2()
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
        int loadId = lineLoad.first;
        int lineId = std::get<0>(lineLoad.second);
        double Fx = std::get<1>(lineLoad.second);
        double Fz = std::get<2>(lineLoad.second);

        // Find all members associated with this lineId
        for (const auto &member : membersMap) {
            int memberId = member.first;
            const SolverFEM::Member &memberData = member.second;

            if (memberData.getLineId() == lineId) {
                int newLoadId = uniformLoadsMap.size() + 1; // Generate unique ID for each member's load
                uniformLoadsMap.emplace(newLoadId, SolverFEM::UniformLoad(newLoadId, memberId, Fx, Fz));

                std::cout << "Uniform Load ID: " << newLoadId
                          << ", Member ID: " << memberId
                          << ", Fx: " << Fx
                          << ", Fz: " << Fz << std::endl;
            }
        }
    }

    std::cout << "Total uniform loads created: " << uniformLoadsMap.size() << std::endl;
}


void DataBaseSolverPreparer::createNodeSupports()
{
    nodeSupportsMap.clear();

    for (const auto &support : supportsMap) {
        int id = support.first; // Support ID
        int nodeId = std::get<0>(support.second); // Node ID
        bool Ry = std::get<1>(support.second);
        bool Tz = std::get<2>(support.second);
        bool Tx = std::get<3>(support.second);

        // Debugging: Print the support details before adding to the map
        std::cout << "Adding Node Support: ID=" << id
                  << ", NodeID=" << nodeId
                  << ", Tx=" << Tx << ", Tz=" << Tz << ", Ry=" << Ry << std::endl;

        // Use nodeId as the key instead of support ID
        nodeSupportsMap.emplace(nodeId, SolverFEM::NodeSupport(id, nodeId, Tx, Tz, Ry));
    }

    // Debugging: Print the entire nodeSupportsMap after populating
    std::cout << "Final Node Supports Map:" << std::endl;
    for (const auto &nodeSupport : nodeSupportsMap) {
        std::cout << "Node Support ID: " << nodeSupport.second.getId() // Print using the Node ID
                  << ", Node ID: " << nodeSupport.first
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

        // Initialize support conditions for start and end nodes
        bool startTx = false, startTz = false, startRy = false;
        bool endTx = false, endTz = false, endRy = false;

        // Debugging: Print the member ID and its start/end nodes
        std::cout << "Processing Member ID: " << memberId
                  << ", Start Node ID: " << startNodeId
                  << ", End Node ID: " << endNodeId << std::endl;

        // Check for support at the start node
        if (nodeSupportsMap.find(startNodeId) != nodeSupportsMap.end()) {
            const SolverFEM::NodeSupport& startSupport = nodeSupportsMap.at(startNodeId);
            startTx = startSupport.getTx();
            startTz = startSupport.getTz();
            startRy = startSupport.getRy();
            // Debugging: Print the support conditions for the start node
            std::cout << "Support found for Start Node ID: " << startNodeId
                      << " (Tx: " << startTx << ", Tz: " << startTz << ", Ry: " << startRy << ")" << std::endl;
        } else {
            std::cout << "No support found for Start Node ID: " << startNodeId << std::endl;
        }

        // Check for support at the end node
        if (nodeSupportsMap.find(endNodeId) != nodeSupportsMap.end()) {
            const SolverFEM::NodeSupport& endSupport = nodeSupportsMap.at(endNodeId);
            endTx = endSupport.getTx();
            endTz = endSupport.getTz();
            endRy = endSupport.getRy();
            // Debugging: Print the support conditions for the end node
            std::cout << "Support found for End Node ID: " << endNodeId
                      << " (Tx: " << endTx << ", Tz: " << endTz << ", Ry: " << endRy << ")" << std::endl;
        } else {
            std::cout << "No support found for End Node ID: " << endNodeId << std::endl;
        }

        // Add member support conditions to the map
        memberSupportConditionsMap.emplace(memberId, SolverFEM::MemberSupportConditions(
                                                         memberId, startNodeId, endNodeId, startTx, startTz, startRy, endTx, endTz, endRy));
    }

    // Debugging: Print the final member support conditions
    std::cout << "Final Member Support Conditions:" << std::endl;
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
