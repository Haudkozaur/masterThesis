#ifndef DATABASELINESUPPORTSMANAGER_H
#define DATABASELINESUPPORTSMANAGER_H

#include "DataBaseModelObjectsManager.h"

namespace DataBaseManagers {

class DataBaseLineSupportsManager : public DataBaseModelObjectsManager
{
public:
    explicit DataBaseLineSupportsManager(const string &databaseName);

    void addObjectToDataBase(int lineID);

    void deleteObjectFromDataBase(int lineID);

    void iterateOverTable();

    std::map<int, std::tuple<int, int>> getLineSupportsMap() const;
private:
    std::map<int, std::tuple<int, int>> lineSupportsMap;
};

} // namespace DataBaseManagers

#endif // DATABASELINESUPPORTSMANAGER_H
