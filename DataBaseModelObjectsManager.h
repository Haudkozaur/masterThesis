#include <iostream>
#include "sqlite/sqlite3.h"
using namespace std;
#ifndef MASTERTHESIS_DATABASEMODELOBJECTSMANAGER_H
#define MASTERTHESIS_DATABASEMODELOBJECTSMANAGER_H

//abstract class to create model objects in the database

class DataBaseModelObjectsManager {
public:
    explicit DataBaseModelObjectsManager(string dateBaseName);

    //virtual void AddObjectToDataBase();
    //virtual void DeleteObjectFromDataBase();
    //virtual void EditObjectInDataBase();


    //Variables
    string dateBaseName;
    const char *dateBaseNameAsChar;
    sqlite3 *DB;
    char *zErrMsg = 0;
};


#endif //MASTERTHESIS_DATABASEMODELOBJECTSMANAGER_H
