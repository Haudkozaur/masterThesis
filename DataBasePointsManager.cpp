#include <iostream>
#include <utility>
#include "DataBaseModelObjectsManager.h"
#include "DataBasePointsManager.h"


DataBasePointsManager::DataBasePointsManager(string dateBaseName) : DataBaseModelObjectsManager(
        std::move(dateBaseName)) {}


void DataBasePointsManager::AddObjectToDataBase(int YCoordinate, int ZCoordinate) {
    string QueryInsertPoint = "INSERT INTO points (id, y_cord, z_cord) VALUES (NULL, " + to_string(YCoordinate) + ", " +
                              to_string(ZCoordinate) + ")";

    int rc = sqlite3_exec(this->DB, QueryInsertPoint.c_str(), nullptr, nullptr, &this->zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl << "\n";
    } else {
        cout  << "Point added successfully" << endl << "\n";
    }
}

