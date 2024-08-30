#ifndef MASTERTHESIS_DATABASELINESMANAGER_H
#define MASTERTHESIS_DATABASELINESMANAGER_H

#include "DataBaseModelObjectsManager.h"
#include <map>
#include <tuple>
#include <vector>
#include <string>

using namespace std;
namespace DataBaseManagers {
class DataBaseLinesManager : public DataBaseModelObjectsManager
{
public:
    explicit DataBaseLinesManager(const string &databaseName);

    void addObjectToDataBase(int startPointID, int endPointID);
    void deleteObjectFromDataBase(int id);
    void editLine(int id, int newStartPoint, int newEndPoint);
    void editLine(int id, int crossSectionId);

    double getLineLengthFromPoints(int startPointID, int endPointID);
    double getLineInclinationAngleFromPoints(int startPointID, int endPointID);
    void iterateOverTable();

    // Map of lines with parameters
    map<int, tuple<int, int, int>> getLinesMap() const;

    void addObjectToDataBaseConsideringCircularLines(int startPointID, int endPointID);
private:
    map<int, tuple<int, int, int>> linesMap;
    int defaultCrossSectionID;
};
}
#endif // MASTERTHESIS_DATABASELINESMANAGER_H
