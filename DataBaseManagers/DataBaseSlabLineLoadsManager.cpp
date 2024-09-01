#include "DataBaseSlabLineLoadsManager.h"
#include "TableTypes.h"
#include "toStringWithPrecision.h"

namespace DataBaseManagers {
DataBaseSlabLineLoadsManager::DataBaseSlabLineLoadsManager(std::string dateBaseName)
    : DataBaseModelObjectsManager(std::move(dateBaseName)) {}

void DataBaseSlabLineLoadsManager::addObjectToDataBase(int x1, int z1, int x2, int z2, double F)
{
    // Create the SQL query for inserting a slab line load
    std::string queryAddSlabLineLoad = "INSERT INTO " + tableTypesMap.at(TableType::SLAB_LINE_LOADS) +
                                       " (x1, z1, x2, z2, Fz) VALUES " +
                                       "(" + std::to_string(x1) + ", " + std::to_string(z1) + ", " +
                                       std::to_string(x2) + ", " + std::to_string(z2) + ", " +
                                       toStringWithPrecision(F) + ")";

    // Execute the query and check for success
    executeAndCheckIfSQLOk(queryAddSlabLineLoad, TableType::SLAB_LINE_LOADS);
}

void DataBaseSlabLineLoadsManager::deleteObjectFromDataBase()
{

}

void DataBaseSlabLineLoadsManager::editObjectInDataBase()
{

}

void DataBaseSlabLineLoadsManager::iterateOverTable()
{
    std::string querySelectAllSlabLineLoads = "SELECT id, x1, z1, x2, z2, Fz FROM " + tableTypesMap.at(TableType::SLAB_LINE_LOADS);
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllSlabLineLoads);

    lineLoadsMap.clear(); // Clear the map before populating it

    for (const auto &row : results) {
        if (row.size() == 6) {
            int id = std::stoi(row[0]);
            int x1 = std::stoi(row[1]);
            int z1 = std::stoi(row[2]);
            int x2 = std::stoi(row[3]);
            int z2 = std::stoi(row[4]);
            double Fz = std::stod(row[5]);

            lineLoadsMap[id] = std::make_tuple(x1, z1, x2, z2, Fz);
        }
    }

    // Print line loads for debugging
    for (const auto &load : lineLoadsMap) {
        std::cout << "Load ID: " << load.first
                  << " X1: " << std::get<0>(load.second)
                  << " Z1: " << std::get<1>(load.second)
                  << " X2: " << std::get<2>(load.second)
                  << " Z2: " << std::get<3>(load.second)
                  << " Fz: " << std::get<4>(load.second) << std::endl;
    }
}


std::map<int, std::tuple<int, int, int, int, double> > DataBaseSlabLineLoadsManager::getLineLoadsMap()
{
    return lineLoadsMap;
}
}



