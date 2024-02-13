//
// Created by Julian Haudek on 11/02/2024.
//

#include "DataBaseSupportsManager.h"

DataBaseSupportsManager::DataBaseSupportsManager(const string &dateBaseName) : DataBaseModelObjectsManager(
        dateBaseName) {}

void DataBaseSupportsManager::addObjectToDataBase(int pointID, bool Rx, bool Tz, bool Tx) {
    string queryInsertSupport =
            "INSERT INTO " + tableTypesMap.at(TableType::SUPPORTS) + " (id, point_id, rx, tz, tx) VALUES (NULL, " +
            to_string(pointID) + ", " +
            to_string(Rx) + ", " + to_string(Tz) + ", " + to_string(Tx) + ")";

    if (validate(pointID, TableType::POINTS)) {
        cout << "Point exists" << endl;
        executeAndCheckIfSQLOk(queryInsertSupport, TableType::SUPPORTS);
    } else {
        cout << "Error: " << "point doesn't exist in DB" << endl;
        return;
    }

    cout << "\n";
}

void DataBaseSupportsManager::deleteObjectFromDataBase(int id) {

    string queryDeleteSupport = "DELETE FROM " + tableTypesMap.at(TableType::SUPPORTS) + " WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeleteSupport, TableType::SUPPORTS);
    cout << "\n";
}