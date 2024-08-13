#include "DataBaseMeshManager.h"
#include "toStringWithPrecision.h"

namespace DataBaseManagers {

DataBaseMeshManager::DataBaseMeshManager(const string &dateBaseName)
    : DataBaseModelObjectsManager(dateBaseName){
}

void DataBaseMeshManager::addObjectToDataBase(int lineID, double nodeX, double nodeZ)
{
    string queryAddMesh = "INSERT INTO " + tableTypesMap.at(TableType::MESH)
                          + " (id, line_id, node_x, node_z) VALUES (NULL, "+  to_string(lineID) + ", "
                          + toStringWithPrecision(nodeX) + ", " + toStringWithPrecision(nodeZ) + ")";
    //temporarily commented out validation
    //TODO: consider if validation is needed here
    //if (validate(lineID, TableType::LINES)) {
        // cout << "Line exists" << endl;
        executeAndCheckIfSQLOk(queryAddMesh, TableType::MESH);
    // }
    // else {
    //     cout << "Error: "
    //          << "line doesn't exist in DB" << endl;
    //     return;
    // }
    cout << "\n";
}

void DataBaseMeshManager::deleteObjectFromDataBase(int lineID)
{
    string queryDeleteMesh = "DELETE FROM " + tableTypesMap.at(TableType::MESH)
                             + " WHERE line_id = " + to_string(lineID);
    executeAndCheckIfSQLOk(queryDeleteMesh, TableType::MESH);
    cout << "\n";
}

void DataBaseMeshManager::iterateOverTable()
{
    std::string querySelectAllMesh = "SELECT * FROM " + tableTypesMap.at(TableType::MESH);
    std::vector<std::vector<string>> results = executeQuery(querySelectAllMesh);

    meshMap.clear(); // Clear the map before populating it

    for (const auto& row : results) {
        if (row.size() == 4) {
            int id = stoi(row[0]);
            int lineID = stoi(row[1]);
            double nodeX = stod(row[2]);
            double nodeZ = stod(row[3]);
            meshMap[id] = make_pair(lineID, make_pair(nodeX, nodeZ));
        }
    }
    //print meshmap for debugging
    for (const auto& mesh : meshMap) {
        cout << "ID: " << mesh.first << ", Line ID: " << mesh.second.first << ", X: " << mesh.second.second.first
             << ", Z: " << mesh.second.second.second << endl;
    }

}
void DataBaseMeshManager::clearTable()
{
    string queryClearTable = "DELETE FROM " + tableTypesMap.at(TableType::MESH);
    executeAndCheckIfSQLOk(queryClearTable, TableType::MESH);
    cout << "MESH table cleared." << endl;
}

}
