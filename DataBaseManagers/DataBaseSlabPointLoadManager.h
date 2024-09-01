#ifndef DATABASESLABPOINTLOADMANAGER_H
#define DATABASESLABPOINTLOADMANAGER_H
#include "DataBaseModelObjectsManager.h"

namespace DataBaseManagers {
class DataBaseSlabPointLoadManager : public DataBaseModelObjectsManager{
public:
    DataBaseSlabPointLoadManager(std::string dateBaseName);
    void addObjectToDataBase(double Fz, int x1, int z1);
    void deleteObjectFromDataBase();
    void editObjectInDataBase();
    void iterateOverTable();

    std::map<int, std::tuple<double, int, int>> getNodalLoadsMap();
    std::map<int, std::tuple<double, int, int>> nodalLoadsMap;
};
}
#endif // DATABASESLABPOINTLOADMANAGER_H
