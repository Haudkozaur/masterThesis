#include "DataBaseModelObjectsManager.h"
#ifndef MASTERTHESIS_NODALLOADSMANAGER_H
#define MASTERTHESIS_NODALLOADSMANAGER_H


class DataBaseNodalLoadsManager : public DataBaseModelObjectsManager{
public:
    explicit DataBaseNodalLoadsManager(const string& dateBaseName);

    void addObjectToDataBase(int pointID, int My, int Fz, int Fx);
    void deleteObjectFromDataBase(int id);
};




#endif //MASTERTHESIS_NODALLOADSMANAGER_H
