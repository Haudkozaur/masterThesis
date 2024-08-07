#include "DataBaseLinesManager.h"
#include "DataBaseModelObjectsManager.h"
#include <cmath>
#include "toStringWithPrecision.h"
namespace DataBaseManagers {
DataBaseLinesManager::DataBaseLinesManager(const string &dateBaseName)
    : DataBaseModelObjectsManager(dateBaseName)
{
    //default cross section id
    this->defaultCrossSectionID = 1;
}

void DataBaseLinesManager::addObjectToDataBase(int startPointID, int endPointID)
{
    //check if there is no duplicated lines in db
    if (checkIfDuplicate(TableType::LINES, make_tuple(startPointID, endPointID, 0))
        || checkIfDuplicate(TableType::LINES, make_tuple(endPointID, startPointID, 0))) {
        cout << "Error: "
             << "Line already exists in DB" << endl;
        return;
    }

    //contamination of add line query
    string queryInsertLine = "INSERT INTO lines (id, start_point, end_point, cross_section_id, "
                             "length, inclination_angle) VALUES (NULL, "
                             + to_string(startPointID) + ", " + to_string(endPointID) + ", "
                             + to_string(defaultCrossSectionID) + ", "
                             + toStringWithPrecision(
                                 getLineLengthFromPoints(startPointID, endPointID))
                             + ", "
                             + toStringWithPrecision(
                                 getLineInclinationAngleFromPoints(startPointID, endPointID))
                             + ")";

    if (validate(startPointID, TableType::POINTS) && validate(endPointID, TableType::POINTS)) {
        cout << "Start and End points exists" << endl;

        executeAndCheckIfSQLOk(queryInsertLine, TableType::LINES);

    } else {
        cout << "Error: "
             << "one of the points doesn't exist in DB" << endl;
    }
    cout << "\n";
}

void DataBaseLinesManager::deleteObjectFromDataBase(int id)
{
    string queryDeleteLine = "DELETE FROM lines WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeleteLine, TableType::LINES);
    cout << "\n";
    return;
}

void DataBaseLinesManager::editLine(int id, int newStartPoint, int newEndPoint)
{
    // Check if the new start and end points exist
    if (!validate(newStartPoint, TableType::POINTS) || !validate(newEndPoint, TableType::POINTS)) {
        cout << "Error: "
             << "one of the points doesn't exist in DB" << endl;
        return;
    }

    // Check if the new line already exists
    if (checkIfDuplicate(TableType::LINES, make_tuple(newStartPoint, newEndPoint, 0))) {
        cout << "Error: "
             << "Line already exists in DB" << endl;
        return;
    }

    // Edit the line
    string queryEditLine = "UPDATE lines SET start_point = " + to_string(newStartPoint)
                           + ", end_point = " + to_string(newEndPoint)
                           + " WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryEditLine, TableType::LINES);
    cout << "\n";
    return;
}
void DataBaseLinesManager::editLine(int id, int crossSectionId)
{
    //debug
    cout << "setting Cross section idas : " << crossSectionId << "for line: " << id << endl;

    if (!validate(crossSectionId, TableType::CROSS_SECTIONS)) {
        cout << "Error: "
             << "Cross section doesn't exist in DB" << endl;
        return;
    }
    if (!validate(id, TableType::LINES)) {
        cout << "Error: "
             << "Line doesn't exist in DB" << endl;
    }
    // Edit the line
    string queryEditLine = "UPDATE lines SET cross_section_id = " + to_string(crossSectionId)
                           + " WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryEditLine, TableType::LINES);
    cout << "\n";
    return;
}

double DataBaseLinesManager::getLineLengthFromPoints(int startPointID, int endPointID)
{
    double x1 = stod(selectObjectPropertyByID(TableType::POINTS, startPointID, "x_cord"));
    double z1 = stod(selectObjectPropertyByID(TableType::POINTS, startPointID, "z_cord"));
    double x2 = stod(selectObjectPropertyByID(TableType::POINTS, endPointID, "x_cord"));
    double z2 = stod(selectObjectPropertyByID(TableType::POINTS, endPointID, "z_cord"));

    return sqrt(pow(x2 - x1, 2) + pow(z2 - z1, 2));
}

double DataBaseLinesManager::getLineInclinationAngleFromPoints(int startPointID, int endPointID)
{
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
void DataBaseLinesManager::iterateOverTable()
{
    string querySelectAllLines = "SELECT id, start_point, end_point, cross_section_id FROM lines";
    vector<vector<string>> results = executeQuery(querySelectAllLines);

    linesMap.clear(); // Clear the map before populating it

    for (const auto &row : results) {
        if (row.size() == 4) {
            int id = stoi(row[0]);
            int startPoint = stoi(row[1]);
            int endPoint = stoi(row[2]);
            int crossSectionId = stoi(row[3]);
            linesMap[id] = make_tuple(startPoint, endPoint, crossSectionId);
        }
    }

    // Print lines for debugging

    for (const auto &line : linesMap) {
        cout << "Line ID: " << line.first << " Start Point: " << get<0>(line.second)
             << " End Point: " << get<1>(line.second)
             << " Cross Section ID: " << get<2>(line.second) << endl;
    }
}

map<int, tuple<int, int, int>> DataBaseLinesManager::getLinesMap() const
{
    return linesMap;
}
}
