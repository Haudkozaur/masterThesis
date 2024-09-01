#include <iostream>
#include "DataBaseStarter.h"
#include "TableTypes.h"

using namespace std;
namespace DataBaseManagers {
DataBaseStarter::DataBaseStarter(const string &dataBaseName) {
    this->dataBaseName = dataBaseName;
    this->dataBaseNameAsChar = dataBaseName.c_str();
    sqlite3 *DB;
}

void DataBaseStarter::startDateBase() {
    sqlite3_open(dataBaseNameAsChar, &DB);
}

void DataBaseStarter::createPointsTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreatePointsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::POINTS) +
                                      " (id INTEGER PRIMARY KEY AUTOINCREMENT, x_cord INTEGER, z_cord INTEGER)";
    int rc = sqlite3_exec(DB,
                          queryToCreatePointsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Points Table created successfully" << endl;
    }
}

void DataBaseStarter::createLinesTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateLinesTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::LINES) +
                                     " (id INTEGER PRIMARY KEY AUTOINCREMENT, start_point INTEGER, end_point INTEGER, cross_section_id, length REAL, inclination_angle REAL,"
                                     "FOREIGN KEY (start_point) REFERENCES points(id),FOREIGN KEY (end_point) REFERENCES points(id), FOREIGN KEY (cross_section_id) REFERENCES cross_sections(id))";
    int rc = sqlite3_exec(DB,
                          queryToCreateLinesTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Lines Table created successfully" << endl;
    }
}

void DataBaseStarter::createSurfacesTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);

    string queryToCreateSurfacesTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::SURFACES) + " ("
                                                                                                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                                                                                "surface_type TEXT, "
                                                                                                                "line1_id INTEGER, "
                                                                                                                "line2_id INTEGER, "
                                                                                                                "line3_id INTEGER, "
                                                                                                                "line4_id INTEGER, "
                                                                                                                "circular_line_id INTEGER, "
                                                                                                                "material_id INTEGER, "
                                                                                                                "thickness INTEGER, "
                                                                                                                "is_opening BOOLEAN, "
                                                                                                                "FOREIGN KEY(line1_id) REFERENCES lines(id), "
                                                                                                                "FOREIGN KEY(line2_id) REFERENCES lines(id), "
                                                                                                                "FOREIGN KEY(line3_id) REFERENCES lines(id), "
                                                                                                                "FOREIGN KEY(line4_id) REFERENCES lines(id), "
                                                                                                                "FOREIGN KEY(circular_line_id) REFERENCES circular_lines(id), "
                                                                                                                "FOREIGN KEY(material_id) REFERENCES materials(id))";

    int rc = sqlite3_exec(DB, queryToCreateSurfacesTable.c_str(), nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Surfaces Table created successfully" << endl;
    }

    sqlite3_close(DB);
}


void DataBaseStarter::createCircularLinesTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);

    string queryToCreateCircularLinesTable = "CREATE TABLE IF NOT EXISTS circular_lines ("
                                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                             "centre_x INTEGER, "
                                             "centre_z INTEGER, "
                                             "diameter INTEGER)";

    int rc = sqlite3_exec(DB, queryToCreateCircularLinesTable.c_str(), nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Circular Lines Table created successfully" << endl;
    }

    sqlite3_close(DB);
}


void DataBaseStarter::createMaterialsTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateMaterialsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::MATERIALS) +
                                         " (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE, E REAL, v REAL, ro REAL)";
    int rc = sqlite3_exec(DB,
                          queryToCreateMaterialsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Materials Table created successfully" << endl;
    }
}

void DataBaseStarter::createCrossSectionsTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateCrossSectionsTable =
        "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::CROSS_SECTIONS) +
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, material_id INTEGER, name TEXT UNIQUE, A REAL, I REAL, FOREIGN KEY (material_id) REFERENCES materials(id))";
    int rc = sqlite3_exec(DB,
                          queryToCreateCrossSectionsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Cross-Sections Table created successfully" << endl;
    }

}

void DataBaseStarter::createSupportsTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateSupportsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::SUPPORTS) +
                                        " (id INTEGER PRIMARY KEY AUTOINCREMENT, point_id INTEGER, ry BOOLEAN, tz BOOLEAN, tx BOOLEAN, FOREIGN KEY (point_id) REFERENCES points(id))";
    int rc = sqlite3_exec(DB,
                          queryToCreateSupportsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Supports Table created successfully" << endl;
    }

}

void DataBaseStarter::createNodalLoadsTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateNodalLoadsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::NODAL_LOADS) +
                                          " (id INTEGER PRIMARY KEY AUTOINCREMENT, point_id INTEGER, My REAL, Fz REAL, Fx REAL, FOREIGN KEY (point_id) REFERENCES points(id))";
    int rc = sqlite3_exec(DB,
                          queryToCreateNodalLoadsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Nodal loads Table created successfully" << endl;
    }

}

void DataBaseStarter::createLineLoadsTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    string queryToCreateLineLoadsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::LINE_LOADS) +
                                         " (id INTEGER PRIMARY KEY AUTOINCREMENT, line_id INTEGER, Fx REAL, Fz REAL, FOREIGN KEY (line_id) REFERENCES lines(id))";
    int rc = sqlite3_exec(DB,
                          queryToCreateLineLoadsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Line loads Table created successfully" << endl;
    }
    cout << "\n";
}

