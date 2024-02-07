#include "DataBaseLinesManager.h"

DataBaseLinesManager::DataBaseLinesManager(string dateBaseName) : DataBaseModelObjectsManager(dateBaseName) {
    this->dateBaseName = dateBaseName;
}

void DataBaseLinesManager::AddObjectToDataBase(int startPointID, int endPointID) {

    //contamination of add line query
    string QueryInsertLine =
            "INSERT INTO lines (id, start_point, end_point) VALUES (NULL, " + to_string(startPointID) + ", " +
            to_string(endPointID) + ")";


    //check if point with whis id exists
    string QueryCheckStartPoint = "SELECT * FROM points WHERE id = " + to_string(startPointID);
    string QueryCheckEndPoint = "SELECT * FROM points WHERE id = " + to_string(endPointID);
    int checkStartPoint = sqlite3_exec(this->DB, QueryCheckStartPoint.c_str(), nullptr, nullptr, &this->zErrMsg);
    int checkEndPoint = sqlite3_exec(this->DB, QueryCheckEndPoint.c_str(), nullptr, nullptr, &this->zErrMsg);


    string sql("SELECT * FROM points WHERE id = 2");
    sqlite3_stmt* stmt;
    // compile sql statement to binary
    if(sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        printf("ERROR: while compiling sql: %s\n", sqlite3_errmsg(DB));
        sqlite3_close(DB);
        sqlite3_finalize(stmt);
        return;
    }

    // execute sql statement, and while there are rows returned, print ID
    int ret_code = 0;
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("TEST: ID = %d\n", sqlite3_column_int(stmt, 0));
        if(sqlite3_column_int(stmt, 0) != 2){
            cout << "Point with this ID does not exist" << endl;
        }
    }



    if(ret_code != SQLITE_DONE) {
        //this error handling could be done better, but it works
        printf("ERROR: while performing sql: %s\n", sqlite3_errmsg(DB));
        printf("ret_code = %d\n", ret_code);
    }



    if (checkStartPoint == SQLITE_OK && checkEndPoint == SQLITE_OK) {
        cout << "Start and end points exists" << endl;
        int addLine = sqlite3_exec(this->DB, QueryInsertLine.c_str(), nullptr, nullptr, &this->zErrMsg);
        if (addLine != SQLITE_OK) {
            cout << "Error: " << zErrMsg << endl;
        } else {
            cout << "Line added successfully" << endl;
        }
        return;
    } else {
        cout << "Error: " << zErrMsg << endl;
        return;
    }

    //get sqlite response for checkStartPoint and checkEndPoint


}