#include "DataBaseModelObjectsManager.h"
#include "sqlite/sqlite3.h"

DataBaseModelObjectsManager::DataBaseModelObjectsManager(string dateBaseName)
{
    this->dateBaseName = dateBaseName;
    this->dateBaseNameAsChar = this->dateBaseName.c_str();
    sqlite3_open(this->dateBaseNameAsChar, &this->DB);
    cout << "Database " << this->dateBaseName << " has been opened"
         << "\n"
         << endl;
}

void DataBaseModelObjectsManager::addObjectToDataBase() {}

void DataBaseModelObjectsManager::deleteObjectFromDataBase() {}

void DataBaseModelObjectsManager::iterateOverTable() {}

void DataBaseModelObjectsManager::dropTable(const TableType &tableName)
{
    string queryDropTable = "DROP TABLE " + tableTypesMap.at(tableName);
    cout << "Clearing table " + tableTypesMap.at(tableName) << "\n";
    executeAndCheckIfSQLOk(queryDropTable, tableName);

}




bool DataBaseModelObjectsManager::validate(int objectID, const TableType &tableName)
{
    if (objectID == 0) {
        cout << "Object with ID = 0 is not valid." << endl;
        return false;
    }
    sqlite3_open(this->dateBaseNameAsChar, &this->DB);
    cout << "Validating object with ID = " << objectID << " in table "
         << tableTypesMap.at(tableName) << endl;
    bool isObjectExist = true;
    string QueryCheckObject = "SELECT * FROM " + tableTypesMap.at(tableName)
                              + " WHERE id = " + to_string(objectID);

    //    cout << QueryCheckObject << endl;

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(DB, QueryCheckObject.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(DB));
        isObjectExist = false;

    } else {
        //      IDK exactly why this have to be here, but it works
        sqlite3_step(stmt);
        //        cout << sqlite3_column_int(stmt, 0) << endl;
        if (sqlite3_column_int(stmt, 0) != objectID) {
            cout << "Object with ID - " << objectID << " does not exist" << endl;
            isObjectExist = false;
        }
    }
    cout << "\n";
    sqlite3_close(DB);
    sqlite3_finalize(stmt);
    return isObjectExist;
}
bool DataBaseModelObjectsManager::checkIfDuplicate(TableType tableName,
                                                   const tuple<int, int, int> &properties)
{
    bool isDuplicate = false;
    string queryCheckIfDuplicate;
    if (tableName == TableType::POINTS) {
        queryCheckIfDuplicate = "SELECT * FROM " + tableTypesMap.at(tableName)
                                + " WHERE x_cord = " + to_string(get<0>(properties))
                                + " AND z_cord = " + to_string(get<1>(properties));
    } else if (tableName == TableType::LINES) {
        queryCheckIfDuplicate = "SELECT * FROM " + tableTypesMap.at(tableName)
                                + " WHERE start_point = " + to_string(get<0>(properties))
                                + " AND end_point = " + to_string(get<1>(properties));
    } else if (tableName == TableType::SUPPORTS) {
        queryCheckIfDuplicate = "SELECT * FROM " + tableTypesMap.at(tableName)
                                + " WHERE point_id = " + to_string(get<0>(properties));
    }
    sqlite3_open(this->dateBaseNameAsChar, &this->DB);
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, queryCheckIfDuplicate.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(DB));
        isDuplicate = true;
    } else {
        sqlite3_step(stmt);
        if (sqlite3_column_int(stmt, 0) != 0) {
            cout << "Object already exists" << endl;
            isDuplicate = true;
        }
    }

    sqlite3_close(DB);
    sqlite3_finalize(stmt);
    return isDuplicate;
}

bool DataBaseModelObjectsManager::checkIfNotNull(string value)
{
    if (value.empty()) {
        cout << "Value cannot be empty" << endl;
        return false;
    }
    return true;
}

void DataBaseModelObjectsManager::executeAndCheckIfSQLOk(const string &query, TableType tableName)
{
    int rc = sqlite3_exec(this->DB, query.c_str(), nullptr, nullptr, &this->zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;

    } else {
        cout << "Operation on table " + tableTypesMap.at(tableName) + " completed successfully"
             << endl;
    }
}

void DataBaseModelObjectsManager::selectAllFromTableByID(TableType tableName, int id)
{
    sqlite3_open(this->dateBaseNameAsChar, &this->DB);

    string querySelect = "SELECT * FROM " + tableTypesMap.at(tableName)
                         + " WHERE id = " + to_string(id);
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, querySelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(DB));
    } else {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < sqlite3_column_count(stmt); i++) {
                cout << sqlite3_column_name(stmt, i) << " = " << sqlite3_column_text(stmt, i)
                     << endl;
            }
        }
    }
    sqlite3_close(DB);
    sqlite3_finalize(stmt);
    cout << "\n";
}

string DataBaseModelObjectsManager::selectObjectPropertyByID(TableType tableName,
                                                             int id,
                                                             const string &propertyName) const
{
    string querySelect = "SELECT " + propertyName + " FROM " + tableTypesMap.at(tableName)
                         + " WHERE id = " + to_string(id);
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, querySelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(DB));
        return sqlite3_errmsg(DB);
    } else if (sqlite3_step(stmt) == SQLITE_ROW) {
        string result = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        sqlite3_close(DB);
        sqlite3_finalize(stmt);
        return result;

    } else {
        return "Object with ID - " + to_string(id) + " does not exist";
    }
}

void DataBaseModelObjectsManager::updateObjectInDataBase(TableType tableName,
                                                         int id,
                                                         const string &propertyName,
                                                         const string &newValue)
{
    string queryUpdate = "UPDATE " + tableTypesMap.at(tableName) + " SET " + propertyName + " = "
                         + newValue + " WHERE id = " + to_string(id);
    executeAndCheckIfSQLOk(queryUpdate, tableName);
    cout << "\n";
}
int DataBaseModelObjectsManager::getNumberOfObjectsInTable(TableType tableName)
{
    sqlite3_open(this->dateBaseNameAsChar, &this->DB);
    string querySelect = "SELECT COUNT(*) FROM " + tableTypesMap.at(tableName);
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, querySelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(DB));
    } else {
        sqlite3_step(stmt);
        int result = sqlite3_column_int(stmt, 0);
        sqlite3_close(DB);
        sqlite3_finalize(stmt);
        return result;
    }
    return 0;
}
vector<vector<string>> DataBaseModelObjectsManager::executeQuery(const string &query)
{
    sqlite3_stmt *stmt;
    vector<vector<string>> results;

    if (sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw runtime_error("Failed to execute query: " + string(sqlite3_errmsg(DB)));
    }

    int cols = sqlite3_column_count(stmt);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        vector<string> row;
        for (int i = 0; i < cols; ++i) {
            const char *val = reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
            row.push_back(val ? val : "");
        }
        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}
