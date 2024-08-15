#ifndef DATABASERESULTSMANAGER_H
#define DATABASERESULTSMANAGER_H
#include "DataBaseModelObjectsManager.h"

namespace DataBaseManagers {
class DataBaseResultsManager : public DataBaseModelObjectsManager
{
public:
    explicit DataBaseResultsManager(std::string dateBaseName);

    void addObjectToDataBase(int nodeId, double XCord, double ZCord, double Nx, double Vz, double My, double deformation);

    void iterateOverTable();

    std::map<int, std::tuple<double,double,double,double,double,double>> getResultsMap() const;

private:
    std::map<int, std::tuple<double,double,double,double,double,double>> resultsMap;
};

} // namespace DataBaseManagers
#endif // DATABASERESULTSMANAGER_H
