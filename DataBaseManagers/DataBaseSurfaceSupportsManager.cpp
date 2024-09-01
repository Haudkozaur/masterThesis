#include "DataBaseSurfaceSupportsManager.h"
#include "TableTypes.h"

namespace DataBaseManagers {

DataBaseSurfaceSupportsManager::DataBaseSurfaceSupportsManager(const string &dateBaseName)
    : DataBaseModelObjectsManager(dateBaseName) {
    // Constructor body (if needed)
}
void DataBaseSurfaceSupportsManager::addObjectToDataBase(int x1, int z1, int x2, int z2)
{
    // Create the SQL query for inserting a rectangular surface
    std::string queryAddSurfaceSupport = "INSERT INTO " + tableTypesMap.at(TableType::SURFACE_SUPPORTS) +
                                         "(x1, z1, x2, z2) VALUES " +
                                         "(" + std::to_string(x1) + ", " + std::to_string(z1) + ", " + std::to_string(x2) + ", " + std::to_string(z2) + ")";

    // Execute the query
    executeAndCheckIfSQLOk(queryAddSurfaceSupport, TableType::SURFACE_SUPPORTS);
}


void DataBaseSurfaceSupportsManager::deleteObjectFromDataBase()
{

}

void DataBaseSurfaceSupportsManager::iterateOverTable()
{
    std::string querySelectAllSupports = "SELECT id, x1, z1, x2, z2 FROM " + tableTypesMap.at(TableType::SURFACE_SUPPORTS);
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllSupports);

    surfaceSupportsMap.clear(); // Clear the map before populating it

    for (const auto &row : results) {
        if (row.size() == 5) {
            int id = std::stoi(row[0]);
            int x1 = std::stoi(row[1]);
            int z1 = std::stoi(row[2]);
            int x2 = std::stoi(row[3]);
            int z2 = std::stoi(row[4]);

            surfaceSupportsMap[id] = std::make_tuple(x1, z1, x2, z2);
        }
    }

    // Print surface supports for debugging
    for (const auto &support : surfaceSupportsMap) {
        std::cout << "Support ID: " << support.first
                  << " X1: " << std::get<0>(support.second)
                  << " Z1: " << std::get<1>(support.second)
                  << " X2: " << std::get<2>(support.second)
                  << " Z2: " << std::get<3>(support.second) << std::endl;
    }
}

std::map<int, std::tuple<int, int, int, int>> DataBaseSurfaceSupportsManager::getSurfaceSupportsMap() const
{
    return surfaceSupportsMap;
}

} // namespace DataBaseManagers
