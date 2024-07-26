#include "DataBaseModelObjectsManager.h"
#include "sqlite/sqlite3.h"
#include <map>
#ifndef MASTERTHESIS_DATEBASEPOINTSMANAGER_H
#define MASTERTHESIS_DATEBASEPOINTSMANAGER_H
#include <vector>
using namespace std;


class DataBasePointsManager : public DataBaseModelObjectsManager{
public:

    explicit DataBasePointsManager(string dateBaseName);

    void addObjectToDataBase(int XCoordinate, int ZCoordinate);
    void deleteObjectFromDataBase(int id);
    void iterateOverTable();
    void editPoint(int id, int XCoordinate, int ZCoordinate);
    std::map<int, std::pair<int, int>> getPointsMap() const;
private:
    map<int, pair<int, int>> pointsMap; // To store points with their IDs

};
#endif //MASTERTHESIS_DATEBASEPOINTSMANAGER_H
