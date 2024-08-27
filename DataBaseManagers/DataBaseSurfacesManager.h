#ifndef DATABASESURFACESMANAGER_H
#define DATABASESURFACESMANAGER_H

#include "DataBaseModelObjectsManager.h"
#include <map>
#include <tuple>


namespace DataBaseManagers {

class DataBaseSurfacesManager : public DataBaseModelObjectsManager{

public:
    DataBaseSurfacesManager(const string &dateBaseName);


    void deleteObjectFromDataBase(int id);
    void iterateOverTable();
    bool hasNonOpeningSurface();
    //void editSurface(int id, int XCoordinate, int ZCoordinate);
    std::map<int, std::tuple<std::string, int, int, int, int, int, int, int, bool>> getSurfacesMap() const;

    void addObjectToDataBase(int line1_id, int line2_id, int line3_id, int material_id, int thickness, bool is_opening);
    void addObjectToDataBase(int circular_line_id, int material_id, int thickness, bool is_opening);
    void addObjectToDataBase(int line1_id, int line2_id, int line3_id, int line4_id, int material_id, int thickness, bool is_opening);
    std::tuple<std::string, int, int, int, int, int, int, int, bool> getMainSurface();
private:
    std::map<int, std::tuple<std::string, int, int, int, int, int, int, int, bool>> surfacesMap; // To store points with their IDs
};

} // namespace DataBaseManagers


#endif // DATABASESURFACESMANAGER_H
