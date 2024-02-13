#ifndef MASTERTHESIS_DATABASELINELOADSMANAGER_H
#define MASTERTHESIS_DATABASELINELOADSMANAGER_H


#include "DataBaseModelObjectsManager.h"

class DataBaseLineLoadsManager : public DataBaseModelObjectsManager {

public:

    DataBaseLineLoadsManager(const string &dateBaseName);

    void addObjectToDataBase(int lineID, double Fx, double Fz);

    void deleteObjectFromDataBase(int id);


};




#endif //MASTERTHESIS_DATABASELINELOADSMANAGER_H
