#include "DataBaseModelObjectsManager.h"
#ifndef MASTERTHESIS_DATEBASEMATERIALSMANAGER_H
#define MASTERTHESIS_DATEBASEMATERIALSMANAGER_H


class DataBaseMaterialsManager : public DataBaseModelObjectsManager{


public:
    explicit DataBaseMaterialsManager(const string& dateBaseName);

    void addObjectToDataBase(const string &name, double E, double v, double ro);

    void deleteObjectFromDataBase(int id);

};
#endif //MASTERTHESIS_DATEBASEMATERIALSMANAGER_H