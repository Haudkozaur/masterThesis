
#include "DataBaseCrossSectionsManager.h"
#include "toStringWithPrecision.h"
using namespace std;

#include <iostream>
#include <sstream>
namespace DataBaseManagers {
DataBaseCrossSectionsManager::DataBaseCrossSectionsManager(const string &dateBaseName) : DataBaseModelObjectsManager(
        dateBaseName) {

}

void DataBaseCrossSectionsManager::addObjectToDataBase(const string &name, int materialID, double A, double I) {

    string queryAddCrossSection =
            "INSERT INTO " + tableTypesMap.at(TableType::CROSS_SECTIONS) + " (name, material_id, A, I) VALUES ('" + name + "', " + to_string(materialID) +
            ", " + toStringWithPrecision(A) + ", " + toStringWithPrecision(I) + ");";


    if (validate(materialID, TableType::MATERIALS)) {
        cout << "Material exists" << endl;
        executeAndCheckIfSQLOk(queryAddCrossSection, TableType::CROSS_SECTIONS);
    } else {
        cout << "Error: " << "Material doesn't exist in DB" << endl;
    }
    cout << "\n";

}

void DataBaseCrossSectionsManager::iterateOverTable()
{
    std::string querySelectAll = "SELECT id, name, material_id FROM " + tableTypesMap.at(TableType::CROSS_SECTIONS);
    std::vector<std::vector<string>> results = executeQuery(querySelectAll);

    crossSectionsMap.clear(); // Clear the map before populating it

    for (const auto &row : results) {
        if (row.size() == 3) {
            int id = stoi(row[0]);
            string name = row[1];
            int materialID = stoi(row[2]);
            crossSectionsMap[id] = name;
        }
    }

}

std::map<int, string> DataBaseCrossSectionsManager::getCrossSectionsMap() const
{
    return crossSectionsMap;
}
}
