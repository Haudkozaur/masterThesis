#ifndef DATABASEFREEFEMPREPARER_H
#define DATABASEFREEFEMPREPARER_H

#include "../Solver/Solver.h" // Assuming Solver-related classes are defined here
#include "DataBaseModelObjectsManager.h"
#include <map>
#include <string>
#include <vector>

namespace DataBaseManagers {

class DataBaseFreeFEMPreparer : public DataBaseModelObjectsManager {
public:
    explicit DataBaseFreeFEMPreparer(const std::string &dataBaseName);

    void fetchAllData();

    // Getter methods for accessing the fetched data
    const std::map<int, std::pair<int, int>> &getPoints() const;
    const std::map<int, std::tuple<int, int, double, double>> &getLines() const;
    const std::map<int, std::tuple<int, int, int, int, int, int, std::string, double>> &getMainSurface() const;
    const std::map<int, std::tuple<int, int, int, int, int, int, std::string, double>> &getOpenings() const;
    const std::map<int, std::tuple<int, int, int>> &getCircularLines() const;
    const std::map<int, std::tuple<std::string, double, double, double>> &getMaterials() const;
    const std::map<int, std::tuple<int, int>> &getLineSupports() const;
    const std::map<int, std::tuple<int, int, int, int>> &getSurfaceSupports() const;
    const std::map<int, std::tuple<int, int, double>> &getSlabPointLoads() const;
    const std::map<int, std::tuple<int, int, int, int, double>> &getSlabLineLoads() const;
    const std::map<int, std::tuple<int, int, int, int, double>> &getSurfaceLoads() const;
    const std::map<int, std::tuple<int, int>> &getMesh() const;

private:
    // Methods to fetch specific types of data
    void fetchPoints();
    void fetchLines();
    void fetchSurfaces(); // This method will now populate both mainSurfaceMap and openingsMap
    void fetchCircularLines();
    void fetchMaterials();
    void fetchLineSupports();
    void fetchSurfaceSupports();
    void fetchSlabPointLoads();
    void fetchSlabLineLoads();
    void fetchSurfaceLoads();
    void fetchMesh();

    // Maps to store the fetched data
    std::map<int, std::pair<int, int>> pointsMap;
    std::map<int, std::tuple<int, int, double, double>> linesMap;
    std::map<int, std::tuple<int, int, int, int, int, int, std::string, double>> mainSurfaceMap;
    std::map<int, std::tuple<int, int, int, int, int, int, std::string, double>> openingsMap;
    std::map<int, std::tuple<int, int, int>> circularLinesMap;
    std::map<int, std::tuple<std::string, double, double, double>> materialsMap;
    std::map<int, std::tuple<int, int>> lineSupportsMap;
    std::map<int, std::tuple<int, int, int, int>> surfaceSupportsMap;
    std::map<int, std::tuple<int, int, double>> slabPointLoadsMap;
    std::map<int, std::tuple<int, int, int, int, double>> slabLineLoadsMap;
    std::map<int, std::tuple<int, int, int, int, double>> surfaceLoadsMap;
    std::map<int, std::tuple<int, int>> meshMap;
};

} // namespace DataBaseManagers

#endif // DATABASEFREEFEMPREPARER_H
