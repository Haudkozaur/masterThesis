#include "DataBaseSlabPointLoadManager.h"
#include "TableTypes.h"
#include "toStringWithPrecision.h"
namespace DataBaseManagers {


DataBaseSlabPointLoadManager::DataBaseSlabPointLoadManager(std::string dateBaseName)
    : DataBaseModelObjectsManager(std::move(dateBaseName)) {

}


void DataBaseSlabPointLoadManager::addObjectToDataBase(double Fz, int x1, int z1)
{
    // Create the SQL query for inserting a slab point load
    std::string queryAddSlabPointLoad = "INSERT INTO " + tableTypesMap.at(TableType::SLAB_POINT_LOADS) +
                                        " (x1, z1, Fz) VALUES " +
                                        "(" + std::to_string(x1) + ", " + std::to_string(z1) + ", " +
                                        toStringWithPrecision(Fz) + ")";

    // Execute the query and check for success
    executeAndCheckIfSQLOk(queryAddSlabPointLoad, TableType::SLAB_POINT_LOADS);
}


void DataBaseSlabPointLoadManager::deleteObjectFromDataBase()
{

}

void DataBaseSlabPointLoadManager::editObjectInDataBase()
{

}

void DataBaseSlabPointLoadManager::iterateOverTable()
{
    std::string querySelectAllSlabPointLoads = "SELECT id, Fz, x1, z1 FROM " + tableTypesMap.at(TableType::SLAB_POINT_LOADS);
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllSlabPointLoads);

    nodalLoadsMap.clear(); // Clear the map before populating it

    for (const auto &row : results) {
        if (row.size() == 4) {
            int id = std::stoi(row[0]);
            double Fz = std::stod(row[1]);
            int x1 = std::stoi(row[2]);
            int z1 = std::stoi(row[3]);

            nodalLoadsMap[id] = std::make_tuple(Fz, x1, z1);
        }
    }

    // Print nodal loads for debugging
    for (const auto &load : nodalLoadsMap) {
        std::cout << "Load ID: " << load.first
                  << " Fz: " << std::get<0>(load.second)
                  << " X: " << std::get<1>(load.second)
                  << " Z: " << std::get<2>(load.second) << std::endl;
    }
}


std::map<int, std::tuple<double, int, int>> DataBaseSlabPointLoadManager::getNodalLoadsMap()
{
    return nodalLoadsMap;
}

}