void DataBaseStarter::createMeshTable() {
    sqlite3_open(dataBaseNameAsChar, &DB);
    //query that creates mesg table with id as primary key, line id as foreign key and double node_x and double node_Z
    string queryToCreateMeshTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::MESH) +
                                    " (id INTEGER PRIMARY KEY AUTOINCREMENT, line_id INTEGER, node_x REAL, node_z REAL, FOREIGN KEY (line_id) REFERENCES lines(id))";
    int rc = sqlite3_exec(DB,
                          queryToCreateMeshTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Mesh Table created successfully" << endl;
    }
    cout << "\n";
}
void DataBaseStarter::createFETable()
{
    sqlite3_open(dataBaseNameAsChar, &DB);

    // Create the FE table with additional columns for member_Id, line_Id, and isStart
    string queryToCreateFETable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::NODES) + // Assuming the TableType::NODES key is used for FE table name
                                  "(node_Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "x_cord REAL, z_cord REAL, "
                                  "member_Id INTEGER, line_Id INTEGER, isStart BOOLEAN)";

    int rc = sqlite3_exec(DB,
                          queryToCreateFETable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "FE Table created successfully" << endl;
    }
    cout << "\n";

    sqlite3_close(DB);
}


void DataBaseStarter::createResultsTable()
{
    sqlite3_open(dataBaseNameAsChar, &DB);

    // Create the results table with additional columns for lineId and isStart
    string queryToCreateResultsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::RESULTS) +
                                       "(result_Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                       "line_Id INTEGER, "  // New column for lineId
                                       "member_Id INTEGER, "
                                       "node_Id INTEGER, "
                                       "Nx REAL, Vz REAL, My REAL, deformation REAL, "
                                       "isStart BOOLEAN, "  // New column for isStart
                                       "FOREIGN KEY (node_Id) REFERENCES nodes(node_Id))";

    int rc = sqlite3_exec(DB,
                          queryToCreateResultsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Error: " << zErrMsg << endl;
    } else {
        cout << "Results Table created successfully" << endl;
    }
    cout << "\n";

    sqlite3_close(DB);
}

void DataBaseStarter::createLineSupportsTable()
{
    sqlite3_open(dataBaseNameAsChar, &DB);

    // Create the line supports table with columns for line_id and circular_line_id
    std::string queryToCreateLineSupportsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::LINE_SUPPORTS) +
                                                 "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                 "line_id INTEGER, "
                                                 "circular_line_id INTEGER, "
                                                 "FOREIGN KEY (line_id) REFERENCES lines(id), "
                                                 "FOREIGN KEY (circular_line_id) REFERENCES circular_lines(id))";

    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(DB,
                          queryToCreateLineSupportsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        if (zErrMsg) {
            cout << "Error: " << zErrMsg << endl;
            sqlite3_free(zErrMsg); // Free the error message memory
        }
    } else {
        cout << "Line Supports Table created successfully" << endl;
    }
    cout << "\n";

    sqlite3_close(DB);
}

void DataBaseStarter::createSurfaceSupportsTable()

{
    sqlite3_open(dataBaseNameAsChar, &DB);
    std::string queryToCreateSurfaceSupportsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::SURFACE_SUPPORTS) +
                                                    "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                    "x1 INTEGER, z1 INTEGER, x2 INTEGER, z2 INTEGER)";
    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(DB,
                          queryToCreateSurfaceSupportsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        if (zErrMsg) {
            cout << "Error: " << zErrMsg << endl;
            sqlite3_free(zErrMsg); // Free the error message memory
        }
    } else {
        cout << "Surface Supports Table created successfully" << endl;
    }
    cout << "\n";

    sqlite3_close(DB);
}

void DataBaseStarter::createSlabPointLoadsTable()
{
    sqlite3_open(dataBaseNameAsChar, &DB);
    std::string queryToCreateSlabPointLoadsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::SLAB_POINT_LOADS) +
                                                    "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                    "x1 INTEGER, z1 INTEGER, Fz REAL)";
    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(DB,
                          queryToCreateSlabPointLoadsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        if (zErrMsg) {
            cout << "Error: " << zErrMsg << endl;
            sqlite3_free(zErrMsg); // Free the error message memory
        }
    } else {
        cout << "Slab Point Loads Table created successfully" << endl;
    }
    cout << "\n";

    sqlite3_close(DB);
}

void DataBaseStarter::createSlabLineLoadsTable()
{
    sqlite3_open(dataBaseNameAsChar, &DB);
    std::string queryToCreateSlabLineLoadsTable = "CREATE TABLE IF NOT EXISTS " + tableTypesMap.at(TableType::SLAB_LINE_LOADS) +
                                                   "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                   "x1 INTEGER, z1 INTEGER, x2 INTEGER, z2 INTEGER Fz REAL)";
    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(DB,
                          queryToCreateSlabLineLoadsTable.c_str(),
                          nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        if (zErrMsg) {
            cout << "Error: " << zErrMsg << endl;
            sqlite3_free(zErrMsg); // Free the error message memory
        }
    } else {
        cout << "Slab Line Loads Table created successfully" << endl;
    }
    cout << "\n";

    sqlite3_close(DB);
}


}






