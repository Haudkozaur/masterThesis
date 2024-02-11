#include "DataBaseLinesManager.h"


DataBaseLinesManager::DataBaseLinesManager(const string &dateBaseName) : DataBaseModelObjectsManager(dateBaseName) {
    //default cross section id
    this->defaultCrossSectionID = 1;
}

void DataBaseLinesManager::addObjectToDataBase(int startPointID, int endPointID) {

    //contamination of add line query
    string QueryInsertLine =
            "INSERT INTO lines (id, start_point, end_point, cross_section_id) VALUES (NULL, " +
            to_string(startPointID) + ", " +
            to_string(endPointID) + ", " + to_string(defaultCrossSectionID) + ")";

    if (validate(startPointID, TableType::POINTS) && validate(endPointID, TableType::POINTS)) {
        cout << "Start and End points exists" << endl;

        int addLine = sqlite3_exec(this->DB, QueryInsertLine.c_str(), nullptr, nullptr, &this->zErrMsg);

        if (addLine != SQLITE_OK) {
            cout << "Error: " << zErrMsg << endl;
        } else {
            cout << "Line added successfully" << endl;
        }
    } else {
        cout << "Error: " << "one of the points doesn't exist in DB" << endl;

    }
    cout << "\n";
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