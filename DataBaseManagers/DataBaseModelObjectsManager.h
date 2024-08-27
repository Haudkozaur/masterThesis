#ifndef MASTERTHESIS_DATABASEMODELOBJECTSMANAGER_H
#define MASTERTHESIS_DATABASEMODELOBJECTSMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include "../sqlite/sqlite3.h"
#include "TableTypes.h"

namespace DataBaseManagers {

class DataBaseModelObjectsManager {
public:
    explicit DataBaseModelObjectsManager(const std::string& dateBaseName);

    virtual void addObjectToDataBase();
    virtual void deleteObjectFromDataBase();
    virtual void iterateOverTable();

    void dropTable(const TableType& tableName);
    bool validate(int id, const TableType& tableName);
    void executeAndCheckIfSQLOk(const std::string& query, TableType tableName);
    void selectAllFromTableByID(TableType tableName, int id);
    void updateObjectInDataBase(TableType tableName, int id, const std::string& propertyName, const std::string& newValue);
    std::string selectObjectPropertyByID(TableType tableName, int id, const std::string& propertyName) const;
    int getNumberOfObjectsInTable(TableType tableName);
    std::vector<std::vector<std::string>> executeQuery(const std::string& query) const;
    bool checkIfDuplicate(TableType tableName, const std::tuple<int, int, int>& properties);
    bool checkIfNotNull(const std::string& value);
    int getLastInsertedRowID();

private:
    std::string dateBaseName;
    const char* dateBaseNameAsChar;
    sqlite3* DB;
    char* zErrMsg = nullptr;
};

} // namespace DataBaseManagers

#endif // MASTERTHESIS_DATABASEMODELOBJECTSMANAGER_H
