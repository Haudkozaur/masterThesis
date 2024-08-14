#include "DataBaseModelObjectsManager.h"

namespace DataBaseManagers {

DataBaseModelObjectsManager::DataBaseModelObjectsManager(const std::string& dateBaseName)
    : dateBaseName(dateBaseName), dateBaseNameAsChar(dateBaseName.c_str()), DB(nullptr) {
    sqlite3_open(dateBaseNameAsChar, &DB);
    std::cout << "Database " << dateBaseName << " has been opened" << std::endl;
}

void DataBaseModelObjectsManager::addObjectToDataBase() {}

void DataBaseModelObjectsManager::deleteObjectFromDataBase() {}

void DataBaseModelObjectsManager::iterateOverTable() {}

void DataBaseModelObjectsManager::dropTable(const TableType& tableName) {
    std::string queryDropTable = "DROP TABLE " + tableTypesMap.at(tableName);
    std::cout << "Clearing table " + tableTypesMap.at(tableName) << std::endl;
    executeAndCheckIfSQLOk(queryDropTable, tableName);
}

bool DataBaseModelObjectsManager::validate(int objectID, const TableType& tableName) {
    if (objectID == 0) {
        std::cout << "Object with ID = 0 is not valid." << std::endl;
        return false;
    }
    sqlite3_open(dateBaseNameAsChar, &DB);
    std::cout << "Validating object with ID = " << objectID << " in table "
              << tableTypesMap.at(tableName) << std::endl;
    bool isObjectExist = true;
    std::string QueryCheckObject = "SELECT * FROM " + tableTypesMap.at(tableName)
                                   + " WHERE id = " + std::to_string(objectID);

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DB, QueryCheckObject.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(DB));
        isObjectExist = false;
    } else {
        sqlite3_step(stmt);
        if (sqlite3_column_int(stmt, 0) != objectID) {
            std::cout << "Object with ID - " << objectID << " does not exist" << std::endl;
            isObjectExist = false;
        }
    }
    std::cout << std::endl;
    sqlite3_close(DB);
    sqlite3_finalize(stmt);
    return isObjectExist;
}

bool DataBaseModelObjectsManager::checkIfDuplicate(TableType tableName, const std::tuple<int, int, int>& properties) {
    bool isDuplicate = false;
    cout << "Checking if object already exists in table " + tableTypesMap.at(tableName) << endl;
    cout << "DataBaseName: " << dateBaseName << endl;
    std::string queryCheckIfDuplicate;
    if (tableName == TableType::POINTS) {
        queryCheckIfDuplicate = "SELECT * FROM " + tableTypesMap.at(tableName)
                                + " WHERE x_cord = " + std::to_string(std::get<0>(properties))
                                + " AND z_cord = " + std::to_string(std::get<1>(properties));
        cout << queryCheckIfDuplicate << endl;
    } else if (tableName == TableType::LINES) {
        queryCheckIfDuplicate = "SELECT * FROM " + tableTypesMap.at(tableName)
                                + " WHERE start_point = " + std::to_string(std::get<0>(properties))
                                + " AND end_point = " + std::to_string(std::get<1>(properties));
    } else if (tableName == TableType::SUPPORTS) {
        queryCheckIfDuplicate = "SELECT * FROM " + tableTypesMap.at(tableName)
                                + " WHERE point_id = " + std::to_string(std::get<0>(properties));
    }
    // sqlite3_open(dateBaseNameAsChar, &DB);
    // cout << "db opened" << endl;
    // cout << dateBaseNameAsChar << endl;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DB, queryCheckIfDuplicate.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(DB));
        isDuplicate = true;
    } else {
        sqlite3_step(stmt);
        if (sqlite3_column_int(stmt, 0) != 0) {
            std::cout << "Object already exists" << std::endl;
            isDuplicate = true;
        }
    }
    sqlite3_close(DB);
    sqlite3_finalize(stmt);
    return isDuplicate;
}

bool DataBaseModelObjectsManager::checkIfNotNull(const std::string& value) {
    if (value.empty()) {
        std::cout << "Value cannot be empty" << std::endl;
        return false;
    }
    return true;
}

void DataBaseModelObjectsManager::executeAndCheckIfSQLOk(const std::string& query, TableType tableName) {
    int rc = sqlite3_exec(DB, query.c_str(), nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cout << "Error: " << zErrMsg << std::endl;
    } else {
        std::cout << "Operation on table " + tableTypesMap.at(tableName) + " completed successfully" << std::endl;
    }
}

void DataBaseModelObjectsManager::selectAllFromTableByID(TableType tableName, int id) {
    sqlite3_open(dateBaseNameAsChar, &DB);
    std::string querySelect = "SELECT * FROM " + tableTypesMap.at(tableName)
                              + " WHERE id = " + std::to_string(id);
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DB, querySelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(DB));
    } else {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < sqlite3_column_count(stmt); i++) {
                std::cout << sqlite3_column_name(stmt, i) << " = " << sqlite3_column_text(stmt, i) << std::endl;
            }
        }
    }
    sqlite3_close(DB);
    sqlite3_finalize(stmt);
    std::cout << std::endl;
}

std::string DataBaseModelObjectsManager::selectObjectPropertyByID(TableType tableName, int id, const std::string& propertyName) const {
    std::string querySelect = "SELECT " + propertyName + " FROM " + tableTypesMap.at(tableName)
                              + " WHERE id = " + std::to_string(id);
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DB, querySelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(DB));
        return sqlite3_errmsg(DB);
    } else if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        sqlite3_close(DB);
        sqlite3_finalize(stmt);
        return result;
    } else {
        return "Object with ID - " + std::to_string(id) + " does not exist";
    }
}

void DataBaseModelObjectsManager::updateObjectInDataBase(TableType tableName, int id, const std::string& propertyName, const std::string& newValue) {
    std::string queryUpdate = "UPDATE " + tableTypesMap.at(tableName) + " SET " + propertyName + " = "
                              + newValue + " WHERE id = " + std::to_string(id);
    executeAndCheckIfSQLOk(queryUpdate, tableName);
    std::cout << std::endl;
}

int DataBaseModelObjectsManager::getNumberOfObjectsInTable(TableType tableName) {
    sqlite3_open(dateBaseNameAsChar, &DB);
    std::string querySelect = "SELECT COUNT(*) FROM " + tableTypesMap.at(tableName);
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DB, querySelect.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(DB));
    } else {
        sqlite3_step(stmt);
        int result = sqlite3_column_int(stmt, 0);
        sqlite3_close(DB);
        sqlite3_finalize(stmt);
        return result;
    }
    return 0;
}

std::vector<std::vector<std::string>> DataBaseModelObjectsManager::executeQuery(const std::string& query) {
    sqlite3_stmt* stmt;
    std::vector<std::vector<std::string>> results;

    if (sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to execute query: " + std::string(sqlite3_errmsg(DB)));
    }

    int cols = sqlite3_column_count(stmt);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<std::string> row;
        for (int i = 0; i < cols; ++i) {
            const char* val = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row.push_back(val ? val : "");
        }
        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}

} // namespace DataBaseManagers
