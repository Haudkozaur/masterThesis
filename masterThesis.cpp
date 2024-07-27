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
#include <cmath>
#include <algorithm>
#include <Eigen/Dense>
#include <QApplication>
#include "gui.h"
using namespace Eigen;
using namespace std;

int main(int argc, char *argv[]) {

    string dateBaseName = "mesosoic_test";

    dateBaseName = dateBaseName + ".db";

    DataBaseStarter *dateBaseStarter = new DataBaseStarter(dateBaseName);
    dateBaseStarter->startDateBase();
    dateBaseStarter->createPointsTable();
    dateBaseStarter->createLinesTable();
    dateBaseStarter->createSurfacesTable();
    dateBaseStarter->createMaterialsTable();
    dateBaseStarter->createCrossSectionsTable();
    dateBaseStarter->createSupportsTable();
    dateBaseStarter->createNodalLoadsTable();
    dateBaseStarter->createLineLoadsTable();

    QApplication app(argc, argv);

    // Tworzenie wskaŸników do mened¿erów baz danych
    DataBasePointsManager *pointsManager = new DataBasePointsManager(dateBaseName);
    DataBaseLinesManager *linesManager = new DataBaseLinesManager(dateBaseName);
    DataBaseSupportsManager *supportsManager = new DataBaseSupportsManager(dateBaseName);

    // Tworzenie obiektu Gui i przekazywanie wskaŸników
    Gui mainWindow(pointsManager, linesManager, supportsManager, dateBaseStarter);
    mainWindow.show();

    int result = app.exec();

    delete pointsManager;
    delete linesManager;
    delete supportsManager;



    // cout << "Eigen learning" << "\n" << endl;

    // // define 3x3 matrix of floats and set its entries to zero -explicit declaration
    // int const rows = 3;
    // int const cols = 3;
    // Matrix<float, rows, cols> matrixA;
    // matrixA.setZero();
    // cout << matrixA << endl;

    // Matrix<double, 3, 3> matrixA2;
    // matrixA2.setZero();
    // cout << "\n" << matrixA2 << endl;

    // //insert values into the matrix
    // matrixA(0, 0) = 1;
    // matrixA(0, 1) = 2;
    // matrixA(0, 2) = 3;
    // matrixA(1, 0) = 4;
    // matrixA(1, 1) = 5;
    // matrixA(1, 2) = 6;
    // matrixA(2, 0) = 7;
    // matrixA(2, 1) = 8;
    // matrixA(2, 2) = 9;
    // cout << "\n" << matrixA << endl;

    // //multiply matrix by a scalar
    // Matrix<float, rows, cols> matrixC;
    // matrixC = 2 * matrixA;
    // cout << "\n" << matrixC << endl;

    // //add two matrices
    // Matrix<float, rows, cols> matrixD;
    // matrixD = matrixA + matrixC;
    // cout << "\n" << matrixD << endl;





//
//    //define 3x3 matrix of floats and set its entries to zero -typedef declaration
//    Matrix4d matrixA1;
//    matrixA1.setZero();
//    cout << "\n" << matrixA1 << endl;
//
//     // define a dynamic matrix, explicit declaration
//     Matrix<float, Dynamic, Dynamic> matrixB;
//     matrixB.setZero(10, 10);
//     cout << "\n" << matrixB << endl;
// //
//     //define a dynamic matrix, typedef declaration
//     MatrixXd matrixB1;
//     matrixB1.setZero(5, 5);
//
//    // constructor
//    MatrixXd matrixC(10, 10);
//
//    Matrix4f matrixD;
//    matrixD << 1, 2, 3, 4,
//            5, 6, 7, 8,
//            9, 10, 11, 12,
//            13, 14, 15, 16;
//    cout << endl << matrixD << endl;
//    cout << endl << matrixD(1, 2) << endl;
//
//    Matrix3f matrixD1;
//    matrixD1.resize(3, 3);
//    matrixD1.setZero(3, 3);
//    cout <<endl<< matrixD1;
//
//    // setting matrix entries - two approaches
//    int rowsNumber = 5;
//    int columnNumber= 5;
//
//    // matrix of zeros
//    MatrixXf matrix1zeros;
//    matrix1zeros = MatrixXf::Zero(rowsNumber, columnNumber);
//    cout << "\n \n"<< matrix1zeros<<endl;
//    // another option:
//    MatrixXf matrix1zeros1;
//    matrix1zeros1.setZero(rowsNumber, columnNumber);
//    cout << "\n \n" << matrix1zeros1 << endl;
//
//    //matrix of ones
//    MatrixXf matrix1ones;
//    matrix1ones = MatrixXf::Ones(rowsNumber, columnNumber);
//    cout << "\n \n" << matrix1ones << endl;
//    //another option
//    MatrixXf matrix1ones1;
//    matrix1ones1.setOnes(rowsNumber, columnNumber);
//    cout << "\n \n" << matrix1ones1 << endl;
//
//    //matrix of constants
//    float value = 1.1;
//    MatrixXf matrix1const;
//    matrix1const = MatrixXf::Constant(rowsNumber, columnNumber,value);
//    cout << "\n \n" << matrix1const << endl;
//    //another option
//    MatrixXf matrix1const1;
//    matrix1const1.setConstant(rowsNumber, columnNumber, value);
//    cout << "\n \n" << matrix1const1 << endl;
//
//    //create a diagonal matrix out of a vector
//    Matrix <double, 3, 1> vector1;
//    vector1 << 1, 2, 3;
//    MatrixXd diagonalMatrix;
//    diagonalMatrix = vector1.asDiagonal();
//    cout << " Diagonal matrix is \n\n" << diagonalMatrix;
//
//    cout << "WELCOME IN MES-OS-OIC Project" << "\n" << endl;
//


//    cout << "Name your project: ";
//    cin >> dateBaseName;
//    cout << "You entered: " << dateBaseName << endl;

    // This is basic starting section of the program


//     DataBaseMaterialsManager dataBaseMaterialsManager = DataBaseMaterialsManager(dateBaseName);
//     dataBaseMaterialsManager.addObjectToDataBase("Steel", 210.0 * pow(10, 9), 0.3, 7800.0);
//     dataBaseMaterialsManager.addObjectToDataBase("Concrete C30/37", 32.0 * pow(10, 9), 0.2, 2400.0);

//     DataBaseCrossSectionsManager dataBaseCrossSectionsManager = DataBaseCrossSectionsManager(dateBaseName);
//     dataBaseCrossSectionsManager.addObjectToDataBase("IPE 100", 1, 10.3 * pow(10.0, -4.0),
//                                                      171.000000000000 * pow(10.0, -8));
//     dataBaseCrossSectionsManager.addObjectToDataBase("Concrete beam 500x300", 2, 0.15, 0.003125);

// //********************************************************************************************************************



//     DataBasePointsManager dataBasePointsManager = DataBasePointsManager(dateBaseName);
// //    dataBasePointsManager.addObjectToDataBase(69, 420);
// //    dataBasePointsManager.addObjectToDataBase(1, 2);

//     DataBaseLinesManager dataBaseLinesManager = DataBaseLinesManager(dateBaseName);
// //    dataBaseLinesManager.addObjectToDataBase(1, 2);
// //    dataBaseLinesManager.addObjectToDataBase(10, 20);

//     DataBaseSupportsManager dataBaseSupportsManager = DataBaseSupportsManager(dateBaseName);
// //    dataBaseSupportsManager.addObjectToDataBase(1, true, true, true);
// //    dataBaseSupportsManager.addObjectToDataBase(2, false, true, false);

//     DataBaseNodalLoadsManager dataBaseNodalLoadsManager = DataBaseNodalLoadsManager(dateBaseName);
// //    dataBaseNodalLoadsManager.addObjectToDataBase(1, 0, 0, -420.0);
// //    dataBaseNodalLoadsManager.addObjectToDataBase(2, 2137.0, 690.0, 0);
// //    dataBaseNodalLoadsManager.addObjectToDataBase(2, 0, 0, 694202137.0);

//     DataBaseLineLoadsManager dataBaseLineLoadsManager = DataBaseLineLoadsManager(dateBaseName);
// //    dataBaseLineLoadsManager.addObjectToDataBase(1, 0, -420.0);
// //    dataBaseLineLoadsManager.addObjectToDataBase(20, 20.0, -69.0);
// //    dataBaseLineLoadsManager.addObjectToDataBase(2, 69.0, -420.0);


//     // ex.1 rod in tension
//     //Adding points
//     dataBasePointsManager.addObjectToDataBase(0, 0);
//     dataBasePointsManager.addObjectToDataBase(0, -500);
//     dataBasePointsManager.addObjectToDataBase(0, -1000);
//     dataBasePointsManager.addObjectToDataBase(0, -2000);

//     dataBasePointsManager.selectAllFromTableByID(TableType::POINTS, 1);
//     dataBasePointsManager.selectAllFromTableByID(TableType::POINTS, 2);
//     dataBasePointsManager.selectAllFromTableByID(TableType::POINTS, 3);
//     dataBasePointsManager.selectAllFromTableByID(TableType::POINTS, 4);

//     //Adding lines
//     dataBaseLinesManager.addObjectToDataBase(1, 2);
//     dataBaseLinesManager.addObjectToDataBase(2, 3);
//     dataBaseLinesManager.addObjectToDataBase(3, 4);

//     dataBaseLinesManager.selectAllFromTableByID(TableType::LINES, 1);
//     dataBaseLinesManager.selectAllFromTableByID(TableType::LINES, 2);
//     dataBaseLinesManager.selectAllFromTableByID(TableType::LINES, 3);


//     //Adding supports
//     dataBaseSupportsManager.addObjectToDataBase(1, true, true, true);

//     //Adding materials
//     dataBaseMaterialsManager.addObjectToDataBase("Gold", 78.0 * pow(10, 9), 0.42, 19280.0);

//     dataBaseMaterialsManager.selectAllFromTableByID(TableType::MATERIALS, 3);

//     //Adding cross-sections
//     CrossSectionsAssistant crossSectionsAssistant = CrossSectionsAssistant();

//     dataBaseCrossSectionsManager.addObjectToDataBase("Concrete beam 200x200", 2,
//                                                      crossSectionsAssistant.calculateArea(200, 200),
//                                                      crossSectionsAssistant.calculateInertia(200, 200));

//     dataBaseCrossSectionsManager.addObjectToDataBase("Golden rod fi400", 3, crossSectionsAssistant.calculateArea(200),
//                                                      crossSectionsAssistant.calculateInertia(200));

//     //Updating lines with cross-sections
//     dataBaseLinesManager.updateObjectInDataBase(TableType::LINES, 1, "cross_section_id", "4");
//     dataBaseLinesManager.updateObjectInDataBase(TableType::LINES, 2, "cross_section_id", "4");
//     dataBaseLinesManager.updateObjectInDataBase(TableType::LINES, 3, "cross_section_id", "3");

//     //Adding nodal loads
//     dataBaseNodalLoadsManager.addObjectToDataBase(2, 0, -5, 0);
//     dataBaseNodalLoadsManager.addObjectToDataBase(3, 0, -10, 0);
//     dataBaseNodalLoadsManager.addObjectToDataBase(4, 0, -20, 0);

//     int numberOfFE = dataBaseLinesManager.getNumberOfObjectsInTable(TableType::LINES);

//     cout << "Number of FE: " << numberOfFE << endl;

//     MatrixHandler K;

//     K = MatrixHandler(numberOfFE + 1, numberOfFE + 1); //global matrix of stiffness

//     K.printMatrix();

//     vector<RodsHandler> rods;
//     for (int i = 0; i < numberOfFE; i++) {
//         string crossSectionOfFEID = dataBaseLinesManager.selectObjectPropertyByID(TableType::LINES, i + 1,
//                                                                                   "cross_section_id");
//         cout << "crossSectionOfFEID: " << crossSectionOfFEID << endl;
//         string materialOfFEID = dataBaseCrossSectionsManager.selectObjectPropertyByID(TableType::CROSS_SECTIONS,
//                                                                                       stoi(crossSectionOfFEID),
//                                                                                       "material_id");
//         cout << "materialOfFEID: " << materialOfFEID << endl;
//         double E = stod(
//                 dataBaseMaterialsManager.selectObjectPropertyByID(TableType::MATERIALS, stoi(materialOfFEID), "E"));
//         cout << "E: " << E << endl;
//         double A = stod(dataBaseCrossSectionsManager.selectObjectPropertyByID(TableType::CROSS_SECTIONS,
//                                                                               stoi(crossSectionOfFEID), "A"));
//         cout << "A: " << A << endl;
//         double L = stod(dataBaseLinesManager.selectObjectPropertyByID(TableType::LINES, i + 1, "length"));
//         cout << "L: " << L << endl;

//         RodsHandler rod = RodsHandler(E, L, A);

//         rods.push_back({rod});
//     }
//     MatrixAggregator matrixAggregator;
//     for (int i = 0; i < rods.size(); i++) {

//         matrixAggregator.rodsStiffnessMatrixAggregation(K.matrix, rods[i].matrix, i, i + 1);
//         K.matrix = matrixAggregator.globalStiffnesMatrixAggregated;
//     }

//     K.printMatrix();

//     // Vector of nodal loads



//     // Vector of factors

//     vector<string> nodes;
//     for (int i = 1; i < dataBaseNodalLoadsManager.getNumberOfObjectsInTable(TableType::NODAL_LOADS)+1; i++) {
//         nodes.push_back(dataBaseNodalLoadsManager.selectObjectPropertyByID(TableType::NODAL_LOADS, i, "point_id"));
//         cout << "nodes[" << i << "]: " << nodes[i-1] << endl;

//     }
//     vector<int> nodesWithForce;
//     vector<double> F;
//     for (const auto & node : nodes) {
//         nodesWithForce.push_back(stoi(node));
//     }
//     vector<double> factors;
//     for (int i = 1; i < dataBasePointsManager.getNumberOfObjectsInTable(TableType::POINTS)+1; i++) {
//         if (nodesWithForce.end() != std::find(nodesWithForce.begin(), nodesWithForce.end(), i)) {
//             factors.push_back(1);
//             string z = dataBaseNodalLoadsManager.selectObjectPropertyByID(TableType::NODAL_LOADS, i, "Fz");
//             double zdouble = stod(z);
//             F.push_back(zdouble);
//         } else {
//             factors.push_back(0);
//             F.push_back(0);
//         }
//     }
//     for (int i = 0; i < factors.size(); i++) {
//         cout << "factors[" << i << "]: " << factors[i] << endl;
//     }


//     MatrixHandler Keff = K;
//     Keff.multiplyByVector(factors);
//     Keff.multiplyByVector(factors);
//     Keff.transposeMatrix();
// //    FMatrix.transposeMatrix();
// //    Keff = Keff*FMatrix;
//     Keff.printMatrix();

//     // Effective stiffness matrix
// //    MatrixHandler Keff;
// //    Keff = K.

//     //Solve the system of equations
//     vector<double> U;

//     U = Keff.solveLinearSystem(F);
//     for (int i = 0; i < U.size(); i++) {
//         cout << "U[" << i << "]: " << U[i] << endl;
//     }







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

    return result;
}
