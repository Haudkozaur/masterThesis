#include <iostream>
#include "sqlite/sqlite3.h"
#include "TableTypes.h"
#include "toStringWithPrecision.h"
#include <vector>
using namespace std;

#ifndef MASTERTHESIS_DATABASEMODELOBJECTSMANAGER_H
#define MASTERTHESIS_DATABASEMODELOBJECTSMANAGER_H


//abstract class to create model objects in the database

class DataBaseModelObjectsManager {
public:
    explicit DataBaseModelObjectsManager(string dateBaseName);

    virtual void addObjectToDataBase();
    virtual void deleteObjectFromDataBase();

    virtual void iterateOverTable();

    //virtual void editObjectInDataBase();

    bool validate(int id, const TableType& tableName);
    void executeAndCheckIfSQLOk(const string& query, TableType tableName);
    void selectAllFromTableByID(TableType tableName, int id);
    void updateObjectInDataBase(TableType tableName, int id, const string& propertyName, const string& newValue);
    string selectObjectPropertyByID(TableType tableName, int id, const string& propertyName) const;
    int getNumberOfObjectsInTable(TableType tableName);
    vector<vector<string>> executeQuery(const string& query); // Function to execute SELECT queries
    bool checkIfDuplicate(TableType tableName, const tuple<int, int, int> &properties);

    //Variables
    string dateBaseName;
    const char *dateBaseNameAsChar;
    sqlite3 *DB;
    char *zErrMsg = 0;
};


#endif //MASTERTHESIS_DATABASEMODELOBJECTSMANAGER_H
