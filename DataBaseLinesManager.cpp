#include "DataBaseLinesManager.h"

DataBaseLinesManager::DataBaseLinesManager(string dateBaseName) : DataBaseModelObjectsManager(dateBaseName) {
    this->dateBaseName = dateBaseName;
}
void DataBaseLinesManager::AddObjectToDataBase(int startPointID, int endPointID) {



    string QueryInsertLine = "INSERT INTO lines (id, start_point, end_point) VALUES (NULL, " + to_string(startPointID) + ", " + to_string(endPointID) + ")";

    int rc = sqlite3_exec(this->DB, QueryInsertLine.c_str(), nullptr, nullptr, &this->zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Line added successfully" << endl;
    }
}