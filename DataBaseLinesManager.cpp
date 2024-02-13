#include "DataBaseLinesManager.h"


DataBaseLinesManager::DataBaseLinesManager(const string &dateBaseName) : DataBaseModelObjectsManager(dateBaseName) {
    //default cross section id
    this->defaultCrossSectionID = 1;
}

void DataBaseLinesManager::addObjectToDataBase(int startPointID, int endPointID) {

    //contamination of add line query
    string queryInsertLine =
            "INSERT INTO lines (id, start_point, end_point, cross_section_id) VALUES (NULL, " +
            to_string(startPointID) + ", " +
            to_string(endPointID) + ", " + to_string(defaultCrossSectionID) + ")";

    if (validate(startPointID, TableType::POINTS) && validate(endPointID, TableType::POINTS)) {
        cout << "Start and End points exists" << endl;

        executeAndCheckIfSQLOk(queryInsertLine, TableType::LINES);

    } else {
        cout << "Error: " << "one of the points doesn't exist in DB" << endl;
    }
    cout << "\n";
}

void DataBaseLinesManager::deleteObjectFromDataBase(int id) {
    string queryDeleteLine = "DELETE FROM lines WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeleteLine, TableType::LINES);
    cout << "\n";
    return;
}