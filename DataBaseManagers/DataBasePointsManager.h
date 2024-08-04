#ifndef MASTERTHESIS_DATEBASEPOINTSMANAGER_H
#define MASTERTHESIS_DATEBASEPOINTSMANAGER_H

#include <iostream>
#include <map>
#include <vector>
#include "DataBaseModelObjectsManager.h"
#include "TableTypes.h"

namespace DataBaseManagers {

class DataBasePointsManager : public DataBaseModelObjectsManager {
public:
    explicit DataBasePointsManager(std::string dateBaseName);

    void addObjectToDataBase(int XCoordinate, int ZCoordinate);
    void deleteObjectFromDataBase(int id);
    void iterateOverTable();
    void editPoint(int id, int XCoordinate, int ZCoordinate);
    std::map<int, std::pair<int, int>> getPointsMap() const;

private:
    std::map<int, std::pair<int, int>> pointsMap; // To store points with their IDs
};

} // namespace DataBaseManagers

#endif // MASTERTHESIS_DATEBASEPOINTSMANAGER_H
