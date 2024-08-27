#include "DataBaseSurfacesManager.h"

namespace DataBaseManagers {

// Constructor implementation
DataBaseSurfacesManager::DataBaseSurfacesManager(const string &dateBaseName)
    : DataBaseModelObjectsManager(dateBaseName) {
    // Constructor body (if needed)
}

void DataBaseSurfacesManager::addObjectToDataBase(int line1_id, int line2_id, int line3_id, int line4_id, int material_id, int thickness, bool is_opening) {

    // Create the SQL query for inserting a rectangular surface
    std::string query = "INSERT INTO " + tableTypesMap.at(TableType::SURFACES) +
                        "(surface_type, line1_id, line2_id, line3_id, line4_id, circular_line_id, material_id, thickness, is_opening) VALUES "
                        "('rectangle', " + std::to_string(line1_id) + ", " + std::to_string(line2_id) + ", " + std::to_string(line3_id) + ", " + std::to_string(line4_id) + ", NULL, " + std::to_string(material_id) + ", " + std::to_string(thickness) + ", " + (is_opening ? "1" : "0") + ")";

    // Execute the query
    executeAndCheckIfSQLOk(query, TableType::SURFACES);
}

void DataBaseSurfacesManager::addObjectToDataBase(int circular_line_id, int material_id, int thickness, bool is_opening) {

    // Create the SQL query for inserting a circular surface
    std::string query = "INSERT INTO " + tableTypesMap.at(TableType::SURFACES) +
                        "(surface_type, line1_id, line2_id, line3_id, line4_id, circular_line_id, material_id, thickness, is_opening) VALUES "
                        "('circle', NULL, NULL, NULL, NULL, " + std::to_string(circular_line_id) + ", " + std::to_string(material_id) + ", " + std::to_string(thickness) + ", " + (is_opening ? "1" : "0") + ")";

    // Execute the query
    executeAndCheckIfSQLOk(query, TableType::SURFACES);
}

void DataBaseSurfacesManager::addObjectToDataBase(int line1_id, int line2_id, int line3_id, int material_id, int thickness, bool is_opening) {

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
    // Define the SQL query to select all data from the surfaces table
    std::string querySelectAllSurfaces = "SELECT id, surface_type, line1_id, line2_id, line3_id, line4_id, circular_line_id, material_id, thickness, is_opening FROM " + tableTypesMap.at(TableType::SURFACES);

    // Execute the query and store the results
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllSurfaces);

    // Clear the surfaces map before populating it
    surfacesMap.clear();

    // Iterate over each row in the results
    for (const auto &row : results) {
        // Ensure that each row has exactly 10 elements as expected
        if (row.size() == 10) {
            // Parse the row data, checking for NULL values
            int id = std::stoi(row[0]);
            std::string surfaceType = row[1];
            int line1Id = row[2].empty() ? -1 : std::stoi(row[2]); // Default to -1 if NULL
            int line2Id = row[3].empty() ? -1 : std::stoi(row[3]);
            int line3Id = row[4].empty() ? -1 : std::stoi(row[4]);
            int line4Id = row[5].empty() ? -1 : std::stoi(row[5]);
            int circularLineId = row[6].empty() ? -1 : std::stoi(row[6]);
            int materialId = row[7].empty() ? -1 : std::stoi(row[7]);
            int thickness = row[8].empty() ? -1 : std::stoi(row[8]);
            bool isOpening = row[9] == "1";

            surfacesMap[id] = std::make_tuple(surfaceType, line1Id, line2Id, line3Id, line4Id, circularLineId, materialId, thickness, isOpening);
        }
    }

    // Print the surfaces for debugging
    for (const auto &surface : surfacesMap) {
        std::cout << "Surface ID: " << surface.first
                  << " Surface Type: " << std::get<0>(surface.second)
                  << " Line1 ID: " << std::get<1>(surface.second)
                  << " Line2 ID: " << std::get<2>(surface.second)
                  << " Line3 ID: " << std::get<3>(surface.second)
                  << " Line4 ID: " << std::get<4>(surface.second)
                  << " Circular Line ID: " << std::get<5>(surface.second)
                  << " Material ID: " << std::get<6>(surface.second)
                  << " Thickness: " << std::get<7>(surface.second)
                  << " Is Opening: " << (std::get<8>(surface.second) ? "True" : "False") << std::endl;
    }
}


bool DataBaseSurfacesManager::hasNonOpeningSurface() {
    std::string query = "SELECT COUNT(*) FROM " + tableTypesMap.at(TableType::SURFACES) + " WHERE is_opening = 0";

    std::vector<std::vector<std::string>> results = executeQuery(query);

    if (!results.empty() && !results[0].empty()) {
        int count = std::stoi(results[0][0]);
        return count > 0;
    } else {
        std::printf("ERROR: Failed to execute query or no data returned.\n");
        return false;
    }
}

std::tuple<std::string, int, int, int, int, int, int, int, bool> DataBaseSurfacesManager::getMainSurface()
{
    std::string query = "SELECT surface_type, line1_id, line2_id, line3_id, line4_id, circular_line_id, material_id, thickness, is_opening FROM "
                        + tableTypesMap.at(TableType::SURFACES)
                        + " WHERE is_opening = 0 LIMIT 1";

    std::vector<std::vector<std::string>> results = executeQuery(query);

    if (!results.empty() && results[0].size() == 9) {
        std::string surfaceType = results[0][0];
        int line1Id = results[0][1].empty() ? -1 : std::stoi(results[0][1]);
        int line2Id = results[0][2].empty() ? -1 : std::stoi(results[0][2]);
        int line3Id = results[0][3].empty() ? -1 : std::stoi(results[0][3]);
        int line4Id = results[0][4].empty() ? -1 : std::stoi(results[0][4]);
        int circularLineId = results[0][5].empty() ? -1 : std::stoi(results[0][5]);
        int materialId = results[0][6].empty() ? -1 : std::stoi(results[0][6]);
        int thickness = results[0][7].empty() ? -1 : std::stoi(results[0][7]);
        bool isOpening = results[0][8] == "1";

        return std::make_tuple(surfaceType, line1Id, line2Id, line3Id, line4Id, circularLineId, materialId, thickness, isOpening);
    }

    throw std::runtime_error("No main surface found.");
}




std::map<int, std::tuple<std::string, int, int, int, int, int, int, int, bool>> DataBaseSurfacesManager::getSurfacesMap() const
{
    return surfacesMap;
}



} // namespace DataBaseManagers
