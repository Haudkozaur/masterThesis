#include <iostream>
#include <utility>
#include "DataBaseModelObjectsManager.h"
#include "DataBasePointsManager.h"


DataBasePointsManager::DataBasePointsManager(string dateBaseName) : DataBaseModelObjectsManager(
        std::move(dateBaseName)) {}


void DataBasePointsManager::AddObjectToDataBase(int XCoordinate, int ZCoordinate) {

    string QueryInsertPoint = "INSERT INTO points (id, x_cord, z_cord) VALUES (NULL, " + to_string(XCoordinate) + ", " +
                              to_string(ZCoordinate) + ")";

    int rc = sqlite3_exec(this->DB, QueryInsertPoint.c_str(), nullptr, nullptr, &this->zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl << "\n";
    } else {
        cout  << "Point added successfully" << endl << "\n";
    }



}
void DataBasePointsManager::deleteObjectFromDataBase(int id) {

    string queryDeletePoint = "DELETE FROM points WHERE id = " + to_string(id);
    int rc = sqlite3_exec(this->DB, queryDeletePoint.c_str(), nullptr, nullptr, &this->zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Point deleted successfully" << endl;
    }
    string queryDeleteLine = "DELETE FROM lines WHERE start_point = " + to_string(id) + " OR end_point = " + to_string(id);
    rc = sqlite3_exec(this->DB, queryDeleteLine.c_str(), nullptr, nullptr, &this->zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Lines defined by point " + to_string(id) + " deleted successfully" << endl;
    }

    cout << "\n";
}
