#include "DataBaseCircularLinesManager.h"

namespace DataBaseManagers {
DataBaseCircularLinesManager::DataBaseCircularLinesManager(const string &dateBaseName)
    : DataBaseModelObjectsManager(dateBaseName)
{}

void DataBaseCircularLinesManager::addObjectToDataBase(int centreX, int centreZ, int diameter)
{
    std::string queryInsertCircularLine = "INSERT INTO " +tableTypesMap.at(TableType::CIRCULAR_LINES)+ " (centre_x, centre_z, diameter) VALUES ('" + std::to_string(centreX) + "', '" + std::to_string(centreZ) + "', '" + std::to_string(diameter) + "');";
    executeAndCheckIfSQLOk(queryInsertCircularLine, TableType::CIRCULAR_LINES);
}


void DataBaseCircularLinesManager::deleteObjectFromDataBase()
{

}

void DataBaseCircularLinesManager::iterateOverTable()
{
    // Define the SQL query to select all data from the circular_lines table
    std::string querySelectAllCircularLines = "SELECT id, centre_x, centre_z, diameter FROM circular_lines";

    // Execute the query and store the results
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllCircularLines);

    // Clear the circular lines map before populating it
    circularLinesMap.clear();

    // Iterate over each row in the results
    for (const auto &row : results) {
        // Ensure that each row has exactly 4 elements: id, centre_x, centre_z, and diameter
        if (row.size() == 4) {
            // Parse the row data and store it in the circularLinesMap
            int id = std::stoi(row[0]);
            int centreX = std::stoi(row[1]);
            int centreZ = std::stoi(row[2]);
            int diameter = std::stoi(row[3]);
            circularLinesMap[id] = std::make_tuple(centreX, centreZ, diameter);
        }
    }

    // Print the circular lines for debugging
    for (const auto &circularLine : circularLinesMap) {
        std::cout << "Circular Line ID: " << circularLine.first
                  << " Centre X: " << std::get<0>(circularLine.second)
                  << " Centre Z: " << std::get<1>(circularLine.second)
                  << " Diameter: " << std::get<2>(circularLine.second) << std::endl;
    }
}

std::map<int, std::tuple<int, int, int> > DataBaseCircularLinesManager::getCircularLinesMap() const
{
    return circularLinesMap;
}

}
