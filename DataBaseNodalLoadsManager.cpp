#include "DataBaseNodalLoadsManager.h"

DataBaseNodalLoadsManager::DataBaseNodalLoadsManager(const string &dateBaseName) : DataBaseModelObjectsManager(
        dateBaseName) {}

void DataBaseNodalLoadsManager::addObjectToDataBase(int pointID, double My, double Fz, double Fx) {
    string queryAddNodalLoad =
            "INSERT INTO nodal_loads (point_id, My, Fz, Fx) VALUES (" + to_string(pointID) + ", " + toStringWithPrecision(My) + ", " +
            toStringWithPrecision(Fz) + ", " + toStringWithPrecision(Fx) + ")";

    if (validate(pointID, TableType::POINTS)) {
        cout << "Point exists" << endl;
        int rc = sqlite3_exec(this->DB, queryAddNodalLoad.c_str(), nullptr, nullptr, &this->zErrMsg);
        if (rc != SQLITE_OK) {
            cout << "Error: " << zErrMsg << endl;
        } else {
            cout << "Nodal Load added successfully" << endl;
        }
    } else {
        cout << "Error: " << "point doesn't exist in DB" << endl;
        return;
    }
    cout << "\n";
}

void DataBaseNodalLoadsManager::deleteObjectFromDataBase(int id) {
    string queryDeleteNodalLoad = "DELETE FROM nodal_loads WHERE id = " + to_string(id);
    int rc = sqlite3_exec(this->DB, queryDeleteNodalLoad.c_str(), nullptr, nullptr, &this->zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Nodal Load deleted successfully" << endl;
    }
    cout << "\n";
}