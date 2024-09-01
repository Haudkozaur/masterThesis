#ifndef DATABASESLABLINELOADSMANAGER_H
#define DATABASESLABLINELOADSMANAGER_H
#include "DataBaseModelObjectsManager.h"

namespace DataBaseManagers {
class DataBaseSlabLineLoadsManager : public DataBaseModelObjectsManager{

public:
    DataBaseSlabLineLoadsManager(std::string dateBaseName);
    void addObjectToDataBase(int x1, int z1, int x2, int z2, double F);
    void deleteObjectFromDataBase();
    void editObjectInDataBase();
    void iterateOverTable();

    std::map<int, std::tuple<int, int, int, int, double>> getLineLoadsMap();
private:
    std::map<int, std::tuple<int, int, int, int, double>> lineLoadsMap;
};
}
#endif // DATABASESLABLINELOADSMANAGER_H

