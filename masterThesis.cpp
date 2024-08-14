#include <QApplication>
#include "DataBaseManagers/DataBaseManagers.h"
#include "GUI/gui.h"
#include "sqlite/sqlite3.h"
#include "Solver/solver.h"
#include <Eigen/Dense>
#include <cmath>

using namespace Eigen;
using namespace Ui;
using namespace std;
using namespace DataBaseManagers;

int main(int argc, char *argv[])
{
    //Setting DB name
    string dataBaseName = "mesosoic_test";
    dataBaseName = dataBaseName + ".db";

    //Creating DB starter and initialize tables
    DataBaseStarter *dataBaseStarter = new DataBaseStarter(dataBaseName);
    dataBaseStarter->startDateBase();
    dataBaseStarter->createPointsTable();
    dataBaseStarter->createLinesTable();
    dataBaseStarter->createSurfacesTable();
    dataBaseStarter->createMaterialsTable();
    dataBaseStarter->createCrossSectionsTable();
    dataBaseStarter->createSupportsTable();
    dataBaseStarter->createNodalLoadsTable();
    dataBaseStarter->createLineLoadsTable();
    dataBaseStarter->createMeshTable();

    //Creating DB managers and create basic objects
    DataBaseMaterialsManager *materialsManager = new DataBaseMaterialsManager(dataBaseName);
    materialsManager->addObjectToDataBase("Steel", 210.0 * pow(10, 9), 0.3, 7800.0);
    materialsManager->addObjectToDataBase("Concrete C30/37", 32.0 * pow(10, 9), 0.2, 2400.0);

    DataBaseCrossSectionsManager *crossSectionsManager = new DataBaseCrossSectionsManager(dataBaseName);
    crossSectionsManager->addObjectToDataBase("IPE 100", 1, 10.3 * pow(10.0, -4.0), 171.000000000000 * pow(10.0, -8));
    crossSectionsManager->addObjectToDataBase("Concrete beam 500x300", 2, 0.15, 0.003125);

    CrossSectionsAssistant *crossSectionsAssistant = new CrossSectionsAssistant();
    DataBasePointsManager *pointsManager = new DataBasePointsManager(dataBaseName);
    DataBaseLinesManager *linesManager = new DataBaseLinesManager(dataBaseName);
    DataBaseSupportsManager *supportsManager = new DataBaseSupportsManager(dataBaseName);
    DataBaseNodalLoadsManager *nodalLoadsManager = new DataBaseNodalLoadsManager(dataBaseName);
    DataBaseLineLoadsManager *lineLoadsManager = new DataBaseLineLoadsManager(dataBaseName);
    DataBaseMeshManager *meshManager = new DataBaseMeshManager(dataBaseName);

    //Creating and testing DataBaseSolverPreparer
    DataBaseSolverPreparer *solverPreparer = new DataBaseSolverPreparer(dataBaseName);
    std::cout << "Solver Preparer created successfully" << std::endl;
    solverPreparer->fetchAllData();  // Add logging inside this method to trace execution
    std::cout << "fetchAllData completed successfully" << std::endl;

    SolverFEM::Solver solver(solverPreparer);
    solver.solve();

    //Creating GUI
    QApplication app(argc, argv);

    ::Gui mainWindow(pointsManager,
                     linesManager,
                     supportsManager,
                     materialsManager,
                     crossSectionsManager,
                     dataBaseStarter,
                     nodalLoadsManager,
                     lineLoadsManager,
                     meshManager,
                     crossSectionsAssistant);
    mainWindow.show();

    int result = app.exec();

    delete pointsManager;
    delete linesManager;
    delete supportsManager;
    delete materialsManager;
    delete crossSectionsManager;
    delete meshManager;
    delete crossSectionsAssistant;
    delete solverPreparer;

    return result;
}
