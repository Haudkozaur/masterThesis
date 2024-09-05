#include "DataBaseFreeFEMPreparer.h"
#include "DataBaseStarter.h"

namespace DataBaseManagers {

DataBaseFreeFEMPreparer::DataBaseFreeFEMPreparer(const std::string &dataBaseName)
    : DataBaseModelObjectsManager(dataBaseName) {

}

void DataBaseFreeFEMPreparer::fetchAllData() {
    fetchPoints();
    cout << "Points fetched" << endl;
    fetchLines();
    cout << "Lines fetched" << endl;
    fetchSurfaces();
    cout << "Surfaces fetched" << endl;
    fetchCircularLines();
    cout << "CircularLines fetched" << endl;
    fetchMaterials();
    cout << "Materials fetched" << endl;
    fetchLineSupports();
    cout << "line supports fetched" << endl;
    fetchSurfaceSupports();
    cout << "SurfaceSupports fetched" << endl;
    fetchSlabPointLoads();
    cout << "SlabPointLoads fetched" << endl;
    fetchSlabLineLoads();
    cout << "SlabLineLoads fetched" << endl;
    fetchSurfaceLoads();
    cout << "SurfaceLoads fetched" << endl;
    fetchMesh();
    cout << "Mesh fetched" << endl;
}

void DataBaseFreeFEMPreparer::fetchPoints() {
    std::string querySelectAllPoints = "SELECT id, x_cord, z_cord FROM points";
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllPoints);

    pointsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 3) {
            int id = std::stoi(row[0]);
            int x = std::stoi(row[1]);
            int z = std::stoi(row[2]);
            pointsMap[id] = std::make_pair(x, z);
        }
    }
}

void DataBaseFreeFEMPreparer::fetchLines() {
    std::string querySelectAllLines = "SELECT id, start_point, end_point, length, inclination_angle FROM lines";
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllLines);

    linesMap.clear();
    for (const auto &row : results) {
        if (row.size() == 5) {
            int id = std::stoi(row[0]);
            int startPoint = std::stoi(row[1]);
            int endPoint = std::stoi(row[2]);
            double length = std::stod(row[3]);
            double inclinationAngle = std::stod(row[4]);
            linesMap[id] = std::make_tuple(startPoint, endPoint, length, inclinationAngle); // Corrected assignment
        }
    }
}


void DataBaseFreeFEMPreparer::fetchSurfaces() {
    std::string querySelectAllSurfaces = "SELECT id, surface_type, line1_id, line2_id, line3_id, line4_id, circular_line_id, material_id, thickness, is_opening FROM surfaces";
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllSurfaces);

    mainSurfaceMap.clear();
    openingsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 10) {
            int id = std::stoi(row[0]);
            std::string surfaceType = row[1];
            int line1Id = row[2].empty() ? -1 : std::stoi(row[2]);
            int line2Id = row[3].empty() ? -1 : std::stoi(row[3]);
            int line3Id = row[4].empty() ? -1 : std::stoi(row[4]);
            int line4Id = row[5].empty() ? -1 : std::stoi(row[5]);
            int circularLineId = row[6].empty() ? -1 : std::stoi(row[6]);
            int materialId = std::stoi(row[7]);  // materialId is converted to int
            double thickness = row[8].empty() ? -1 : std::stod(row[8]);
            bool isOpening = row[9] == "1";

            if (isOpening) {
                openingsMap[id] = std::make_tuple(line1Id, line2Id, line3Id, line4Id, circularLineId, materialId, surfaceType, thickness);
            } else {
                mainSurfaceMap[id] = std::make_tuple(line1Id, line2Id, line3Id, line4Id, circularLineId, materialId, surfaceType, thickness);  // Store as int
            }
        }
    }

}




void DataBaseFreeFEMPreparer::fetchCircularLines() {

    std::string querySelectAllCircularLines = "SELECT id, centre_x, centre_z, diameter FROM circular_lines";
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllCircularLines);

    circularLinesMap.clear();
    for (const auto &row : results) {
        if (row.size() == 4) {
            int id = std::stoi(row[0]);
            int centreX = std::stoi(row[1]);
            int centreZ = std::stoi(row[2]);
            int diameter = std::stoi(row[3]);
            circularLinesMap[id] = std::make_tuple(centreX, centreZ, diameter);
        }
    }
}

void DataBaseFreeFEMPreparer::fetchMaterials() {
    std::string querySelectAllMaterials = "SELECT id, name, E, v, ro FROM materials";
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllMaterials);

    materialsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 5) {
            int id = std::stoi(row[0]);
            std::string name = row[1];
            double E = std::stod(row[2]);
            double v = std::stod(row[3]);
            double ro = std::stod(row[4]);
            materialsMap[id] = std::make_tuple(name, E, v, ro);
        }
    }
}

void DataBaseFreeFEMPreparer::fetchLineSupports() {
    std::string querySelectAllLineSupports = "SELECT id, line_id, circular_line_id FROM line_supports";
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllLineSupports);

    lineSupportsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 3) {
            int id = std::stoi(row[0]);
            int lineId = row[1].empty() ? -1 : std::stoi(row[1]);
            int circularLineId = row[2].empty() ? -1 : std::stoi(row[2]);
            lineSupportsMap[id] = std::make_tuple(lineId, circularLineId);
        }
    }
}

