
#include "DataBaseMaterialsManager.h"

DataBaseMaterialsManager::DataBaseMaterialsManager(const string &dateBaseName) : DataBaseModelObjectsManager(
        dateBaseName) {
}



void DataBaseMaterialsManager::addObjectToDataBase(const string &name, double E, double v, double ro) {

    if (!checkIfNotNull(name) || !checkIfNotNull(to_string(E)) || !checkIfNotNull(to_string(v)) || !checkIfNotNull(to_string(ro))) {
        cout << "One of the values is null" << endl;
        return;
    }

    string queryInsertMaterial =
            "INSERT INTO Materials (name, E, v, ro) VALUES ('" + name + "', '" + toStringWithPrecision(E) + "', '" +
            to_string(v) +
            "', '" + toStringWithPrecision(ro) + "')";
    executeAndCheckIfSQLOk(queryInsertMaterial, TableType::MATERIALS);

}
void DataBaseMaterialsManager::iterateOverTable()
{
    string querySelectAllMaterials = "SELECT id, name FROM materials";
    std::vector<std::vector<string>> results = executeQuery(querySelectAllMaterials);

    materialsMap.clear(); // Clear the map before populating it

    for (const auto &row : results) {
        if (row.size() == 2) {
            int id = stoi(row[0]);
            string name = (row[1]);

            materialsMap[id] = name;
        }
    }

    // Print materials for debugging
    for (const auto &material : materialsMap) {
        cout << "ID: " << material.first << ", X: " << material.second
             << endl;
    }
}
std::map<int, string> DataBaseMaterialsManager::getMaterialsMap() const
{
    return materialsMap;
}

//TODO: implement deleteObjectFromDataBase after making decision what to do with cross section with material that is deleted
//void DataBaseMaterialsManager::deleteObjectFromDataBase(int id) {
//    string queryDeleteMaterial = "DELETE FROM Materials WHERE id = " + to_string(id);
//    int rc = sqlite3_exec(this->DB, queryDeleteMaterial.c_str(), nullptr, nullptr, &this->zErrMsg);
//    if (rc != SQLITE_OK) {
//        cout << "Error: " << zErrMsg << endl;
//    } else {
//        cout << "Material deleted successfully" << endl << "\n";
//    }
//}

