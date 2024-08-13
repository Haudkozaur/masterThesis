#ifndef DATABASEMESHMANAGER_H
#define DATABASEMESHMANAGER_H

#include "DataBaseModelObjectsManager.h"
namespace DataBaseManagers {
class DataBaseMeshManager : public DataBaseModelObjectsManager{


public:
    explicit DataBaseMeshManager(const string& dateBaseName);

    void iterateOverTable();

    void addObjectToDataBase(int lineID, double nodeX, double nodeZ);

    void deleteObjectFromDataBase(int lineID);

    void clearTable();

    std::map<int, std::pair<int, std::pair<double, double>>> getMeshMap() const;

private:
    std::map<int, std::pair<int, std::pair<double, double>>> meshMap; // To store mesh with their IDs
};

} // namespace DataBaseManagers

#endif // DATABASEMESHMANAGER_H
