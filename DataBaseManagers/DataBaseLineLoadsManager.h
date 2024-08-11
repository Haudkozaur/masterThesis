#ifndef MASTERTHESIS_DATABASELINELOADSMANAGER_H
#define MASTERTHESIS_DATABASELINELOADSMANAGER_H

#include "DataBaseModelObjectsManager.h"
namespace DataBaseManagers {
class DataBaseLineLoadsManager : public DataBaseModelObjectsManager
{
private:
    std::map<int, tuple<int, double, double>> lineLoadsMap;

public:
    DataBaseLineLoadsManager(const string &dateBaseName);

    void addObjectToDataBase(int lineID, double Fx, double Fz);

    void deleteObjectFromDataBase(int id);

    void editObjectInDataBase(int lineId, double Fx, double Fz);

    void iterateOverTable();

    std::map<int, tuple<int, double, double>> getLineLoadsMap();
};
}
#endif //MASTERTHESIS_DATABASELINELOADSMANAGER_H