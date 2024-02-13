
#ifndef MASTERTHESIS_DATABASESUPPORTSMANAGER_H
#define MASTERTHESIS_DATABASESUPPORTSMANAGER_H


#include "DataBaseModelObjectsManager.h"

class DataBaseSupportsManager : public DataBaseModelObjectsManager{

public:

    explicit DataBaseSupportsManager(const string& dateBaseName);

    void addObjectToDataBase(int pointID, bool Rx, bool Tz, bool Tx);

    void deleteObjectFromDataBase(int id);


};

#endif //MASTERTHESIS_DATABASESUPPORTSMANAGER_H
