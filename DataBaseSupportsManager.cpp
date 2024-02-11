//
// Created by Julian Haudek on 11/02/2024.
//

#include "DataBaseSupportsManager.h"

DataBaseSupportsManager::DataBaseSupportsManager(const string &dateBaseName) : DataBaseModelObjectsManager(dateBaseName) {}

void DataBaseSupportsManager::addObjectToDataBase(int pointID, bool Rx, bool Tz, bool Tx) {
    string queryInsertSupport = "INSERT INTO supports (id, point_id, rx, tz, tx) VALUES (NULL, " + to_string(pointID) + ", " +
                                to_string(Rx) + ", " + to_string(Tz) + ", " + to_string(Tx) + ")";

    if (validate(pointID, TableType::POINTS)) {
        cout << "Point exists" << endl;
        int rc = sqlite3_exec(this->DB, queryInsertSupport.c_str(), nullptr, nullptr, &this->zErrMsg);
        if (rc != SQLITE_OK) {
            cout << "Error: " << zErrMsg << endl;
        } else {
            cout << "Support added successfully" << endl;
        }
    } else {
        cout << "Error: " << "point doesn't exist in DB" << endl;
        return;
    }

    cout << "\n";
}
void DataBaseSupportsManager::deleteObjectFromDataBase(int id) {
    string queryDeleteSupport = "DELETE FROM supports WHERE id = " + to_string(id);
    int rc = sqlite3_exec(this->DB, queryDeleteSupport.c_str(), nullptr, nullptr, &this->zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Support deleted successfully" << endl;
    }
    cout << "\n";
}