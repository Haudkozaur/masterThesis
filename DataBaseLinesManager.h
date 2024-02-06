#include "DataBaseModelObjectsManager.h"
#include "sqlite/sqlite3.h"

#ifndef MASTERTHESIS_DATABASELINESMANAGER_H
#define MASTERTHESIS_DATABASELINESMANAGER_H

using namespace std;

class DataBaseLinesManager : public DataBaseModelObjectsManager{
public:

    explicit DataBaseLinesManager(string dateBaseName);

    void AddObjectToDataBase(int startPointID, int endPointID);

};


#endif //MASTERTHESIS_DATABASELINESMANAGER_H
