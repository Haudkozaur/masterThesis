#include <QApplication>
#include "DataBaseManagers/DataBaseManagers.h"
#include "GUI/gui.h"
#include "GUI/StartWindow.h"
#include "SlabGUI/SlabGUI.h"
#include "sqlite/sqlite3.h"
#include "Solver/solver.h"
#include <Eigen/Dense>
#include <cmath>
#include <memory>

using namespace Eigen;
using namespace Ui;
using namespace std;
using namespace DataBaseManagers;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ::StartWindow startWindow;
    if (startWindow.exec() != QDialog::Accepted) {
        return 0;
    }

    string dataBaseName;
    unique_ptr<DataBaseStarter> dataBaseStarter;

    switch (startWindow.selectedModule()) {
    case ::StartWindow::Frame2D: {
        //Setting DB name


        unique_ptr<DataBaseMaterialsManager> materialsManager;
        unique_ptr<DataBaseCrossSectionsManager> crossSectionsManager;
        unique_ptr<CrossSectionsAssistant> crossSectionsAssistant;
        unique_ptr<DataBasePointsManager> pointsManager;
        unique_ptr<DataBaseLinesManager> linesManager;
        unique_ptr<DataBaseSupportsManager> supportsManager;
        unique_ptr<DataBaseNodalLoadsManager> nodalLoadsManager;
        unique_ptr<DataBaseLineLoadsManager> lineLoadsManager;
        unique_ptr<DataBaseMeshManager> meshManager;
        unique_ptr<DataBaseSolverPreparer> solverPreparer;
        unique_ptr<DataBaseResultsManager> resultsManager;
        dataBaseName = "mesosoic_test";
        dataBaseName += ".db";

        //Creating DB starter and initialize tables
        dataBaseStarter = make_unique<DataBaseStarter>(dataBaseName);
        dataBaseStarter->startDateBase();
        dataBaseStarter->createPointsTable();
        dataBaseStarter->createLinesTable();
        //dataBaseStarter->createSurfacesTable();
        dataBaseStarter->createMaterialsTable();
        dataBaseStarter->createCrossSectionsTable();
        dataBaseStarter->createSupportsTable();
        dataBaseStarter->createNodalLoadsTable();
        dataBaseStarter->createLineLoadsTable();
        dataBaseStarter->createMeshTable();
        dataBaseStarter->createFETable();
        dataBaseStarter->createResultsTable();

        //Creating DB managers and create basic objects
        materialsManager = make_unique<DataBaseMaterialsManager>(dataBaseName);
        materialsManager->addObjectToDataBase("Steel", 210.0 * pow(10, 9), 0.3, 7800.0);
        materialsManager->addObjectToDataBase("Concrete C30/37", 32.0 * pow(10, 9), 0.2, 2400.0);

        crossSectionsManager = make_unique<DataBaseCrossSectionsManager>(dataBaseName);
        crossSectionsManager->addObjectToDataBase("IPE 100", 1, 10.3 * pow(10.0, -4.0), 171.000000000000 * pow(10.0, -8));
        crossSectionsManager->addObjectToDataBase("Concrete beam 500x300", 2, 0.15, 0.003125);

        crossSectionsAssistant = make_unique<CrossSectionsAssistant>();
        pointsManager = make_unique<DataBasePointsManager>(dataBaseName);
        linesManager = make_unique<DataBaseLinesManager>(dataBaseName);
        supportsManager = make_unique<DataBaseSupportsManager>(dataBaseName);
        nodalLoadsManager = make_unique<DataBaseNodalLoadsManager>(dataBaseName);
        lineLoadsManager = make_unique<DataBaseLineLoadsManager>(dataBaseName);
        meshManager = make_unique<DataBaseMeshManager>(dataBaseName);
        solverPreparer = make_unique<DataBaseSolverPreparer>(dataBaseName);
        resultsManager = make_unique<DataBaseResultsManager>(dataBaseName);

        //Creating GUI
        ::Gui mainWindow(pointsManager.get(),
                         linesManager.get(),
                         supportsManager.get(),
                         materialsManager.get(),
                         crossSectionsManager.get(),
                         dataBaseStarter.get(),
                         nodalLoadsManager.get(),
                         lineLoadsManager.get(),
                         meshManager.get(),
                         crossSectionsAssistant.get(),
                         solverPreparer.get(),
                         resultsManager.get());
        mainWindow.show();

        int result = app.exec();

        return result;
    }

    case ::StartWindow::Plate: {

        unique_ptr<DataBasePointsManager> pointsManager;
        unique_ptr<DataBaseLinesManager> linesManager;
        unique_ptr<DataBaseMaterialsManager> materialsManager;
        unique_ptr<DataBaseSurfacesManager> surfacesManager;
        unique_ptr<DataBaseCircularLinesManager> circularLinesManager;
        unique_ptr<DataBaseLineSupportsManager> lineSupportsManager;
        unique_ptr<DataBaseSurfaceSupportsManager> surfaceSupportsManager;
        unique_ptr<DataBaseSlabPointLoadManager> slabPointLoadsManager;
        unique_ptr<DataBaseSlabLineLoadsManager> slabLineLoadsManager;
        dataBaseName = "mesosoic_slab_test";
        dataBaseName += ".db";

        dataBaseStarter = make_unique<DataBaseStarter>(dataBaseName);
        dataBaseStarter->startDateBase();
        dataBaseStarter->createPointsTable();
        dataBaseStarter->createLinesTable();
        dataBaseStarter->createCircularLinesTable();
        dataBaseStarter->createSurfacesTable();
        dataBaseStarter->createMaterialsTable();
        dataBaseStarter->createLineSupportsTable();
        dataBaseStarter->createSurfaceSupportsTable();
        dataBaseStarter->createSlabPointLoadsTable();
        dataBaseStarter->createSlabLineLoadsTable();


        pointsManager = make_unique<DataBasePointsManager>(dataBaseName);
        linesManager = make_unique<DataBaseLinesManager>(dataBaseName);

        materialsManager = make_unique<DataBaseMaterialsManager>(dataBaseName);
        materialsManager->addObjectToDataBase("Steel", 210.0 * pow(10, 9), 0.3, 7800.0);
        materialsManager->addObjectToDataBase("Concrete C30/37", 32.0 * pow(10, 9), 0.2, 2400.0);

        surfacesManager = make_unique<DataBaseSurfacesManager>(dataBaseName);
        circularLinesManager = make_unique<DataBaseCircularLinesManager>(dataBaseName);
        lineSupportsManager = make_unique<DataBaseLineSupportsManager>(dataBaseName);
        surfaceSupportsManager = make_unique<DataBaseSurfaceSupportsManager>(dataBaseName);
        slabPointLoadsManager = make_unique<DataBaseSlabPointLoadManager>(dataBaseName);
        slabLineLoadsManager = make_unique<DataBaseSlabLineLoadsManager>(dataBaseName);

        ::SlabGUI mainWindow(pointsManager.get(), linesManager.get(), materialsManager.get(), surfacesManager.get(), circularLinesManager.get(),
                             lineSupportsManager.get(),  surfaceSupportsManager.get(),slabPointLoadsManager.get(),slabLineLoadsManager.get(), dataBaseStarter.get());
        mainWindow.show();
        int result = app.exec();
        return result;

    }

    default:
        return 0;
    }

    return 0;
}
