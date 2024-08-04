#include "DataBaseNodalLoadsManager.h"
#include "DataBaseModelObjectsManager.h"
#include "toStringWithPrecision.h"
namespace DataBaseManagers {
DataBaseNodalLoadsManager::DataBaseNodalLoadsManager(const string &dateBaseName)
    : DataBaseModelObjectsManager(dateBaseName)
{}

void DataBaseNodalLoadsManager::addObjectToDataBase(int pointID, double My, double Fz, double Fx)
{
    string queryAddNodalLoad = "INSERT INTO " + tableTypesMap.at(TableType::NODAL_LOADS)
                               + " (point_id, My, Fz, Fx) VALUES (" + to_string(pointID) + ", "
                               + toStringWithPrecision(My) + ", " + toStringWithPrecision(Fz) + ", "
                               + toStringWithPrecision(Fx) + ")";

    if (validate(pointID, TableType::POINTS)) {
        cout << "Point exists" << endl;
        executeAndCheckIfSQLOk(queryAddNodalLoad, TableType::NODAL_LOADS);
    } else {
        cout << "Error: "
             << "point doesn't exist in DB" << endl;
        return;
    }
    cout << "\n";
}

void DataBaseNodalLoadsManager::deleteObjectFromDataBase(int id)
{
    string queryDeleteNodalLoad = "DELETE FROM " + tableTypesMap.at(TableType::NODAL_LOADS)
                                  + " WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeleteNodalLoad, TableType::NODAL_LOADS);
    cout << "\n";
}

std::map<int, std::tuple<int, double, double, double>> DataBaseNodalLoadsManager::getNodalLoadsMap()
{
    return nodalLoadsMap;
}

void DataBaseNodalLoadsManager::iterateOverTable()
{
    string querySelectAllNodalLoads = "SELECT * FROM " + tableTypesMap.at(TableType::NODAL_LOADS);

    std::vector<std::vector<string>> results = executeQuery(querySelectAllNodalLoads);

    nodalLoadsMap.clear(); // Clear the map before populating it

    for (const auto &row : results) {
        if (row.size() == 5) {
            int id = stoi(row[0]);
            int pointId = stoi(row[1]);
            double My = stod(row[2]);
            double Fz = stod(row[3]);
            double Fx = stod(row[4]);
            nodalLoadsMap[id] = make_tuple(pointId, My, Fz, Fx);
        }
    }
    // Print points for debugging
    for (const auto &load : nodalLoadsMap) {
        cout << "ID: " << load.first << ", PointID: " << get<0>(load.second)
             << ", My: " << get<1>(load.second) << ", Fz: " << get<2>(load.second)
             << ", Fx: " << get<3>(load.second) << endl;
    }
}
}
