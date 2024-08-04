#include "DataBaseLineLoadsManager.h"
#include "toStringWithPrecision.h"
namespace DataBaseManagers {
DataBaseLineLoadsManager::DataBaseLineLoadsManager(const string &dateBaseName)
    : DataBaseModelObjectsManager(dateBaseName)
{}

void DataBaseLineLoadsManager::addObjectToDataBase(int lineID, double Fx, double Fz)
{
    //contamination of add line load query
    string queryInsertLineLoad = "INSERT INTO " + tableTypesMap.at(TableType::LINE_LOADS)
                                 + " (id, line_id, Fx, Fz) VALUES (NULL, " + to_string(lineID)
                                 + ", " + toStringWithPrecision(Fx) + ", "
                                 + toStringWithPrecision(Fz) + ")";

    if (validate(lineID, TableType::LINES)) {
        cout << "Line exists" << endl;

        executeAndCheckIfSQLOk(queryInsertLineLoad, TableType::LINE_LOADS);
    } else {
        cout << "Error: "
             << "Line doesn't exist in DB" << endl;
    }
    cout << "\n";
}

void DataBaseLineLoadsManager::deleteObjectFromDataBase(int id)
{
    string queryDeleteLineLoad = "DELETE FROM " + tableTypesMap.at(TableType::LINE_LOADS)
                                 + " WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryDeleteLineLoad, TableType::LINE_LOADS);
    cout << "\n";
}

std::map<int, tuple<int, double, double>> DataBaseLineLoadsManager::getLineLoadsMap()
{
    return lineLoadsMap;
}

void DataBaseLineLoadsManager::iterateOverTable()
{
    lineLoadsMap.clear();

    string querySelectLineLoads = "SELECT * FROM " + tableTypesMap.at(TableType::LINE_LOADS);
    vector<vector<string>> lineLoadsData = executeQuery(querySelectLineLoads);

    for (const auto &lineLoad : lineLoadsData) {
        int id = stoi(lineLoad[0]);
        int lineID = stoi(lineLoad[1]);
        double Fx = stod(lineLoad[2]);
        double Fz = stod(lineLoad[3]);
        lineLoadsMap[id] = make_tuple(lineID, Fx, Fz);
    }
    //print line loads for Debug
    for (const auto &lineLoad : lineLoadsMap) {
        cout << "ID: " << lineLoad.first << " LineID: " << get<0>(lineLoad.second)
             << " Fx: " << get<1>(lineLoad.second) << " Fz: " << get<2>(lineLoad.second) << endl;
    }


}
}
