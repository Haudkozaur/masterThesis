#include "DataBaseResultsManager.h"
#include "TableTypes.h"
#include "toStringWithPrecision.h"

namespace DataBaseManagers {

DataBaseResultsManager::DataBaseResultsManager(std::string dateBaseName)
    : DataBaseModelObjectsManager(std::move(dateBaseName)) {
}

// Method to add or update node information in the nodes table
void DataBaseManagers::DataBaseResultsManager::addOrUpdateNodeToDataBase(int nodeId, double XCord, double ZCord)
{
    std::string queryAddOrUpdateNode = "INSERT INTO nodes (node_id, x_cord, z_cord) VALUES ("
                                       + std::to_string(nodeId) + ", "
                                       + toStringWithPrecision(XCord) + ", " + toStringWithPrecision(ZCord) + ") "
                                       + "ON CONFLICT(node_id) DO UPDATE SET x_cord = excluded.x_cord, z_cord = excluded.z_cord";
    executeAndCheckIfSQLOk(queryAddOrUpdateNode, TableType::NODES);
}

// Method to add internal force results to the results table
void DataBaseManagers::DataBaseResultsManager::addResultToDataBase(int lineId, int memberId, int nodeId, double Nx, double Vz, double My, double deformation, bool isStart)
{
    std::string queryAddResult = "INSERT INTO " + tableTypesMap.at(TableType::RESULTS)
                                 + " (line_Id, member_Id, node_Id, Nx, Vz, My, deformation, isStart) VALUES ("
                                 + std::to_string(lineId) + ", " + std::to_string(memberId) + ", " + std::to_string(nodeId) + ", "
                                 + toStringWithPrecision(Nx) + ", " + toStringWithPrecision(Vz) + ", "
                                 + toStringWithPrecision(My) + ", " + toStringWithPrecision(deformation) + ", "
                                 + std::to_string(isStart) + ")";
    executeAndCheckIfSQLOk(queryAddResult, TableType::RESULTS);
}

// Method to iterate over the results table and store the data in the resultsMap
void DataBaseManagers::DataBaseResultsManager::iterateOverTable()
{
    // Fetch data from the nodes table
    std::string querySelectNodes = "SELECT node_id, x_cord, z_cord FROM " + tableTypesMap.at(TableType::NODES);
    std::vector<std::vector<std::string>> nodesData = executeQuery(querySelectNodes);

    // Fetch data from the results table
    std::string querySelectResults = "SELECT line_Id, member_id, node_id, Nx, Vz, My, deformation, isStart FROM " + tableTypesMap.at(TableType::RESULTS);
    std::vector<std::vector<std::string>> resultsData = executeQuery(querySelectResults);

    std::cout << "Fetched data from nodes and results tables." << std::endl;

    resultsMap.clear();

    // Map to store node coordinates indexed by node_id
    std::map<int, std::pair<double, double>> nodeCoords;

    // Populate the nodeCoords map with data from the nodes table
    for (const auto& row : nodesData) {
        if (row.size() == 3) {  // Ensure row size matches expected columns
            int nodeId = std::stoi(row[0]);
            double xCord = std::stod(row[1]);
            double zCord = std::stod(row[2]);

            nodeCoords[nodeId] = std::make_pair(xCord, zCord);
        } else {
            std::cerr << "Error: Unexpected row size in nodes table. Expected 3, got " << row.size() << std::endl;
        }
    }

    // Now process the resultsData and merge with node coordinates
    for (const auto& row : resultsData) {
        if (row.size() == 8) {  // Ensure row size matches expected columns
            try {
                int lineId = std::stoi(row[0]);
                int memberId = std::stoi(row[1]);
                int nodeId = std::stoi(row[2]);
                double Nx = std::stod(row[3]);
                double Vz = std::stod(row[4]);
                double My = std::stod(row[5]);
                double deformation = std::stod(row[6]);
                bool isStart = std::stoi(row[7]);

                // Fetch the coordinates from nodeCoords
                if (nodeCoords.find(nodeId) != nodeCoords.end()) {
                    double xCord = nodeCoords[nodeId].first;
                    double zCord = nodeCoords[nodeId].second;

                    std::cout << "Parsed Line ID: " << lineId << " Member ID: " << memberId << " Node ID: " << nodeId
                              << " xCord: " << xCord << " zCord: " << zCord
                              << " Nx: " << Nx << " Vz: " << Vz << " My: " << My
                              << " Deformation: " << deformation << " IsStart: " << isStart << std::endl;

                    // Create the tuple
                    auto resultTuple = std::make_tuple(memberId, xCord, zCord, Nx, Vz, My, deformation, lineId, isStart);

                    // If nodeId exists in the map, add the tuple to its vector
                    if (resultsMap.find(nodeId) != resultsMap.end()) {
                        resultsMap[nodeId].push_back(resultTuple);
                    } else {
                        // Otherwise, create a new entry with a vector containing the tuple
                        resultsMap[nodeId] = { resultTuple };
                    }
                } else {
                    std::cerr << "Warning: Node ID " << nodeId << " not found in nodes table." << std::endl;
                }

            } catch (const std::exception& e) {
                std::cerr << "Error during parsing: " << e.what() << std::endl;
                continue; // Skip this row if there was an error
            }
        } else {
            std::cerr << "Error: Unexpected row size in results table. Expected 8, got " << row.size() << std::endl;
        }
    }
}

// Method to retrieve the results map
std::map<int, std::vector<std::tuple<int, double, double, double, double, double, double, int, bool> > > DataBaseManagers::DataBaseResultsManager::getResultsMap() const
{
    return resultsMap;
}

} // namespace DataBaseManagers
