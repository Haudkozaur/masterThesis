#include "../sqlite/sqlite3.h"

#include <iostream>

using namespace std;

#ifndef DATEBASESTARTER_H
#define DATEBASESTARTER_H

//Class to start project model database
namespace DataBaseManagers {
class DataBaseStarter
{
public:
    DataBaseStarter(const string &dataBaseName);
    //Methods
    void startDateBase();

    void createPointsTable();

    void createLinesTable();

    void createCircularLinesTable();

    void createSurfacesTable();

    void createMaterialsTable();

    void createCrossSectionsTable();

    void createSupportsTable();

    void createNodalLoadsTable();

    void createLineLoadsTable();

    void createMeshTable();

    void createFETable();

    void createResultsTable();

    //Variables
    string dataBaseName;
    const char *dataBaseNameAsChar;
    sqlite3 *DB;
    char *zErrMsg = 0;



};
} // namespace DataBaseManagers
#endif // DATEBASESTARTER_H
