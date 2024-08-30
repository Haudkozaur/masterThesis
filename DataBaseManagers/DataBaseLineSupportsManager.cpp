#include "DataBaseLineSupportsManager.h"

namespace DataBaseManagers {

DataBaseLineSupportsManager::DataBaseLineSupportsManager(const string &dateBaseName)
    : DataBaseModelObjectsManager(dateBaseName){
}

void DataBaseLineSupportsManager::addObjectToDataBase(int lineID)
{
    // Check if the ID exists in the lines table
    std::string lineExists = selectObjectPropertyByID(TableType::LINES, lineID, "id");

    // Initialize query string
    std::string queryInsertSupport;

    if (lineExists.find("Object with ID") == std::string::npos) {
        // The ID exists in the lines table, add it as a line_id
        if (checkIfDuplicate(TableType::LINE_SUPPORTS, std::make_tuple(lineID, 0,0))) {
            cout << "Error: Support for this line already exists in DB" << endl;
            return;
        }

        queryInsertSupport = "INSERT INTO " + tableTypesMap.at(TableType::LINE_SUPPORTS) +
                             " (line_id, circular_line_id) VALUES (" + std::to_string(lineID) + ", NULL);";
    } else {
        // Check if the ID exists in the circular_lines table
        std::string circularLineExists = selectObjectPropertyByID(TableType::CIRCULAR_LINES, lineID, "id");

        if (circularLineExists.find("Object with ID") == std::string::npos) {
            // The ID exists in the circular_lines table, add it as a circular_line_id
            if (checkIfDuplicate(TableType::LINE_SUPPORTS, std::make_tuple(lineID, 0,0))) {
                cout << "Error: Support for this circular line already exists in DB" << endl;
                return;
            }

            queryInsertSupport = "INSERT INTO " + tableTypesMap.at(TableType::LINE_SUPPORTS) +
                                 " (line_id, circular_line_id) VALUES (NULL, " + std::to_string(lineID) + ");";
        } else {
            // Neither line nor circular line exists with the given ID
            cout << "Error: No line or circular line found with ID: " << lineID << endl;
            return;
        }
    }

    // Execute the SQL query to insert the line support into the database
    executeAndCheckIfSQLOk(queryInsertSupport, TableType::LINE_SUPPORTS);
    cout << "Support added successfully for ID: " << lineID << endl;
}


void DataBaseLineSupportsManager::deleteObjectFromDataBase(int lineID)
{

}

void DataBaseLineSupportsManager::iterateOverTable()
{
    std::string querySelectAllSupports = "SELECT id, line_id, circular_line_id FROM " + tableTypesMap.at(TableType::LINE_SUPPORTS);
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllSupports);

    lineSupportsMap.clear(); // Clear the map before populating it

    for (const auto &row : results) {
        if (row.size() == 3) {
            int id = std::stoi(row[0]);
            int lineId = row[1].empty() ? -1 : std::stoi(row[1]);
            int circularLineId = row[2].empty() ? -1 : std::stoi(row[2]);

            lineSupportsMap[id] = std::make_tuple(lineId, circularLineId);
        }
    }

    // Print line supports for debugging
    for (const auto &support : lineSupportsMap) {
        std::cout << "Support ID: " << support.first
                  << " Line ID: " << std::get<0>(support.second)
                  << " Circular Line ID: " << std::get<1>(support.second) << std::endl;
    }
}
std::map<int, std::tuple<int, int>> DataBaseLineSupportsManager::getLineSupportsMap() const
{
    return lineSupportsMap;
}


} // namespace DataBaseManagers
