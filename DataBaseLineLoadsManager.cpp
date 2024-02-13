//
// Created by jhaud on 13/02/2024.
//

#include "DataBaseLineLoadsManager.h"

DataBaseLineLoadsManager::DataBaseLineLoadsManager(const string &dateBaseName) : DataBaseModelObjectsManager(dateBaseName) {
}

void DataBaseLineLoadsManager::addObjectToDataBase(int lineID, double Fx, double Fz) {

    //contamination of add line load query
    string QueryInsertLineLoad =
            "INSERT INTO line_loads (id, line_id, Fx, Fz) VALUES (NULL, " +
            to_string(lineID) + ", " +
            toStringWithPrecision(Fx) + ", " +
            toStringWithPrecision(Fz) + ")";

    if (validate(lineID, TableType::LINES)) {
        cout << "Line exists" << endl;

        int addLineLoad = sqlite3_exec(this->DB, QueryInsertLineLoad.c_str(), nullptr, nullptr, &this->zErrMsg);

        if (addLineLoad != SQLITE_OK) {
            cout << "Error: " << zErrMsg << endl;
        } else {
            cout << "Line load added successfully" << endl;
        }
    } else {
        cout << "Error: " << "Line doesn't exist in DB" << endl;

    }
    cout << "\n";
}
void DataBaseLineLoadsManager::deleteObjectFromDataBase(int id) {
    string queryDeleteLineLoad = "DELETE FROM line_loads WHERE id = " + to_string(id);
    int rc = sqlite3_exec(this->DB, queryDeleteLineLoad.c_str(), nullptr, nullptr, &this->zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg <<
             endl;
    } else {
        cout << "Line load deleted successfully" <<
             endl;
    }
    cout << "\n";
}