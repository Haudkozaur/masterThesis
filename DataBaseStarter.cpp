#include <iostream>
#include "DataBaseStarter.h"
#include "TableTypes.h"

using namespace std;

DataBaseStarter::DataBaseStarter(const string &dataBaseName) {
    this->dataBaseName = dataBaseName;
    this->dataBaseNameAsChar = dataBaseName.c_str();
    sqlite3 *DB;
}

void DataBaseStarter::startDateBase() {
    sqlite3_open(dataBaseNameAsChar, &DB);
}

void DataBaseStarter::createPointsTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreatePointsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::POINTS) +
                                      " (id INTEGER PRIMARY KEY AUTOINCREMENT, x_cord INTEGER, z_cord INTEGER)";
    int rc = sqlite3_exec(DB,
                          queryToCreatePointsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Points Table created successfully" << endl;
    }
}

void DataBaseStarter::createLinesTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateLinesTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::LINES) +
                                     " (id INTEGER PRIMARY KEY AUTOINCREMENT, start_point INTEGER, end_point INTEGER, cross_section_id, length REAL, inclination_angle REAL,"
                                     "FOREIGN KEY (start_point) REFERENCES points(id),FOREIGN KEY (end_point) REFERENCES points(id), FOREIGN KEY (cross_section_id) REFERENCES cross_sections(id))";
    int rc = sqlite3_exec(DB,
                          queryToCreateLinesTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Lines Table created successfully" << endl;
    }
}

void DataBaseStarter::createSurfacesTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateSurfacesTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::SURFACES) +
                                        " (id INTEGER PRIMARY KEY AUTOINCREMENT, lines_set ARRAY INTEGER)";
    int rc = sqlite3_exec(DB,
                          queryToCreateSurfacesTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Surfaces Table created successfully" << endl;
    }

}

void DataBaseStarter::createMaterialsTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateMaterialsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::MATERIALS) +
                                         " (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, E REAL, v REAL, ro REAL)";
    int rc = sqlite3_exec(DB,
                          queryToCreateMaterialsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Materials Table created successfully" << endl;
    }
}

void DataBaseStarter::createCrossSectionsTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateCrossSectionsTable =
            "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::CROSS_SECTIONS) +
            " (id INTEGER PRIMARY KEY AUTOINCREMENT, material_id INTEGER, name TEXT, A REAL, I REAL, FOREIGN KEY (material_id) REFERENCES materials(id))";
    int rc = sqlite3_exec(DB,
                          queryToCreateCrossSectionsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Cross-Sections Table created successfully" << endl;
    }

}

void DataBaseStarter::createSupportsTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateSupportsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::SUPPORTS) +
                                        " (id INTEGER PRIMARY KEY AUTOINCREMENT, point_id INTEGER, rx BOOLEAN, tz BOOLEAN, tx BOOLEAN, FOREIGN KEY (point_id) REFERENCES points(id))";
    int rc = sqlite3_exec(DB,
                          queryToCreateSupportsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Supports Table created successfully" << endl;
    }

}

void DataBaseStarter::createNodalLoadsTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateNodalLoadsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::NODAL_LOADS) +
                                          " (id INTEGER PRIMARY KEY AUTOINCREMENT, point_id INTEGER, My REAL, Fz REAL, Fx REAL, FOREIGN KEY (point_id) REFERENCES points(id))";
    int rc = sqlite3_exec(DB,
                          queryToCreateNodalLoadsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Nodal loads Table created successfully" << endl;
    }

}

void DataBaseStarter::createLineLoadsTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateLineLoadsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::LINE_LOADS) +
                                         " (id INTEGER PRIMARY KEY AUTOINCREMENT, line_id INTEGER, Fx REAL, Fz REAL, FOREIGN KEY (line_id) REFERENCES lines(id))";
    int rc = sqlite3_exec(DB,
                          queryToCreateLineLoadsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Line loads Table created successfully" << endl;
    }
    cout << "\n";
}
