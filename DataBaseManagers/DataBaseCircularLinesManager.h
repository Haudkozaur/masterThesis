#ifndef DATABASECIRCULARLINESMANAGER_H
#define DATABASECIRCULARLINESMANAGER_H

#include "DataBaseModelObjectsManager.h"
namespace DataBaseManagers {
class DataBaseCircularLinesManager : public DataBaseModelObjectsManager{
public:
    explicit DataBaseCircularLinesManager(const string& dateBaseName);

    void addObjectToDataBase(int centreX, int centreZ, int diameter);
    void deleteObjectFromDataBase();
    void iterateOverTable();
    std::map<int, std::tuple<int, int, int>> getCircularLinesMap() const;
private:
    std::map<int, std::tuple<int, int, int>> circularLinesMap;

};
}


#endif // DATABASECIRCULARLINESMANAGER_H
