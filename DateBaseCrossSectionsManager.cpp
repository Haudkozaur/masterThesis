
#include "DataBaseCrossSectionsManager.h"

using namespace std;

#include <iostream>
#include <sstream>

DataBaseCrossSectionsManager::DataBaseCrossSectionsManager(const string &dateBaseName) : DataBaseModelObjectsManager(
        dateBaseName) {

}

void DataBaseCrossSectionsManager::addObjectToDataBase(const string &name, int materialID, double A, double I) {

    string queryAddCrossSection =
            "INSERT INTO " + tableTypesMap.at(TableType::CROSS_SECTIONS) + " (name, material_id, A, I) VALUES ('" + name + "', " + to_string(materialID) +
            ", " + toStringWithPrecision(A) + ", " + toStringWithPrecision(I) + ");";


    if (validate(materialID, TableType::MATERIALS)) {
        cout << "Material exists" << endl;
        executeAndCheckIfSQLOk(queryAddCrossSection, TableType::CROSS_SECTIONS);
    } else {
        cout << "Error: " << "Material doesn't exist in DB" << endl;
    }
    cout << "\n";
}

