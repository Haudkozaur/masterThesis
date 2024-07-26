//
// Created by Julian Haudek on 11/02/2024.
//

#include "DataBaseSupportsManager.h"

DataBaseSupportsManager::DataBaseSupportsManager(const string &dateBaseName) : DataBaseModelObjectsManager(
        dateBaseName) {}

void DataBaseSupportsManager::addObjectToDataBase(int pointID, bool Ry, bool Tz, bool Tx) {

    //check if there is no duplicated supports in db
    if (checkIfDuplicate(TableType::SUPPORTS, make_tuple(pointID, 0, 0))) {
        cout << "Error: " << "Support for this point already exists in DB" << endl;
        return;
    }
    string queryInsertSupport =
            "INSERT INTO " + tableTypesMap.at(TableType::SUPPORTS) + " (id, point_id, ry, tz, tx) VALUES (NULL, " +
            to_string(pointID) + ", " +
            to_string(Ry) + ", " + to_string(Tz) + ", " + to_string(Tx) + ")";

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

    //delete support based on point to which it is connected
    string queryDeleteSupportBasedOnPoint = "DELETE FROM " + tableTypesMap.at(TableType::SUPPORTS) + " WHERE point_id = " + to_string(id);

    //string queryDeleteSupport = "DELETE FROM " + tableTypesMap.at(TableType::SUPPORTS) + " WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeleteSupportBasedOnPoint, TableType::SUPPORTS);
    cout << "\n";
}

void DataBaseSupportsManager::iterateOverTable() {
    string querySelectAllSupports = "SELECT id, point_id, ry, tz, tx FROM " + tableTypesMap.at(TableType::SUPPORTS);
    std::vector<std::vector<string>> results = executeQuery(querySelectAllSupports);

    supportsMap.clear(); // Clear the map before populating it

    // Populate the map with the results
    for (const auto& row : results) {
        if (row.size() == 5) {
            int id = stoi(row[0]);
            int pointID = stoi(row[1]);
            bool ry = stoi(row[2]);
            bool tz = stoi(row[3]);
            bool tx = stoi(row[4]);
            supportsMap[id] = make_tuple(pointID, ry, tz, tx);
        }
    }

    //Print supports for debugging
    for (const auto& support : supportsMap) {
        cout << "ID: " << support.first << ", Point ID: " << get<0>(support.second) << ", Ry: " << get<1>(support.second) << ", Tz: " << get<2>(support.second) << ", Tx: " << get<3>(support.second) << endl;
    }

}
map<int, tuple<int, bool, bool, bool>> DataBaseSupportsManager::getSupportsMap() const {
    return supportsMap;
}
