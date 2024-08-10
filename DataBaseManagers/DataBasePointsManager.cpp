#include "DataBasePointsManager.h"
#include <iostream>
#include <utility>
#include <vector>

namespace DataBaseManagers {

DataBasePointsManager::DataBasePointsManager(std::string dateBaseName)
    : DataBaseModelObjectsManager(std::move(dateBaseName)) {}

void DataBasePointsManager::addObjectToDataBase(int XCoordinate, int ZCoordinate) {
    // check if there is no duplicated points in db
    if (checkIfDuplicate(TableType::POINTS, std::make_tuple(XCoordinate, ZCoordinate, 0))) {
        std::cout << "Error: Point already exists in DB" << std::endl;
        return;
    }

    std::string QueryInsertPoint = "INSERT INTO points (id, x_cord, z_cord) VALUES (NULL, "
                                   + std::to_string(XCoordinate) + ", " + std::to_string(ZCoordinate) + ")";

    executeAndCheckIfSQLOk(QueryInsertPoint, TableType::POINTS);
}

void DataBasePointsManager::deleteObjectFromDataBase(int id) {
    std::string queryDeletePoint = "DELETE FROM points WHERE id = " + std::to_string(id);
    executeAndCheckIfSQLOk(queryDeletePoint, TableType::POINTS);

    std::string queryDeleteLine = "DELETE FROM lines WHERE start_point = " + std::to_string(id)
                                  + " OR end_point = " + std::to_string(id);
    executeAndCheckIfSQLOk(queryDeleteLine, TableType::LINES);
    std::cout << std::endl;
    std::string queryDeleteSupport = "DELETE FROM supports WHERE point_id = " + std::to_string(id);
    executeAndCheckIfSQLOk(queryDeleteSupport, TableType::SUPPORTS);
    std::string queryDeleteNodalLoad = "DELETE FROM nodal_loads WHERE point_id = " + std::to_string(id);
    executeAndCheckIfSQLOk(queryDeleteNodalLoad, TableType::NODAL_LOADS);

}

void DataBasePointsManager::iterateOverTable() {
    std::string querySelectAllPoints = "SELECT id, x_cord, z_cord FROM points";
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllPoints);

    pointsMap.clear(); // Clear the map before populating it

    for (const auto& row : results) {
        if (row.size() == 3) {
            int id = std::stoi(row[0]);
            int xCord = std::stoi(row[1]);
            int zCord = std::stoi(row[2]);
            pointsMap[id] = std::make_pair(xCord, zCord);
        }
    }

    // Print points for debugging
    for (const auto& point : pointsMap) {
        std::cout << "ID: " << point.first << ", X: " << point.second.first
                  << ", Z: " << point.second.second << std::endl;
    }
}

void DataBasePointsManager::editPoint(int id, int XCoordinate, int ZCoordinate) {
    // Check if the new point already exists
    if (checkIfDuplicate(TableType::POINTS, std::make_tuple(XCoordinate, ZCoordinate, 0))) {
        std::cout << "Error: Point already exists in DB" << std::endl;
        return;
    }

    std::string queryEditPoint = "UPDATE points SET x_cord = " + std::to_string(XCoordinate)
                                 + ", z_cord = " + std::to_string(ZCoordinate)
                                 + " WHERE id = " + std::to_string(id);
    executeAndCheckIfSQLOk(queryEditPoint, TableType::POINTS);
}

std::map<int, std::pair<int, int>> DataBasePointsManager::getPointsMap() const {
    return pointsMap;
}

} // namespace DataBaseManagers