void DataBaseFreeFEMPreparer::fetchSurfaceSupports() {
    std::string querySelectAllSurfaceSupports = "SELECT id, x1, z1, x2, z2 FROM surface_supports";
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllSurfaceSupports);

    surfaceSupportsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 5) {
            int id = std::stoi(row[0]);
            int x1 = std::stoi(row[1]);
            int z1 = std::stoi(row[2]);
            int x2 = std::stoi(row[3]);
            int z2 = std::stoi(row[4]);
            surfaceSupportsMap[id] = std::make_tuple(x1, z1, x2, z2);
        }
    }
}

void DataBaseFreeFEMPreparer::fetchSlabPointLoads() {
    std::string querySelectAllSlabPointLoads = "SELECT id, x1, z1, Fz FROM slab_point_loads";
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllSlabPointLoads);

    slabPointLoadsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 4) {
            int id = std::stoi(row[0]);
            int x1 = std::stoi(row[1]);
            int z1 = std::stoi(row[2]);
            double Fz = std::stod(row[3]);
            slabPointLoadsMap[id] = std::make_tuple(x1, z1, Fz);
        }
    }
}

void DataBaseFreeFEMPreparer::fetchSlabLineLoads() {
    std::string querySelectAllSlabLineLoads = "SELECT id, x1, z1, x2, z2, Fz FROM slab_line_loads";
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllSlabLineLoads);

    slabLineLoadsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 6) {
            int id = std::stoi(row[0]);
            int x1 = std::stoi(row[1]);
            int z1 = std::stoi(row[2]);
            int x2 = std::stoi(row[3]);
            int z2 = std::stoi(row[4]);
            double Fz = std::stod(row[5]);
            slabLineLoadsMap[id] = std::make_tuple(x1, z1, x2, z2, Fz);
        }
    }
}

void DataBaseFreeFEMPreparer::fetchSurfaceLoads() {
    std::string querySelectAllSurfaceLoads = "SELECT id, x1, z1, x2, z2, F FROM surface_loads";
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllSurfaceLoads);

    surfaceLoadsMap.clear();
    for (const auto &row : results) {
        if (row.size() == 6) {
            int id = std::stoi(row[0]);
            int x1 = std::stoi(row[1]);
            int z1 = std::stoi(row[2]);
            int x2 = std::stoi(row[3]);
            int z2 = std::stoi(row[4]);
            double F = std::stod(row[5]) * 1000;  // Convert force to correct units

            surfaceLoadsMap[id] = std::make_tuple(x1, z1, x2, z2, F);
        }
    }

    // Print the content of the surfaceLoadsMap for debugging
    std::cout << "Surface Loads Map Content:" << std::endl;
    for (const auto &entry : surfaceLoadsMap) {
        int id = entry.first;
        int x1 = std::get<0>(entry.second);
        int z1 = std::get<1>(entry.second);
        int x2 = std::get<2>(entry.second);
        int z2 = std::get<3>(entry.second);
        double F = std::get<4>(entry.second);

        std::cout << "ID: " << id << ", x1: " << x1 << ", z1: " << z1
                  << ", x2: " << x2 << ", z2: " << z2
                  << ", F: " << F << std::endl;
    }
}

void DataBaseFreeFEMPreparer::fetchMesh() {
    std::string querySelectAllMesh = "SELECT id, line_id, number_of_FE FROM slab_mesh";
    std::vector<std::vector<std::string>> results = executeQuery(querySelectAllMesh);

    meshMap.clear();
    for (const auto &row : results) {
        if (row.size() == 3) {
            int id = std::stoi(row[0]);
            int lineId = std::stoi(row[1]);
            int numberOfFE = std::stoi(row[2]);
            meshMap[id] = std::make_tuple(lineId, numberOfFE);
        }
    }
}

// Getter methods
const std::map<int, std::pair<int, int>> &DataBaseFreeFEMPreparer::getPoints() const {
    return pointsMap;
}

const std::map<int, std::tuple<int, int, double, double>> &DataBaseFreeFEMPreparer::getLines() const {
    return linesMap;
}

const std::map<int, std::tuple<int, int, int, int, int, int, std::string, double>> &DataBaseFreeFEMPreparer::getMainSurface() const {
    return mainSurfaceMap;
}

const std::map<int, std::tuple<int, int, int, int, int, int, std::string, double>> &DataBaseFreeFEMPreparer::getOpenings() const {
    return openingsMap;
}

const std::map<int, std::tuple<int, int, int>> &DataBaseFreeFEMPreparer::getCircularLines() const {
    return circularLinesMap;
}

const std::map<int, std::tuple<std::string, double, double, double>> &DataBaseFreeFEMPreparer::getMaterials() const {
    return materialsMap;
}

const std::map<int, std::tuple<int, int>> &DataBaseFreeFEMPreparer::getLineSupports() const {
    return lineSupportsMap;
}

const std::map<int, std::tuple<int, int, int, int>> &DataBaseFreeFEMPreparer::getSurfaceSupports() const {
    return surfaceSupportsMap;
}

const std::map<int, std::tuple<int, int, double>> &DataBaseFreeFEMPreparer::getSlabPointLoads() const {
    return slabPointLoadsMap;
}

const std::map<int, std::tuple<int, int, int, int, double>> &DataBaseFreeFEMPreparer::getSlabLineLoads() const {
    return slabLineLoadsMap;
}

const std::map<int, std::tuple<int, int, int, int, double>> &DataBaseFreeFEMPreparer::getSurfaceLoads() const {
    return surfaceLoadsMap;
}

const std::map<int, std::tuple<int, int>> &DataBaseFreeFEMPreparer::getMesh() const {
    return meshMap;
}

} // namespace DataBaseManagers
