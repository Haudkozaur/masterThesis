#include "DataBaseSlabMeshManager.h"
#include "TableTypes.h"

namespace DataBaseManagers {

DataBaseSlabMeshManager::DataBaseSlabMeshManager(std::string dateBaseName)
    : DataBaseModelObjectsManager(std::move(dateBaseName)) {


}

void DataBaseSlabMeshManager::addObjectToDataBase(int lineID, int numberOfFE)
{
    std::string queryAddMesh = "INSERT INTO " + tableTypesMap.at(TableType::SLAB_MESH) +
                               " (line_id, number_of_FE) VALUES " +
                               "(" + std::to_string(lineID) + ", " + std::to_string(numberOfFE) + ")";
    executeAndCheckIfSQLOk(queryAddMesh, TableType::SLAB_MESH);
}

void DataBaseSlabMeshManager::deleteObjectFromDataBase()
{

}

void DataBaseSlabMeshManager::editObjectInDataBase()
{

}

void DataBaseSlabMeshManager::iterateOverTable()
{
    std::string querySelectAllMesh = "SELECT id, line_id, number_of_FE FROM " + tableTypesMap.at(TableType::SLAB_MESH);
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllMesh);

    meshMap.clear(); // Clear the map before populating it

    for (const auto& row : results) {
        if (row.size() == 3) {
            int id = std::stoi(row[0]);
            int lineID = std::stoi(row[1]);
            int numberOfFE = std::stoi(row[2]);

            meshMap[id] = std::make_tuple(lineID, numberOfFE);
        }
    }
    //print meshmap for debugging
    for (const auto& mesh : meshMap) {
        std::cout << "ID: " << mesh.first << ", Line ID: " << std::get<0>(mesh.second) << ", Number of FE: " << std::get<1>(mesh.second) << std::endl;


    }
}

void DataBaseSlabMeshManager::clearTable()
{
    string queryClearTable = "DELETE FROM " + tableTypesMap.at(TableType::SLAB_MESH);
    executeAndCheckIfSQLOk(queryClearTable, TableType::SLAB_MESH);
    cout << "SLAB_MESH table cleared." << endl;
}


std::map<int, std::tuple<int, int>> DataBaseSlabMeshManager::getMeshMap() const
{
    return meshMap;
}


}
