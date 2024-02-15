#include <cmath>
#include "DataBaseLinesManager.h"


DataBaseLinesManager::DataBaseLinesManager(const string &dateBaseName) : DataBaseModelObjectsManager(dateBaseName) {
    //default cross section id
    this->defaultCrossSectionID = 1;
}

void DataBaseLinesManager::addObjectToDataBase(int startPointID, int endPointID) {

    //contamination of add line query
    string queryInsertLine =
            "INSERT INTO lines (id, start_point, end_point, cross_section_id, length, inclination_angle) VALUES (NULL, " +
            to_string(startPointID) + ", " +
            to_string(endPointID) + ", " + to_string(defaultCrossSectionID) + ", " +
            toStringWithPrecision(getLineLengthFromPoints(startPointID, endPointID)) + ", " +
            toStringWithPrecision(getLineInclinationAngleFromPoints(startPointID, endPointID)) + ")";

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

double DataBaseLinesManager::getLineLengthFromPoints(int startPointID, int endPointID) {
    double x1 = stod(selectObjectPropertyByID(TableType::POINTS, startPointID, "x_cord"));
    double z1 = stod(selectObjectPropertyByID(TableType::POINTS, startPointID, "z_cord"));
    double x2 = stod(selectObjectPropertyByID(TableType::POINTS, endPointID, "x_cord"));
    double z2 = stod(selectObjectPropertyByID(TableType::POINTS, endPointID, "z_cord"));

    return sqrt(pow(x2 - x1, 2) + pow(z2 - z1, 2));

}

double DataBaseLinesManager::getLineInclinationAngleFromPoints(int startPointID, int endPointID) {
    double x1 = stod(selectObjectPropertyByID(TableType::POINTS, startPointID, "x_cord"));
    double z1 = stod(selectObjectPropertyByID(TableType::POINTS, startPointID, "z_cord"));
    double x2 = stod(selectObjectPropertyByID(TableType::POINTS, endPointID, "x_cord"));
    double z2 = stod(selectObjectPropertyByID(TableType::POINTS, endPointID, "z_cord"));

    double angle = atan((z2 - z1) / (x2 - x1)) * 180 / M_PI;

    if (angle < 0) {
        angle = angle + 180;
    }
    return angle;
}