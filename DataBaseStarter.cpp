#include <iostream>
#include "DataBaseStarter.h"

using namespace std;

DataBaseStarter::DataBaseStarter(const string& dateBaseName) {
    this->dateBaseName = dateBaseName;
    this->dateBaseNameAsChar = dateBaseName.c_str();
    sqlite3 *DB;
}

void DataBaseStarter::startDateBase() {
    sqlite3_open(dateBaseNameAsChar, &DB);
}

void DataBaseStarter::createPointsTable() {
    sqlite3_open(dateBaseNameAsChar, &DB);
    int rc = sqlite3_exec(DB,
                          "CREATE TABLE IF NOT EXISTS points (id INTEGER PRIMARY KEY AUTOINCREMENT, y_cord INTEGER, z_cord INTEGER)",
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Points Table created successfully" << endl;
    }
}
void DataBaseStarter::createLinesTable() {
    sqlite3_open(dateBaseNameAsChar, &DB);
    int rc = sqlite3_exec(DB,
                          "CREATE TABLE IF NOT EXISTS lines (id INTEGER PRIMARY KEY AUTOINCREMENT, start_point INTEGER, end_point INTEGER,FOREIGN KEY (start_point) REFERENCES points(id),FOREIGN KEY (end_point) REFERENCES points(id))",
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Lines Table created successfully" << endl;
    }
}
void DataBaseStarter::createSurfacesTable() {
    sqlite3_open(dateBaseNameAsChar, &DB);
    int rc = sqlite3_exec(DB,
                          "CREATE TABLE IF NOT EXISTS surfaces (id INTEGER PRIMARY KEY AUTOINCREMENT, lines_set ARRAY INTEGER)",
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Surfaces Table created successfully" << endl;
    }

}

void DataBaseStarter::createMaterialsTable() {
    sqlite3_open(dateBaseNameAsChar, &DB);
    int rc = sqlite3_exec(DB,
                          "CREATE TABLE IF NOT EXISTS materials (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, E REAL, v REAL, ro REAL)",
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Materials Table created successfully" << endl;
    }
}
void DataBaseStarter::createCrossSectionsTable() {
    sqlite3_open(dateBaseNameAsChar, &DB);
    int rc = sqlite3_exec(DB,
                          "CREATE TABLE IF NOT EXISTS cross_sections (id INTEGER PRIMARY KEY AUTOINCREMENT, material_id INTEGER, name TEXT, A REAL, I REAL, FOREIGN KEY (material_id) REFERENCES materials(id))",
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Cross-Sections Table created successfully" << endl;
    }
    cout << "\n";
}
