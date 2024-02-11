#include "DataBaseModelObjectsManager.h"
#include "sqlite/sqlite3.h"


DataBaseModelObjectsManager::DataBaseModelObjectsManager(string dateBaseName) {
    this->dateBaseName = dateBaseName;
    this->dateBaseNameAsChar = this->dateBaseName.c_str();
    sqlite3_open(this->dateBaseNameAsChar, &this->DB);
    cout << "Database " << this->dateBaseName << " has been opened" << "\n" << endl;
    sqlite3_stmt *stmt;
}

void DataBaseModelObjectsManager::addObjectToDataBase() {
}

void DataBaseModelObjectsManager::deleteObjectFromDataBase() {
}

bool DataBaseModelObjectsManager::validate(int objectID, const TableType &tableName) {
    sqlite3_open(this->dateBaseNameAsChar, &this->DB);
    cout <<"Validating object with ID = " << objectID << " in table " << tableTypesMap.at(tableName) << endl;
    bool isObjectExist = true;
    string QueryCheckObject = "SELECT * FROM " + tableTypesMap.at(tableName) + " WHERE id = " + to_string(objectID);

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
