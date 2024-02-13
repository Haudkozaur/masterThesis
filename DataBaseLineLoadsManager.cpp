#include "DataBaseLineLoadsManager.h"

DataBaseLineLoadsManager::DataBaseLineLoadsManager(const string &dateBaseName) : DataBaseModelObjectsManager(
        dateBaseName) {
}

void DataBaseLineLoadsManager::addObjectToDataBase(int lineID, double Fx, double Fz) {

    //contamination of add line load query
    string queryInsertLineLoad =
            "INSERT INTO " + tableTypesMap.at(TableType::LINE_LOADS) + " (id, line_id, Fx, Fz) VALUES (NULL, " +
            to_string(lineID) + ", " +
            toStringWithPrecision(Fx) + ", " +
            toStringWithPrecision(Fz) + ")";

    if (validate(lineID, TableType::LINES)) {
        cout << "Line exists" << endl;

        executeAndCheckIfSQLOk(queryInsertLineLoad, TableType::LINE_LOADS);
    } else {
        cout << "Error: " << "Line doesn't exist in DB" << endl;

    }
    cout << "\n";
}

void DataBaseLineLoadsManager::deleteObjectFromDataBase(int id) {
    string queryDeleteLineLoad =
            "DELETE FROM " + tableTypesMap.at(TableType::LINE_LOADS) + " WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeleteLineLoad, TableType::LINE_LOADS);
    cout << "\n";
}