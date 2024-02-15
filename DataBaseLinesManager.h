#include "DataBaseModelObjectsManager.h"
#include "sqlite/sqlite3.h"

#ifndef MASTERTHESIS_DATABASELINESMANAGER_H
#define MASTERTHESIS_DATABASELINESMANAGER_H

using namespace std;

class DataBaseLinesManager : public DataBaseModelObjectsManager {
public:

    explicit DataBaseLinesManager(const string &dateBaseName);

    void addObjectToDataBase(int startPointID, int endPointID);

    void deleteObjectFromDataBase(int id);

    double getLineLengthFromPoints(int startPointID, int endPointID);
    double getLineInclinationAngleFromPoints(int startPointID, int endPointID);

    //Varibles

    int defaultCrossSectionID;
};


#endif //MASTERTHESIS_DATABASELINESMANAGER_H
