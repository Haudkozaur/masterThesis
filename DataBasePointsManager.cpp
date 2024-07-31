#include "DataBasePointsManager.h"
#include "DataBaseModelObjectsManager.h"
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

DataBasePointsManager::DataBasePointsManager(string dateBaseName)
    : DataBaseModelObjectsManager(std::move(dateBaseName))
{}

void DataBasePointsManager::addObjectToDataBase(int XCoordinate, int ZCoordinate)
{
    //check if there is no duplicated points in db
    if (checkIfDuplicate(TableType::POINTS, make_tuple(XCoordinate, ZCoordinate, 0))) {
        cout << "Error: "
             << "Point already exists in DB" << endl;
        return;
    }

    string QueryInsertPoint = "INSERT INTO points (id, x_cord, z_cord) VALUES (NULL, "
                              + to_string(XCoordinate) + ", " + to_string(ZCoordinate) + ")";

    executeAndCheckIfSQLOk(QueryInsertPoint, TableType::POINTS);
}

void DataBasePointsManager::deleteObjectFromDataBase(int id)
{
    string queryDeletePoint = "DELETE FROM points WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeletePoint, TableType::POINTS);

    string queryDeleteLine = "DELETE FROM lines WHERE start_point = " + to_string(id)
                             + " OR end_point = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeleteLine, TableType::LINES);
    cout << "\n";
    string queryDeleteSupport = "DELETE FROM supports WHERE point_id = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeleteSupport, TableType::SUPPORTS);
    string queryDeleteNodalLoad = "DELETE FROM nodal_loads WHERE point_id = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeleteNodalLoad, TableType::NODAL_LOADS);
}
void DataBasePointsManager::iterateOverTable()
{
    string querySelectAllPoints = "SELECT id, x_cord, z_cord FROM points";
    std::vector<std::vector<string>> results = executeQuery(querySelectAllPoints);

    pointsMap.clear(); // Clear the map before populating it

    for (const auto &row : results) {
        if (row.size() == 3) {
            int id = stoi(row[0]);
            int xCord = stoi(row[1]);
            int zCord = stoi(row[2]);
            pointsMap[id] = make_pair(xCord, zCord);
        }
    }

    // Print points for debugging
    for (const auto &point : pointsMap) {
        cout << "ID: " << point.first << ", X: " << point.second.first
             << ", Z: " << point.second.second << endl;
    }
}

void DataBasePointsManager::editPoint(int id, int XCoordinate, int ZCoordinate) {

    // Check if the new point already exists
    if (checkIfDuplicate(TableType::POINTS, make_tuple(XCoordinate, ZCoordinate, 0))) {
        cout << "Error: "
             << "Point already exists in DB" << endl;
        return;
    }

    string queryEditPoint = "UPDATE points SET x_cord = " + to_string(XCoordinate) + ", z_cord = " + to_string(ZCoordinate) + " WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryEditPoint, TableType::POINTS);
    return;
}

std::map<int, std::pair<int, int>> DataBasePointsManager::getPointsMap() const
{
    return pointsMap;
}
