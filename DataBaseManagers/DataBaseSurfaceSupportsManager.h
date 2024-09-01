#ifndef DATABASESURFACESUPPORTSMANAGER_H
#define DATABASESURFACESUPPORTSMANAGER_H
#include "DataBaseModelObjectsManager.h"


namespace DataBaseManagers {

class DataBaseSurfaceSupportsManager : public DataBaseModelObjectsManager{

public:

    explicit DataBaseSurfaceSupportsManager(const std::string& dateBaseName);


    void addObjectToDataBase(int x1, int z1, int x2, int z2);
    void deleteObjectFromDataBase();
    void iterateOverTable();

    std::map<int, std::tuple<int, int, int, int> > getSurfaceSupportsMap() const;
private:
    std::map<int, std::tuple<int, int, int, int> > surfaceSupportsMap;
};

} // namespace DataBaseManagers

#endif // DATABASESURFACESUPPORTSMANAGER_H
