#include "DataBaseLinesManager.h"

DataBaseLinesManager::DataBaseLinesManager(string dateBaseName) : DataBaseModelObjectsManager(dateBaseName) {
    this->dateBaseName = dateBaseName;
}

void DataBaseLinesManager::AddObjectToDataBase(int startPointID, int endPointID) {

    string QueryInsertLine =
            "INSERT INTO lines (id, start_point, end_point) VALUES (NULL, " + to_string(startPointID) + ", " +
            to_string(endPointID) + ")";



    //check if point with whis id exists
    string QueryCheckStartPoint = "SELECT * FROM points WHERE id = " + to_string(startPointID);
    string QueryCheckEndPoint = "SELECT * FROM points WHERE id = " + to_string(endPointID);
    int checkStartPoint = sqlite3_exec(this->DB, QueryCheckStartPoint.c_str(), nullptr, nullptr, &this->zErrMsg);
    int checkEndPoint = sqlite3_exec(this->DB, QueryCheckEndPoint.c_str(), nullptr, nullptr, &this->zErrMsg);


    if (checkStartPoint == SQLITE_OK && checkEndPoint == SQLITE_OK) {
        cout << "Start and end points exists" << endl;
        int addLine = sqlite3_exec(this->DB, QueryInsertLine.c_str(), nullptr, nullptr, &this->zErrMsg);
        if (addLine != SQLITE_OK) {
            cout << "Error: " << zErrMsg << endl;
        } else {
            cout << "Line added successfully" << endl;
        }
        return;
    } else {
        cout << "Error: " << zErrMsg << endl;
        return;
    }
}