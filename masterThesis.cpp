#include <iostream>
#include <vector>
#include "matrixHandler.h"
#include "RodsHandler.h"
#include "MatrixAggregator.h"
#include "DataBaseStarter.h"
#include "sqlite/sqlite3.h"
#include "DataBasePointsManager.h"
#include "DataBaseModelObjectsManager.h"
#include "DataBaseLinesManager.h"
#include "DataBaseMaterialsManager.h"
#include "DataBaseCrossSectionsManager.h"
#include "DataBaseSupportsManager.h"
#include "DataBaseNodalLoadsManager.h"
#include "DataBaseLineLoadsManager.h"

#include <tuple>
#include <cmath>

using namespace std;

int main() {
    cout << "WELCOME IN MES-OS-OIC Project" << "\n" << endl;

    //eksperymenty z baz¹ danych sqlite

    string dateBaseName = "Project.db";

    DataBaseStarter dateBaseStarter = DataBaseStarter(dateBaseName);
    dateBaseStarter.startDateBase();
    dateBaseStarter.createPointsTable();
    dateBaseStarter.createLinesTable();
    dateBaseStarter.createSurfacesTable();
    dateBaseStarter.createMaterialsTable();
    dateBaseStarter.createCrossSectionsTable();
    dateBaseStarter.createSupportsTable();
    dateBaseStarter.createNodalLoadsTable();
    dateBaseStarter.createLineLoadsTable();

    DataBaseMaterialsManager dataBaseMaterialsManager = DataBaseMaterialsManager(dateBaseName);
    dataBaseMaterialsManager.addObjectToDataBase("Steel", 210.0 * pow(10, 9), 0.3, 7800.0);
    dataBaseMaterialsManager.addObjectToDataBase("Concrete C30/37", 32.0 * pow(10, 9), 0.2, 2400.0);

    DataBaseCrossSectionsManager dataBaseCrossSectionsManager = DataBaseCrossSectionsManager(dateBaseName);
    dataBaseCrossSectionsManager.addObjectToDataBase("IPE 100", 1, 10.3*pow(10.0,-4.0), 171.000000000000*pow(10.0,-8));
    dataBaseCrossSectionsManager.addObjectToDataBase("Concrete beam 500x300", 2, 0.15, 0.003125);

    DataBasePointsManager dataBasePointsManager = DataBasePointsManager(dateBaseName);
    dataBasePointsManager.AddObjectToDataBase(69, 420);
    dataBasePointsManager.AddObjectToDataBase(1, 2);

    DataBaseLinesManager dataBaseLinesManager = DataBaseLinesManager(dateBaseName);
    dataBaseLinesManager.addObjectToDataBase(1, 2);
    dataBaseLinesManager.addObjectToDataBase(10, 20);

    DataBaseSupportsManager dataBaseSupportsManager = DataBaseSupportsManager(dateBaseName);
    dataBaseSupportsManager.addObjectToDataBase(1, true, true, true);
    dataBaseSupportsManager.addObjectToDataBase(2, false, true, false);

    DataBaseNodalLoadsManager dataBaseNodalLoadsManager = DataBaseNodalLoadsManager(dateBaseName);
    dataBaseNodalLoadsManager.addObjectToDataBase(1, 0, 0, -420.0);
    dataBaseNodalLoadsManager.addObjectToDataBase(2, 2137.0, 690.0, 0);
    dataBaseNodalLoadsManager.addObjectToDataBase(2, 0, 0, 694202137.0);

    DataBaseLineLoadsManager dataBaseLineLoadsManager = DataBaseLineLoadsManager(dateBaseName);
    dataBaseLineLoadsManager.addObjectToDataBase(1, 0, -420.0);
    dataBaseLineLoadsManager.addObjectToDataBase(20, 20.0, -69.0);
    dataBaseLineLoadsManager.addObjectToDataBase(2, 69.0, -420.0);

    dataBaseNodalLoadsManager.deleteObjectFromDataBase(3);

    dataBaseSupportsManager.deleteObjectFromDataBase(2);

    dataBaseLineLoadsManager.deleteObjectFromDataBase(2);


    dataBasePointsManager.selectAllFromTableByID(TableType::POINTS, 1);
    dataBaseLinesManager.selectAllFromTableByID(TableType::LINES, 1);
    dataBaseMaterialsManager.selectAllFromTableByID(TableType::MATERIALS, 1);
    dataBaseCrossSectionsManager.selectAllFromTableByID(TableType::CROSS_SECTIONS, 1);
    dataBaseSupportsManager.selectAllFromTableByID(TableType::SUPPORTS, 1);
    dataBaseNodalLoadsManager.selectAllFromTableByID(TableType::NODAL_LOADS, 1);
    dataBaseLineLoadsManager.selectAllFromTableByID(TableType::LINE_LOADS, 1);


    //dateBasePointsManager.deleteObjectFromDataBase(1);

    //dataBaseLinesManager.deleteObjectFromDataBase(1);


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

