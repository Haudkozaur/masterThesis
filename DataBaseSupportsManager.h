
#ifndef MASTERTHESIS_DATABASESUPPORTSMANAGER_H
#define MASTERTHESIS_DATABASESUPPORTSMANAGER_H


#include "DataBaseModelObjectsManager.h"
#include <map>
#include <tuple>

class DataBaseSupportsManager : public DataBaseModelObjectsManager{

public:

    explicit DataBaseSupportsManager(const string& dateBaseName);

    void addObjectToDataBase(int pointID, bool Rx, bool Tz, bool Tx);

    void deleteObjectFromDataBase(int id);

    void iterateOverTable();

    //getter of map
    map<int, tuple<int, bool, bool, bool>> getSupportsMap() const;

private:
    //Map that stores all supports parameters id -> (point_id, ry, tz, tx)
    map<int, tuple<int, bool, bool, bool>> supportsMap;

};

#endif //MASTERTHESIS_DATABASESUPPORTSMANAGER_H
