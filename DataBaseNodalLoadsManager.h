#include "DataBaseModelObjectsManager.h"
#ifndef MASTERTHESIS_NODALLOADSMANAGER_H
#define MASTERTHESIS_NODALLOADSMANAGER_H


class DataBaseNodalLoadsManager : public DataBaseModelObjectsManager{
public:
    explicit DataBaseNodalLoadsManager(const string& dateBaseName);

    void addObjectToDataBase(int pointID, double My, double Fz, double Fx);
    void deleteObjectFromDataBase(int id);

    void iterateOverTable();

    std::map<int, std::tuple<int, double, double, double>> getNodalLoadsMap();
    std::map<int, std::tuple<int, double, double, double>> nodalLoadsMap;

};




#endif //MASTERTHESIS_NODALLOADSMANAGER_H
