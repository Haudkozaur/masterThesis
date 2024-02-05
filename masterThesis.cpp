#include <iostream>
#include <vector>

#include "matrixHandler.h"
#include "RodsHandler.h"
#include "MatrixAggregator.h"
#include "DateBaseStarter.h"
#include "sqlite/sqlite3.h"

#include <tuple>
#include <cmath>

using namespace std;

int main() {
    cout << "WELCOME IN MES-OS-OIC Project\n";

    //eksperymenty z baz¹ danych sqlite
    string dateBaseName = "Project.db";
    DateBaseStarter dateBaseStarter = DateBaseStarter(dateBaseName);
    dateBaseStarter.startDateBase();
    dateBaseStarter.createPointsTable();
    dateBaseStarter.createLinesTable();
    dateBaseStarter.createSurfacesTable();
//
//    for (int i = 0; i < 10; i++) {
//        string sql = "INSERT INTO points (y_cord, z_cord) VALUES (1, 2)";
//        rc = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &zErrMsg);
//        if(rc != SQLITE_OK) {
//            cout << "Error: " << zErrMsg << endl;
//        } else {
//            cout << "Record created successfully" << endl;
//        }
//    }
//    //liczba elementów
//    const int elements = 3;
//    string elementType = "Rod";
//    MatrixHandler K;
//
//    if (elementType == "Rod") {
//        K = MatrixHandler(elements + 1, elements + 1); //globalna macierz sztywnoœci
//    } else {
//        //why? IDK
//        K = MatrixHandler(2, 2);
//    }
//
//    K.printMatrix();
//
//    //Example set of Rods parameters
//    double E1, L1, A1;
//    E1 = 210.0 * pow(10, 9);
//    cout << "E1: " << E1 << endl;
//    L1 = 0.10;
//    cout << "L1: " << L1 << endl;
//    A1 = 10.0 * pow(10, -4);
//    cout << "A1: " << A1 << endl;
//
//    double E2, L2, A2;
//    E2 = 210.0 * pow(10, 9);
//    cout << "E2: " << E2 << endl;
//    L2 = 0.10;
//    cout << "L2: " << L2 << endl;
//    A2 = 10.0 * pow(10, -4);
//    cout << "A2: " << A2 << endl;
//
//    double E3, L3, A3;
//    E3 = 210.0 * pow(10, 9);
//    cout << "E3: " << E3 << endl;
//    L3 = 0.20;
//    cout << "L3: " << L3 << endl;
//    A3 = 10.0 * pow(10, -4);
//    cout << "A3: " << A3 << endl;
//
//    // Create an array of tuples and initialize it
//    vector<tuple<double, double, double>> rodParams = {
//            make_tuple(E1, L1, A1),
//            make_tuple(E2, L2, A2),
//            make_tuple(E3, L3, A3)
//    };
//
//    //RodsHandler rod(E1, L1, A1);
//
//    //agregacja globalnej macierzy sztywnoœci
//    RodsHandler rod;
//
//    for (int i = 0; i < elements; i++) {
//        rod = RodsHandler(get<0>(rodParams[i]), get<1>(rodParams[i]), get<2>(rodParams[i]));
//        rod.rodsStiffnessMatrixAggregation(K.matrix, rod.matrix, i, i + 1);
//        K.matrix = rod.globalStiffnesMatrixAggregated;
//        cout << endl;
//        K.printMatrix();
//        cout << endl;
//    }
    //rod.rodsStiffnessMatrixAggregation(K.matrix, rod.matrix, 0, 1);
    //K.matrix = rod.globalStiffnesMatrixAggregated;
    //K.printMatrix();


}

