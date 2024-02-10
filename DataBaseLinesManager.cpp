#include "DataBaseLinesManager.h"


DataBaseLinesManager::DataBaseLinesManager(const string &dateBaseName) : DataBaseModelObjectsManager(dateBaseName) {

}

void DataBaseLinesManager::addObjectToDataBase(int startPointID, int endPointID) {

    //contamination of add line query
    string QueryInsertLine =
            "INSERT INTO lines (id, start_point, end_point) VALUES (NULL, " + to_string(startPointID) + ", " +
            to_string(endPointID) + ")";

    if (validate(startPointID, TableType::POINTS) && validate(endPointID, TableType::POINTS)) {
        cout << "Start and end points exists" <<
             endl;
        int addLine = sqlite3_exec(this->DB, QueryInsertLine.c_str(), nullptr, nullptr, &this->zErrMsg);
        if (addLine != SQLITE_OK) {
            cout << "Error: " << zErrMsg <<
                 endl;
        } else {
            cout << "Line added successfully" <<
                 endl;
        }

    } else {
        cout << "Error: " << "one of the points doesn't exist in DB" <<
             endl;

    }
    cout << "\n";

    return;
}

void DataBaseLinesManager::deleteObjectFromDataBase(int id) {
    string queryDeleteLine = "DELETE FROM lines WHERE id = " + to_string(id);
    int rc = sqlite3_exec(this->DB, queryDeleteLine.c_str(), nullptr, nullptr, &this->zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg <<
             endl;
    } else {
        cout << "Line deleted successfully" <<
             endl;
    }
    cout << "\n";
    return;
}