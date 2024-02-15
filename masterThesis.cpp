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
#include "CrossSectionsAssistant.h"

#include <tuple>
#include <cmath>

using namespace std;

int main() {

    cout << "WELCOME IN MES-OS-OIC Project" << "\n" << endl;

    string dateBaseName = "mesosoic_test";

//    cout << "Name your project: ";
//    cin >> dateBaseName;
//    cout << "You entered: " << dateBaseName << endl;

    // This is basic starting section of the program

    dateBaseName = dateBaseName + ".db";

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
    dataBaseCrossSectionsManager.addObjectToDataBase("IPE 100", 1, 10.3 * pow(10.0, -4.0),
                                                     171.000000000000 * pow(10.0, -8));
    dataBaseCrossSectionsManager.addObjectToDataBase("Concrete beam 500x300", 2, 0.15, 0.003125);

//********************************************************************************************************************



    DataBasePointsManager dataBasePointsManager = DataBasePointsManager(dateBaseName);
//    dataBasePointsManager.addObjectToDataBase(69, 420);
//    dataBasePointsManager.addObjectToDataBase(1, 2);

    DataBaseLinesManager dataBaseLinesManager = DataBaseLinesManager(dateBaseName);
//    dataBaseLinesManager.addObjectToDataBase(1, 2);
//    dataBaseLinesManager.addObjectToDataBase(10, 20);

    DataBaseSupportsManager dataBaseSupportsManager = DataBaseSupportsManager(dateBaseName);
//    dataBaseSupportsManager.addObjectToDataBase(1, true, true, true);
//    dataBaseSupportsManager.addObjectToDataBase(2, false, true, false);

    DataBaseNodalLoadsManager dataBaseNodalLoadsManager = DataBaseNodalLoadsManager(dateBaseName);
//    dataBaseNodalLoadsManager.addObjectToDataBase(1, 0, 0, -420.0);
//    dataBaseNodalLoadsManager.addObjectToDataBase(2, 2137.0, 690.0, 0);
//    dataBaseNodalLoadsManager.addObjectToDataBase(2, 0, 0, 694202137.0);

    DataBaseLineLoadsManager dataBaseLineLoadsManager = DataBaseLineLoadsManager(dateBaseName);
//    dataBaseLineLoadsManager.addObjectToDataBase(1, 0, -420.0);
//    dataBaseLineLoadsManager.addObjectToDataBase(20, 20.0, -69.0);
//    dataBaseLineLoadsManager.addObjectToDataBase(2, 69.0, -420.0);


    // ex.1 rod in tension
    //Adding points
    dataBasePointsManager.addObjectToDataBase(0, 0);
    dataBasePointsManager.addObjectToDataBase(0, -500);
    dataBasePointsManager.addObjectToDataBase(0, -1000);
    dataBasePointsManager.addObjectToDataBase(0, -2000);

    dataBasePointsManager.selectAllFromTableByID(TableType::POINTS, 1);
    dataBasePointsManager.selectAllFromTableByID(TableType::POINTS, 2);
    dataBasePointsManager.selectAllFromTableByID(TableType::POINTS, 3);
    dataBasePointsManager.selectAllFromTableByID(TableType::POINTS, 4);

    //Adding lines
    dataBaseLinesManager.addObjectToDataBase(1, 2);
    dataBaseLinesManager.addObjectToDataBase(2, 3);
    dataBaseLinesManager.addObjectToDataBase(3, 4);

    dataBaseLinesManager.selectAllFromTableByID(TableType::LINES, 1);
    dataBaseLinesManager.selectAllFromTableByID(TableType::LINES, 2);
    dataBaseLinesManager.selectAllFromTableByID(TableType::LINES, 3);


    //Adding supports
    dataBaseSupportsManager.addObjectToDataBase(1, true, true, true);

    //Adding materials
    dataBaseMaterialsManager.addObjectToDataBase("Gold", 78.0 * pow(10, 9), 0.42, 19280.0);

    dataBaseMaterialsManager.selectAllFromTableByID(TableType::MATERIALS, 3);

    //Adding cross-sections
    CrossSectionsAssistant crossSectionsAssistant = CrossSectionsAssistant();

    dataBaseCrossSectionsManager.addObjectToDataBase("Concrete beam 200x200", 2,
                                                     crossSectionsAssistant.calculateArea(200, 200),
                                                     crossSectionsAssistant.calculateInertia(200, 200));

    dataBaseCrossSectionsManager.addObjectToDataBase("Golden rod fi400", 3, crossSectionsAssistant.calculateArea(200),
                                                     crossSectionsAssistant.calculateInertia(200));

    //Updating lines with cross-sections
    dataBaseLinesManager.updateObjectInDataBase(TableType::LINES, 1, "cross_section_id", "4");
    dataBaseLinesManager.updateObjectInDataBase(TableType::LINES, 2, "cross_section_id", "4");
    dataBaseLinesManager.updateObjectInDataBase(TableType::LINES, 3, "cross_section_id", "3");

    //Adding nodal loads
    dataBaseNodalLoadsManager.addObjectToDataBase(2, 0, -5, 0);
    dataBaseNodalLoadsManager.addObjectToDataBase(3, 0, -10, 0);
    dataBaseNodalLoadsManager.addObjectToDataBase(4, 0, -20, 0);

    int numberOfFE = dataBaseLinesManager.getNumberOfObjectsInTable(TableType::LINES);

    cout << "Number of FE: " << numberOfFE << endl;

    MatrixHandler K;

    K = MatrixHandler(numberOfFE + 1, numberOfFE + 1); //global matrix of stiffness

    K.printMatrix();

    vector<RodsHandler> rods;
    for (int i = 0; i < numberOfFE; i++) {
        string crossSectionOfFEID = dataBaseLinesManager.selectObjectPropertyByID(TableType::LINES, i + 1,
                                                                                  "cross_section_id");
        cout << "crossSectionOfFEID: " << crossSectionOfFEID << endl;
        string materialOfFEID = dataBaseCrossSectionsManager.selectObjectPropertyByID(TableType::CROSS_SECTIONS,
                                                                                      stoi(crossSectionOfFEID),
                                                                                      "material_id");
        cout << "materialOfFEID: " << materialOfFEID << endl;
        double E = stod(
                dataBaseMaterialsManager.selectObjectPropertyByID(TableType::MATERIALS, stoi(materialOfFEID), "E"));
        cout << "E: " << E << endl;
        double A = stod(dataBaseCrossSectionsManager.selectObjectPropertyByID(TableType::CROSS_SECTIONS,
                                                                              stoi(crossSectionOfFEID), "A"));
        cout << "A: " << A << endl;
        double L = stod(dataBaseLinesManager.selectObjectPropertyByID(TableType::LINES, i + 1, "length"));
        cout << "L: " << L << endl;

        RodsHandler rod = RodsHandler(E, L, A);

        rods.push_back({rod});
    }
    MatrixAggregator matrixAggregator;
    for (int i = 0; i < rods.size(); i++) {

        matrixAggregator.rodsStiffnessMatrixAggregation(K.matrix, rods[i].matrix, i, i + 1);
        K.matrix = matrixAggregator.globalStiffnesMatrixAggregated;
    }

    K.printMatrix();

    // Vector of nodal loads
    vector<double> F;
    for (int i = 1; i < dataBaseNodalLoadsManager.getNumberOfObjectsInTable(TableType::NODAL_LOADS)+1; i++) {
        F.push_back(stod(dataBaseNodalLoadsManager.selectObjectPropertyByID(TableType::NODAL_LOADS, i, "Fz")));
    }
    for (int i = 0; i < F.size(); i++) {
        cout << "F[" << i << "]: " << F[i] << endl;
    }

    // Vector of factors
    vector<double> factors;
    vector<int> nodes;
    for (int i = 1; i < dataBaseNodalLoadsManager.getNumberOfObjectsInTable(TableType::NODAL_LOADS)+1; i++) {
        cout << dataBaseNodalLoadsManager.selectObjectPropertyByID(TableType::NODAL_LOADS, i, "point_id") << endl;
        string pointID = dataBaseNodalLoadsManager.selectObjectPropertyByID(TableType::NODAL_LOADS, i, "point_id");
        cout << "pointID: " << pointID << endl;
        cout << "nodes[" << i << "]: " << nodes[i] << endl;
    }


    //dataBasePointsManager.getNumberOfObjectsInTable(TableType::POINTS);









//    dataBaseNodalLoadsManager.deleteObjectFromDataBase(3);
//
//    dataBaseSupportsManager.deleteObjectFromDataBase(2);
//
//    dataBaseLineLoadsManager.deleteObjectFromDataBase(2);


//    dataBasePointsManager.selectAllFromTableByID(TableType::POINTS, 1);
//    dataBaseLinesManager.selectAllFromTableByID(TableType::LINES, 1);
//    dataBaseMaterialsManager.selectAllFromTableByID(TableType::MATERIALS, 1);
//    dataBaseCrossSectionsManager.selectAllFromTableByID(TableType::CROSS_SECTIONS, 1);
//    dataBaseSupportsManager.selectAllFromTableByID(TableType::SUPPORTS, 1);
//    dataBaseNodalLoadsManager.selectAllFromTableByID(TableType::NODAL_LOADS, 1);
//    dataBaseLineLoadsManager.selectAllFromTableByID(TableType::LINE_LOADS, 1);


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

