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


    DataBaseMaterialsManager dataBaseMaterialsManager = DataBaseMaterialsManager(dateBaseName);
    dataBaseMaterialsManager.addObjectToDataBase("Steel", 210.0 * pow(10, 9), 0.3, 7800.0);
    dataBaseMaterialsManager.addObjectToDataBase("Concrete C30/37", 32.0 * pow(10, 9), 0.2, 2400.0);

    DataBaseCrossSectionsManager dataBaseCrossSectionsManager = DataBaseCrossSectionsManager(dateBaseName);
    dataBaseCrossSectionsManager.addObjectToDataBase("IPE 100", 1, 10.3*pow(10.0,-4.0), 171.000000000000*pow(10.0,-8));
    dataBaseCrossSectionsManager.addObjectToDataBase("Concrete beam 500x300", 2, 0.15, 0.003125);

    DataBasePointsManager dateBasePointsManager = DataBasePointsManager(dateBaseName);
    dateBasePointsManager.AddObjectToDataBase(69, 420);
    dateBasePointsManager.AddObjectToDataBase(1, 2);

    DataBaseLinesManager dateBaseLinesManager = DataBaseLinesManager(dateBaseName);
    dateBaseLinesManager.addObjectToDataBase(1, 2);
    dateBaseLinesManager.addObjectToDataBase(10, 20);

    DataBaseSupportsManager dataBaseSupportsManager = DataBaseSupportsManager(dateBaseName);
    dataBaseSupportsManager.addObjectToDataBase(1, true, true, true);
    dataBaseSupportsManager.addObjectToDataBase(2, false, true, false);

    DataBaseNodalLoadsManager dataBaseNodalLoadsManager = DataBaseNodalLoadsManager(dateBaseName);
    dataBaseNodalLoadsManager.addObjectToDataBase(1, 0, 0, -420);
    dataBaseNodalLoadsManager.addObjectToDataBase(2, 2137, 690, 0);
    dataBaseNodalLoadsManager.addObjectToDataBase(2, 0, 0, 694202137);

    dataBaseNodalLoadsManager.deleteObjectFromDataBase(3);

    dataBaseSupportsManager.deleteObjectFromDataBase(1);

    //dateBasePointsManager.deleteObjectFromDataBase(1);

    //dateBaseLinesManager.deleteObjectFromDataBase(1);


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

