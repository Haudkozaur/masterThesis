#include "DataBaseSurfacesManager.h"

namespace DataBaseManagers {

// Constructor implementation
DataBaseSurfacesManager::DataBaseSurfacesManager(const string &dateBaseName)
    : DataBaseModelObjectsManager(dateBaseName) {
    // Constructor body (if needed)
}

void DataBaseSurfacesManager::AddObjectToDataBase(int line1_id, int line2_id, int line3_id, int line4_id, int material_id, int thickness, bool is_opening) {

    // Create the SQL query for inserting a rectangular surface
    std::string query = "INSERT INTO " + tableTypesMap.at(TableType::SURFACES) +
                        "(surface_type, line1_id, line2_id, line3_id, line4_id, circular_line_id, material_id, thickness, is_opening) VALUES "
                        "('rectangle', " + std::to_string(line1_id) + ", " + std::to_string(line2_id) + ", " + std::to_string(line3_id) + ", " + std::to_string(line4_id) + ", NULL, " + std::to_string(material_id) + ", " + std::to_string(thickness) + ", " + (is_opening ? "1" : "0") + ")";

    // Execute the query
    executeAndCheckIfSQLOk(query, TableType::SURFACES);
}

void DataBaseSurfacesManager::AddObjectToDataBase(int circular_line_id, int material_id, int thickness, bool is_opening) {

    // Create the SQL query for inserting a circular surface
    std::string query = "INSERT INTO " + tableTypesMap.at(TableType::SURFACES) +
                        "(surface_type, line1_id, line2_id, line3_id, line4_id, circular_line_id, material_id, thickness, is_opening) VALUES "
                        "('circle', NULL, NULL, NULL, NULL, " + std::to_string(circular_line_id) + ", " + std::to_string(material_id) + ", " + std::to_string(thickness) + ", " + (is_opening ? "1" : "0") + ")";

    // Execute the query
    executeAndCheckIfSQLOk(query, TableType::SURFACES);
}

void DataBaseSurfacesManager::AddObjectToDataBase(int line1_id, int line2_id, int line3_id, int material_id, int thickness, bool is_opening) {

    // Create the SQL query for inserting a triangular surface
    std::string query = "INSERT INTO " + tableTypesMap.at(TableType::SURFACES) +
                        "(surface_type, line1_id, line2_id, line3_id, line4_id, circular_line_id, material_id, thickness, is_opening) VALUES "
                        "('triangle', " + std::to_string(line1_id) + ", " + std::to_string(line2_id) + ", " + std::to_string(line3_id) + ", NULL, NULL, " + std::to_string(material_id) + ", " + std::to_string(thickness) + ", " + (is_opening ? "1" : "0") + ")";

    // Execute the query
    executeAndCheckIfSQLOk(query, TableType::SURFACES);
}


void DataBaseSurfacesManager::deleteObjectFromDataBase(int id)
{

}

void DataBaseSurfacesManager::iterateOverTable()
{

}

std::map<int, std::tuple<int, int> > DataBaseSurfacesManager::getSurfacesMap() const
{

}



} // namespace DataBaseManagers
