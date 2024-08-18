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

    void addOrUpdateNodeToDataBase(int nodeId, double XCord, double ZCord);
    void addResultToDataBase(int lineId, int memberId, int nodeId, double Nx, double Vz, double My, double deformation, bool isStart);  // Updated signature
    std::map<int, std::vector<std::tuple<int, double, double, double, double, double, double, int, bool> > > getResultsMap() const;  // Updated signature

private:
    std::map<int, std::vector<std::tuple<int, double, double, double, double, double, double, int, bool> > > resultsMap;  // Updated signature
};

} // namespace DataBaseManagers
#endif // DATABASERESULTSMANAGER_H
