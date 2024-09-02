#ifndef DATABASESURFACELOADSMANAGER_H
#define DATABASESURFACELOADSMANAGER_H
#include "DataBaseModelObjectsManager.h"

namespace DataBaseManagers {
class DataBaseSurfaceLoadsManager : public DataBaseModelObjectsManager{
public:
    explicit DataBaseSurfaceLoadsManager(std::string dateBaseName);

    void addObjectToDataBase(int x1, int z1, int x2, int z2, double F);
    void deleteObjectFromDataBase();
    void editObjectInDataBase();
    void iterateOverTable();

    std::map<int, std::tuple<int, int, int, int, double>> getSurfaceLoadsMap();
private:

    std::map<int, std::tuple<int, int, int, int, double>> surfaceLoadsMap;
};
}
#endif // DATABASESURFACELOADSMANAGER_H






