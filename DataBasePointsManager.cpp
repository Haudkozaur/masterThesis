#include <iostream>
#include <utility>
#include "DataBaseModelObjectsManager.h"
#include "DataBasePointsManager.h"
#include <vector>
using namespace std;


DataBasePointsManager::DataBasePointsManager(string dateBaseName) : DataBaseModelObjectsManager(
        std::move(dateBaseName)) {}

void DataBasePointsManager::addObjectToDataBase(int XCoordinate, int ZCoordinate) {

    string QueryInsertPoint = "INSERT INTO points (id, x_cord, z_cord) VALUES (NULL, " + to_string(XCoordinate) + ", " +
                              to_string(ZCoordinate) + ")";

    executeAndCheckIfSQLOk(QueryInsertPoint, TableType::POINTS);
}

void DataBasePointsManager::deleteObjectFromDataBase(int id) {

    string queryDeletePoint = "DELETE FROM points WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeletePoint, TableType::POINTS);

    string queryDeleteLine =
            "DELETE FROM lines WHERE start_point = " + to_string(id) + " OR end_point = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeleteLine, TableType::LINES);
    cout << "\n";
}
void DataBasePointsManager::iterateOverTable() {
    string querySelectAllPoints = "SELECT id, x_cord, z_cord FROM points";
    std::vector<std::vector<string>> results = executeQuery(querySelectAllPoints);

    pointsMap.clear(); // Clear the map before populating it

    for (const auto& row : results) {
        if (row.size() == 3) {
            int id = stoi(row[0]);
            int xCord = stoi(row[1]);
            int zCord = stoi(row[2]);
            pointsMap[id] = make_pair(xCord, zCord);
        }
    }

    // Print points for debugging
    for (const auto& point : pointsMap) {
        cout << "ID: " << point.first << ", X: " << point.second.first << ", Z: " << point.second.second << endl;
    }
}

std::map<int, std::pair<int, int>> DataBasePointsManager::getPointsMap() const {
    return pointsMap;
}
