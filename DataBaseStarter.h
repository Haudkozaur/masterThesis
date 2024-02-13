#include "sqlite/sqlite3.h"
#include <iostream>

using namespace std;

#ifndef DATEBASESTARTER_H
#define DATEBASESTARTER_H


//Class to start project model database

class DataBaseStarter {

public:
    DataBaseStarter(const string& dataBaseName);
    //Methods
    void startDateBase();

    void createPointsTable();

    void createLinesTable();

    void createSurfacesTable();

    void createMaterialsTable();

    void createCrossSectionsTable();

    void createSupportsTable();

    void createNodalLoadsTable();

    void createLineLoadsTable();

    //Variables
    string dataBaseName;
    const char *dataBaseNameAsChar;
    sqlite3 *DB;
    char *zErrMsg = 0;
};
#endif // DATEBASESTARTER_H