#include "DataBaseModelObjectsManager.h"
#include "sqlite/sqlite3.h"

#ifndef MASTERTHESIS_DATABASELINESMANAGER_H
#define MASTERTHESIS_DATABASELINESMANAGER_H

using namespace std;

class DataBaseLinesManager : public DataBaseModelObjectsManager{
public:

    explicit DataBaseLinesManager(const string& dateBaseName);

    void addObjectToDataBase(int startPointID, int endPointID);

    void deleteObjectFromDataBase(int id);

};


#endif //MASTERTHESIS_DATABASELINESMANAGER_H
