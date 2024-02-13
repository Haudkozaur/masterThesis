#include "DataBaseNodalLoadsManager.h"

DataBaseNodalLoadsManager::DataBaseNodalLoadsManager(const string &dateBaseName) : DataBaseModelObjectsManager(
        dateBaseName) {}

void DataBaseNodalLoadsManager::addObjectToDataBase(int pointID, double My, double Fz, double Fx) {
    string queryAddNodalLoad =
            "INSERT INTO " + tableTypesMap.at(TableType::NODAL_LOADS) + " (point_id, My, Fz, Fx) VALUES (" +
            to_string(pointID) + ", " +
            toStringWithPrecision(My) + ", " +
            toStringWithPrecision(Fz) + ", " + toStringWithPrecision(Fx) + ")";

    if (validate(pointID, TableType::POINTS)) {
        cout << "Point exists" << endl;
        executeAndCheckIfSQLOk(queryAddNodalLoad, TableType::NODAL_LOADS);
    } else {
        cout << "Error: " << "point doesn't exist in DB" << endl;
        return;
    }
    cout << "\n";
}

void DataBaseNodalLoadsManager::deleteObjectFromDataBase(int id) {
    string queryDeleteNodalLoad =
            "DELETE FROM " + tableTypesMap.at(TableType::NODAL_LOADS) + " WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeleteNodalLoad, TableType::NODAL_LOADS);
    cout << "\n";
}