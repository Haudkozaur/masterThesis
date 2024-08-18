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
    dataBaseStarter->createFETable();
    dataBaseStarter->createResultsTable();

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
    DataBaseSolverPreparer *solverPreparer = new DataBaseSolverPreparer(dataBaseName);
    DataBaseResultsManager *resultsManager = new DataBaseResultsManager(dataBaseName);

    //Creating and testing DataBaseSolverPreparer

    //Creating GUI
    QApplication app(argc, argv);  // Use the correct variable name here
/*    QString darkStyleSheet = R"(
        QWidget {
            background-color: #2e2e2e;
            color: #ffffff;
        }

        QMenuBar {
            background-color: #3e3e3e;
        }

        QMenuBar::item {
            background-color: #3e3e3e;
            color: #ffffff;
        }

        QMenuBar::item:selected {
            background-color: #4e4e4e;
        }

        QMenu {
            background-color: #3e3e3e;
            color: #ffffff;
        }

        QMenu::item:selected {
            background-color: #4e4e4e;
        }

        QToolBar {
            background-color: #3e3e3e;
        }

        QPushButton {
            background-color: #4e4e4e;
            color: #ffffff;
            border: 1px solid #5e5e5e;
        }

        QPushButton:hover {
            background-color: #5e5e5e;
        }

        QPushButton:pressed {
            background-color: #6e6e6e;
        }

        QLineEdit {
            background-color: #4e4e4e;
            color: #ffffff;
            border: 1px solid #5e5e5e;
        }

        QTextEdit {
            background-color: #4e4e4e;
            color: #ffffff;
            border: 1px solid #5e5e5e;
        }

        QTableView {
            background-color: #3e3e3e;
            color: #ffffff;
            gridline-color: #5e5e5e;
        }

        QHeaderView::section {
            background-color: #4e4e4e;
            color: #ffffff;
        }

        QScrollBar:vertical {
            background-color: #3e3e3e;
            width: 16px;
        }

        QScrollBar::handle:vertical {
            background-color: #5e5e5e;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            background-color: #4e4e4e;
        }

        QScrollBar:horizontal {
            background-color: #3e3e3e;
            height: 16px;
        }

        QScrollBar::handle:horizontal {
            background-color: #5e5e5e;
        }

        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            background-color: #4e4e4e;
        }
    )";

    // Apply the dark theme stylesheet
    app.setStyleSheet(darkStyleSheet);*/  // Correct variable name used here
    ::Gui mainWindow(pointsManager,
                     linesManager,
                     supportsManager,
                     materialsManager,
                     crossSectionsManager,
                     dataBaseStarter,
                     nodalLoadsManager,
                     lineLoadsManager,
                     meshManager,
                     crossSectionsAssistant,
                     solverPreparer,
                     resultsManager);
    mainWindow.show();

    int result = app.exec();

    delete dataBaseStarter;
    delete pointsManager;
    delete linesManager;
    delete supportsManager;
    delete materialsManager;
    delete crossSectionsManager;
    delete meshManager;
    delete crossSectionsAssistant;
    delete solverPreparer;
    delete nodalLoadsManager;
    delete lineLoadsManager;
    delete resultsManager;

    return result;
}

