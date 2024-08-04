#include <QApplication>
#include "DataBaseManagers/DataBaseManagers.h"
#include "GUI/gui.h"
#include "sqlite/sqlite3.h"
#include <Eigen/Dense>
#include <cmath>

using namespace Eigen;
using namespace Ui;
using namespace std;
using namespace DataBaseManagers;

int main(int argc, char *argv[])
{
    //Setting DB name
    string dateBaseName = "mesosoic_test";
    dateBaseName = dateBaseName + ".db";

    //Creating DB starter and initializate tables
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

    //Creating DB managers and create basic objects
    DataBaseMaterialsManager *materialsManager = new DataBaseMaterialsManager(dateBaseName);
    materialsManager->addObjectToDataBase("Steel", 210.0 * pow(10, 9), 0.3, 7800.0);
    materialsManager->addObjectToDataBase("Concrete C30/37", 32.0 * pow(10, 9), 0.2, 2400.0);

    DataBaseCrossSectionsManager *crossSectionsManager = new DataBaseCrossSectionsManager(
        dateBaseName);
    crossSectionsManager->addObjectToDataBase("IPE 100",
                                              1,
                                              10.3 * pow(10.0, -4.0),
                                              171.000000000000 * pow(10.0, -8));
    crossSectionsManager->addObjectToDataBase("Concrete beam 500x300", 2, 0.15, 0.003125);

    CrossSectionsAssistant *crossSectionsAssistant = new CrossSectionsAssistant();
    DataBasePointsManager *pointsManager = new DataBasePointsManager(dateBaseName);
    DataBaseLinesManager *linesManager = new DataBaseLinesManager(dateBaseName);
    DataBaseSupportsManager *supportsManager = new DataBaseSupportsManager(dateBaseName);
    DataBaseNodalLoadsManager *nodalLoadsManager = new DataBaseNodalLoadsManager(dateBaseName);
    DataBaseLineLoadsManager *lineLoadsManager = new DataBaseLineLoadsManager(dateBaseName);

    //Creating GUI
    QApplication app(argc, argv);

    ::Gui mainWindow(pointsManager,
                   linesManager,
                   supportsManager,
                   materialsManager,
                   crossSectionsManager,
                   dateBaseStarter,
                   nodalLoadsManager,
                   lineLoadsManager,
                   crossSectionsAssistant);
    mainWindow.show();

    int result = app.exec();

    delete pointsManager;
    delete linesManager;
    delete supportsManager;
    delete materialsManager;
    delete crossSectionsManager;
    delete crossSectionsAssistant;

    return result;
}
