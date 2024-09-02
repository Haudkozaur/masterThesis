#include "DataBaseSurfaceLoadsManager.h"

namespace DataBaseManagers {
DataBaseSurfaceLoadsManager::DataBaseSurfaceLoadsManager(std::string dateBaseName) : DataBaseModelObjectsManager(dateBaseName) {
    // Constructor body (if needed)
}

void DataBaseManagers::DataBaseSurfaceLoadsManager::addObjectToDataBase(int x1, int z1, int x2, int z2, double F)
{
    // Create the SQL query for inserting a rectangular surface
    std::string queryAddSurfaceLoad = "INSERT INTO " + tableTypesMap.at(TableType::SURFACE_LOADS) +
                                      "(x1, z1, x2, z2, F) VALUES " +
                                      "(" + std::to_string(x1) + ", " + std::to_string(z1) + ", " + std::to_string(x2) + ", " + std::to_string(z2) + ", " + std::to_string(F) + ")";

    // Execute the query
    executeAndCheckIfSQLOk(queryAddSurfaceLoad, TableType::SURFACE_LOADS);
}


void DataBaseManagers::DataBaseSurfaceLoadsManager::deleteObjectFromDataBase()
{

}

void DataBaseManagers::DataBaseSurfaceLoadsManager::editObjectInDataBase()
{

}

void DataBaseManagers::DataBaseSurfaceLoadsManager::iterateOverTable()
{
    std::string querySelectAllLoads = "SELECT id, x1, z1, x2, z2, F FROM " + tableTypesMap.at(TableType::SURFACE_LOADS);
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllLoads);

    surfaceLoadsMap.clear(); // Clear the map before populating it

    for (const auto &row : results) {
        if (row.size() == 6) {
            int id = std::stoi(row[0]);
            int x1 = std::stoi(row[1]);
            int z1 = std::stoi(row[2]);
            int x2 = std::stoi(row[3]);
            int z2 = std::stoi(row[4]);
            double F = std::stod(row[5]);

            surfaceLoadsMap[id] = std::make_tuple(x1, z1, x2, z2, F);
        }
    }

    // Print surface loads for debugging
    for (const auto &load : surfaceLoadsMap) {
        std::cout << "Load ID: " << load.first
                  << " X1: " << std::get<0>(load.second)
                  << " Z1: " << std::get<1>(load.second)
                  << " X2: " << std::get<2>(load.second)
                  << " Z2: " << std::get<3>(load.second)
                  << " F: " << std::get<4>(load.second) << std::endl;
    }
}

std::map<int, std::tuple<int, int, int, int, double> > DataBaseManagers::DataBaseSurfaceLoadsManager::getSurfaceLoadsMap()
{
    return surfaceLoadsMap;
}

}
