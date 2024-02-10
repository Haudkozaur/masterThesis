#include "DataBaseModelObjectsManager.h"
#include "sqlite/sqlite3.h"

#ifndef MASTERTHESIS_DATEBASEPOINTSMANAGER_H
#define MASTERTHESIS_DATEBASEPOINTSMANAGER_H

using namespace std;


class DataBasePointsManager : public DataBaseModelObjectsManager{
public:

    explicit DataBasePointsManager(string dateBaseName);

    void AddObjectToDataBase(int YCoordinate, int ZCoordinate);
    void deleteObjectFromDataBase(int id);


};
#endif //MASTERTHESIS_DATEBASEPOINTSMANAGER_H
