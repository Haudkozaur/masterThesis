#include "DataBaseModelObjectsManager.h"

DataBaseModelObjectsManager::DataBaseModelObjectsManager(string dateBaseName) { this->dateBaseName = dateBaseName;
    this->dateBaseNameAsChar = this->dateBaseName.c_str();
    sqlite3_open(this->dateBaseNameAsChar, &this->DB);
    cout << "Database " << this->dateBaseName << " has been opened" << endl;}

//void DataBaseModelObjectsManager::AddObjectToDataBase() {
//}
