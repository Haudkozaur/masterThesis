
#include "DataBaseMaterialsManager.h"

DataBaseMaterialsManager::DataBaseMaterialsManager(const string &dateBaseName) : DataBaseModelObjectsManager(
        dateBaseName) {

}

void DataBaseMaterialsManager::addObjectToDataBase(const string &name, double E, double v, double ro) {
    string queryInsertMaterial =
            "INSERT INTO Materials (name, E, v, ro) VALUES ('" + name + "', '" + toStringWithPrecision(E) + "', '" +
            to_string(v) +
            "', '" + toStringWithPrecision(ro) + "')";
    int rc = sqlite3_exec(this->DB, queryInsertMaterial.c_str(), nullptr, nullptr, &this->zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Material " + name + " added successfully" << endl << "\n";
    }
}

void DataBaseMaterialsManager::deleteObjectFromDataBase(int id) {
    string queryDeleteMaterial = "DELETE FROM Materials WHERE id = " + to_string(id);
    int rc = sqlite3_exec(this->DB, queryDeleteMaterial.c_str(), nullptr, nullptr, &this->zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Material deleted successfully" << endl << "\n";
    }
}

