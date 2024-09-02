#ifndef DATABASESLABMESHMANAGER_H
#define DATABASESLABMESHMANAGER_H
#include "DataBaseModelObjectsManager.h"
namespace DataBaseManagers {
class DataBaseSlabMeshManager : public DataBaseModelObjectsManager{

public:
    DataBaseSlabMeshManager(std::string dateBaseName);
    void addObjectToDataBase(int lineID, int numberOfFE);
    void deleteObjectFromDataBase();
    void editObjectInDataBase();
    void iterateOverTable();
    void clearTable();

    std::map<int, std::tuple<int, int>> getMeshMap() const;
private:
    std::map<int, std::tuple<int, int>> meshMap; // To store mesh with their IDs
};

} // namespace DataBaseManagers


#endif // DATABASESLABMESHMANAGER_H
