#include "sqlite/sqlite3.h"
#include <iostream>

using namespace std;

#ifndef DATEBASESTARTER_H
#define DATEBASESTARTER_H


//Class to start project model database

class DateBaseStarter {

public:
    DateBaseStarter(string dateBaseName);
    //Methods
    void startDateBase();

    void createPointsTable();

    void createLinesTable();

    void createSurfacesTable();

    //Variables
    string dateBaseName;
    const char *dateBaseNameAsChar;
    sqlite3 *DB;
    char *zErrMsg = 0;
};
#endif // DATEBASESTARTER_H