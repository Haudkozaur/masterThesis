#include <iostream>
#include <utility>
#include "DataBaseModelObjectsManager.h"
#include "DataBasePointsManager.h"


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
