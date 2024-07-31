#include "gui.h"
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTransform>
#include <QUiLoader>
#include <QWheelEvent>
#include <QXmlStreamReader>
#include "addboundariesdialog.h"
#include "addcrosssectiondialog.h"
#include "addlinedialog.h"
#include "addlineloaddialog.h"
#include "addmaterialdialog.h"
#include "addpointappliedforce.h"
#include "addpointdialog.h"
#include "deleteobjectdialog.h"
#include "editobjectdialog.h"
#include "setpropertiesdialog.h"
#include "ui_gui.h"
#include <cmath>

Gui::Gui(DataBasePointsManager *pointsManager,
         DataBaseLinesManager *linesManager,
         DataBaseSupportsManager *supportsManager,
         DataBaseMaterialsManager *materialsManager,
         DataBaseCrossSectionsManager *crossSectionsManager,
         DataBaseStarter *starter,
         DataBaseNodalLoadsManager *nodalLoadsManager,
         DataBaseLineLoadsManager *lineLoadsManager,
         CrossSectionsAssistant *crossSectionsAssistant,
         QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Gui)
    , dataBasePointsManager(pointsManager)
    , dataBaseLinesManager(linesManager)
    , dataBaseSupportsManager(supportsManager)
    , dataBaseStarter(starter)
    , dataBaseMaterialsManager(materialsManager)
    , dataBaseCrossSectionsManager(crossSectionsManager)
    , dataBaseNodalLoadsManager(nodalLoadsManager)
    , dataBaseLineLoadsManager(lineLoadsManager)
    , crossSectionsAssistant(crossSectionsAssistant)
    , xCoordinate(0)
    , zCoordinate(0)
    , scaleFactor(1.0)
    , isDragging(false)

{
    ui->setupUi(this);

    // Calculate initial translation offset to center the origin
    translationOffset = QPoint(width() / 2, height() / 2);
    connect(ui->modelPhaseComboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(onComboBoxIndexChanged(int)));

    // Setting background style for button containers
    QWidget *leftButtonContainer = ui->leftverticalLayout->parentWidget();
    QWidget *upperButtonContainer = ui->uphorizontalLayout->parentWidget();
    if (leftButtonContainer && upperButtonContainer) {
        QString backgroundStyle = "background-color: rgba(255, 255, 255, 150);";
        leftButtonContainer->setStyleSheet(backgroundStyle);
        upperButtonContainer->setStyleSheet(backgroundStyle);
    }
}
Gui::~Gui()
{
    delete ui;
    // Cleanup ...
}

void Gui::on_layoutAddPointButton_clicked()
{
    Gui::on_addPointButton_clicked();
}
void Gui::on_layoutAddLineButton_clicked()
{
    Gui::on_addLineButton_clicked();
}
void Gui::on_layoutAddSupportButton_clicked()
{
    Gui::on_addSupportButton_clicked();
}
void Gui::on_addMaterialButton_clicked()
{
    cout << "Material button clicked" << endl;
    AddMaterialDialog *dialog = new AddMaterialDialog(this);
    dialog->moveToBottomLeft();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &AddMaterialDialog::accepted, this, [this, dialog]() {
        string materialName = dialog->getMaterialName();
        double youngModulus = dialog->getYoungModulus();
        double poissonCoefficient = dialog->getPoissonCoefficient();
        double density = dialog->getDensity();

        dataBaseMaterialsManager->addObjectToDataBase(materialName,
                                                      youngModulus * std::pow(10, 9),
                                                      poissonCoefficient,
                                                      density);

        dataBaseMaterialsManager->iterateOverTable();

        Gui::on_addMaterialButton_clicked();
    });

    connect(dialog, &AddMaterialDialog::rejected, dialog, &AddMaterialDialog::deleteLater);
    dialog->show();
}
void Gui::on_addCrossSectionButton_clicked()
{
    qDebug() << "AddCrossSectionDialog opened";
    // Create an instance of the edit dialog
    AddCrossSectionDialog *dialog = new AddCrossSectionDialog(this);

    dataBaseMaterialsManager->iterateOverTable();
    dialog->setMaterials(dataBaseMaterialsManager->getMaterialsMap());
    dialog->updateMaterialsList();

    // Initialize the dialog with the last selected type
    dialog->initializeWithType(csLastSelectedType);

    // Move the dialog to the bottom left of the main window
    dialog->moveToBottomLeft();

    // Set the attribute to delete the dialog on close
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Handle user acceptance
    connect(dialog, &AddCrossSectionDialog::accepted, this, [this, dialog]() {
        qDebug() << "AddCrossSectionDialog accepted";
        QString selectedType = dialog->getSelectedObjectType();

        // Handle editing based on the selected type
        int materialId = dialog->getMaterialId();
        if (selectedType == "General") {
            string name = dialog->getName();
            double area = dialog->getArea();
            double innertia = dialog->getInnertia();

            dataBaseCrossSectionsManager->addObjectToDataBase(name, materialId, area, innertia);
        } else if (selectedType == "Rectangular") {
            string name = dialog->getNameForRect();
            int height = dialog->getHeight();
            int width = dialog->getWidth();
            dataBaseCrossSectionsManager
                ->addObjectToDataBase(name,
                                      materialId,
                                      crossSectionsAssistant->calculateArea(height, width),
                                      crossSectionsAssistant->calculateInertia(height, width));
        } else if (selectedType == "Circular") {
            string name = dialog->getNameForCirc();
            int r = dialog->getRadius();
            dataBaseCrossSectionsManager
                ->addObjectToDataBase(name,
                                      materialId,
                                      crossSectionsAssistant->calculateArea(r),
                                      crossSectionsAssistant->calculateInertia(r));
        }

        // Refresh the UI
        on_refreshButton_clicked();

        // Update the last selected type
        csLastSelectedType = selectedType;
        qDebug() << "Calling on_addCrossSectionButton_clicked() recursively";
        //Gui::on_addCrossSectionButton_clicked();
    });
    connect(dialog, &AddCrossSectionDialog::rejected, dialog, &AddCrossSectionDialog::deleteLater);
    // Show the dialog non-modally
    dialog->show();
}

void Gui::on_openCrossSectionManagerButton()
{
    cout << "Cross section manager button clicked" << endl;
}
void Gui::on_setPropertiesButton_clicked()
{
    SetPropertiesDialog *dialog = new SetPropertiesDialog(this);

    dataBaseCrossSectionsManager->iterateOverTable();
    dialog->setCrossSections(dataBaseCrossSectionsManager->getCrossSectionsMap());
    dialog->updateCrossSectionsList();
    dialog->initializeWithType(setPropLastSelectedType);

    dialog->moveToBottomLeft();

    dialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &SetPropertiesDialog::accepted, this, [this, dialog]() {
        QString selectedType = dialog->getSelectedObjectType();
        int lineId = dialog->getLineId();
        int crossSectionId = dialog->getCrossSectionId();
        dataBaseLinesManager->editLine(lineId, crossSectionId);

        on_refreshButton_clicked();
        setPropLastSelectedType = selectedType;
        Gui::on_setPropertiesButton_clicked();
    });

    connect(dialog, &SetPropertiesDialog::rejected, dialog, &SetPropertiesDialog::deleteLater);
    dialog->show();

    cout << "Properties button clicked" << endl;
}

void Gui::on_addPointAppliedForceButton_clicked()
{
    cout << "Point force button clicked" << endl;
    AddPointAppliedForce *dialog = new AddPointAppliedForce(this);

    dialog->moveToBottomLeft();

    dialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &AddPointAppliedForce::accepted, this, [this, dialog]() {
        int pointId = dialog->getPointId();
        double Fx = dialog->getFx();
        double Fz = dialog->getFz();
        double My = dialog->getMy();
        dataBaseNodalLoadsManager->addObjectToDataBase(pointId, My, Fz, Fx);

        Gui::on_refreshButton_clicked();
        Gui::on_addPointAppliedForceButton_clicked();

        dataBaseNodalLoadsManager->iterateOverTable();
        nodalLoads.clear();

        for (const auto &nodalLoad : dataBaseNodalLoadsManager->getNodalLoadsMap()) {
            nodalLoads.push_back({std::get<0>(nodalLoad.second), std::get<1>(nodalLoad.second), std::get<2>(nodalLoad.second), std::get<3>(nodalLoad.second)});
        }
    });
    connect(dialog, &SetPropertiesDialog::rejected, dialog, &SetPropertiesDialog::deleteLater);
    dialog->show();
}

void Gui::on_addLineLoadButton_clicked()
{
    cout << "Line load button clicked" << endl;

    AddLineLoadDialog *dialog = new AddLineLoadDialog(this);

    dialog->moveToBottomLeft();

    dialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &AddLineLoadDialog::accepted, this, [this, dialog]() {
        int lineId = dialog->getLineId();
        double Fx = dialog->getFx();
        double Fz = dialog->getFz();
        dataBaseLineLoadsManager->addObjectToDataBase(lineId,Fx,Fz);
        cout << "Line load added" << endl;
        Gui::on_refreshButton_clicked();
        Gui::on_addLineLoadButton_clicked();
    });
    connect(dialog, &AddLineLoadDialog::rejected, dialog, &AddLineLoadDialog::deleteLater);
    dialog->show();
}


void Gui::on_openLoadsManagerButton_clicked()
{
    cout << "Loads manager button clicked" << endl;
}

void Gui::onComboBoxIndexChanged(int index)
{
    switch (index) {
    case 0:
        loadStaticSchemeLayout();
        break;
    case 1:
        loadPropertiesLayout();
        break;
    case 2:
        loadLoadsLayout();
        break;
    default:
        break;
    }
}
void Gui::loadPropertiesLayout()
{
    loadLayoutFromFile(":/ui/properties_layout.ui");
    // Znajdź przycisk w załadowanym layoutcie
    addMaterialButton = findChild<QPushButton *>("addMaterialButton");
    if (addMaterialButton) {
        connect(addMaterialButton, &QPushButton::clicked, this, &Gui::on_addMaterialButton_clicked);
    } else {
        qWarning() << "Button 'addMaterialButton' not found!";
    }
    addCrossSectionButton = findChild<QPushButton *>("addCrossSectionButton");
    if (addCrossSectionButton) {
        connect(addCrossSectionButton,
                &QPushButton::clicked,
                this,
                &Gui::on_addCrossSectionButton_clicked);
    } else {
        qWarning() << "Button 'addCrossSectionButton' not found!";
    }
    setPropertiesButton = findChild<QPushButton *>("setPropertiesButton");
    if (setPropertiesButton) {
        connect(setPropertiesButton,
                &QPushButton::clicked,
                this,
                &Gui::on_setPropertiesButton_clicked);
    } else {
        qWarning() << "Button 'setPropertiesButton' not found!";
    }
    openCrossSectionManagerButton = findChild<QPushButton *>("openCrossSectionManagerButton");
    if (openCrossSectionManagerButton) {
        connect(openCrossSectionManagerButton,
                &QPushButton::clicked,
                this,
                &Gui::on_openCrossSectionManagerButton);
    } else {
        qWarning() << "Button 'openCrossSectionManagerButton' not found!";
    }
}

void Gui::loadStaticSchemeLayout()
{
    loadLayoutFromFile(":/ui/static_scheme_layout.ui");

    // Znajdź przycisk w załadowanym layoutcie
    layoutAddPointButton = findChild<QPushButton *>("layoutAddPointButton");
    if (layoutAddPointButton) {
        connect(layoutAddPointButton,
                &QPushButton::clicked,
                this,
                &Gui::on_layoutAddPointButton_clicked);
    } else {
        qWarning() << "Button 'layoutAddPointButton' not found!";
    }
    layoutAddLineButton = findChild<QPushButton *>("layoutAddLineButton");
    if (layoutAddLineButton) {
        connect(layoutAddLineButton,
                &QPushButton::clicked,
                this,
                &Gui::on_layoutAddLineButton_clicked);
    } else {
        qWarning() << "Button 'layoutAddLineButton' not found!";
    }
    layoutAddSupportButton = findChild<QPushButton *>("layoutAddSupportButton");
    if (layoutAddSupportButton) {
        connect(layoutAddSupportButton,
                &QPushButton::clicked,
                this,
                &Gui::on_layoutAddSupportButton_clicked);
    } else {
        qWarning() << "Button 'layoutAddSupportButton' not found!";
    }
}
void Gui::loadLoadsLayout()
{
    loadLayoutFromFile(":/ui/loads_layout.ui");

    // Znajdź przycisk w załadowanym layoutcie
    addPointAppliedForceButton = findChild<QPushButton *>("addPointAppliedForceButton");
    if (addPointAppliedForceButton) {
        connect(addPointAppliedForceButton,
                &QPushButton::clicked,
                this,
                &Gui::on_addPointAppliedForceButton_clicked);
    } else {
        qWarning() << "Button 'addPointAppliedForceButton' not found!";
    }
    addLineLoadButton = findChild<QPushButton *>("addLineLoadButton");
    if (addLineLoadButton) {
        connect(addLineLoadButton, &QPushButton::clicked, this, &Gui::on_addLineLoadButton_clicked);
    } else {
        qWarning() << "Button 'addLineLoadButton' not found!";
    }
    openLoadsManagerButton = findChild<QPushButton *>("openLoadsManagerButton");
    if (openLoadsManagerButton) {
        connect(openLoadsManagerButton,
                &QPushButton::clicked,
                this,
                &Gui::on_openLoadsManagerButton_clicked);
    } else {
        qWarning() << "Button 'openLoadsManagerButton' not found!";
    }
}
void Gui::loadLayoutFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open file:" << fileName;
        return;
    }

    QUiLoader loader;
    QWidget *newWidget = loader.load(&file, this);
    file.close();

    if (!newWidget) {
        qWarning() << "Failed to load layout from file:" << fileName;
        return;
    }

    // Find the QVBoxLayout by searching the newWidget
    QVBoxLayout *newLayout = nullptr;
    QList<QObject *> children = newWidget->findChildren<QObject *>();
    for (QObject *child : children) {
        if (QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(child)) {
            newLayout = layout;
            break;
        }
    }

    if (newLayout) {
        clearLayout(ui->leftverticalLayout);

        // Transfer widgets from the new layout to the existing one
        while (QLayoutItem *item = newLayout->takeAt(0)) {
            if (item->widget()) {
                qDebug() << "Adding widget" << item->widget()->objectName()
                         << "to leftverticalLayout";
                item->widget()->setParent(ui->verticalLayoutWidget); // Ensure correct parenting
                ui->leftverticalLayout->addWidget(item->widget());
            } else if (item->layout()) {
                qDebug() << "Adding layout" << item->layout()->objectName()
                         << "to leftverticalLayout";
                ui->leftverticalLayout->addLayout(item->layout());
            }
        }
    } else {
        qWarning() << "No QVBoxLayout found in loaded widget";
    }

    newWidget->deleteLater();
    qDebug() << "Temporary widget deleted";

    ui->leftverticalLayout->update();   // Force layout update
    ui->verticalLayoutWidget->update(); // Force container update
}

void Gui::clearLayout(QLayout *layout)
{
    if (!layout) {
        qWarning() << "Layout to clear is null";
        return;
    }

    qDebug() << "Clearing layout" << layout->objectName();

    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
            qDebug() << "Deleting widget" << widget->objectName();
            widget->deleteLater();
        }
        if (QLayout *childLayout = item->layout()) {
            qDebug() << "Clearing child layout" << childLayout->objectName();
            clearLayout(childLayout);
        }
        delete item;
    }
}

void Gui::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    // Implement any specific show event logic here
}

void Gui::on_addPointButton_clicked()
{
    // Create a new dialog instance
    AddPointDialog *dialog = new AddPointDialog(this);

    // Move the dialog to the bottom-left corner of the main window
    dialog->moveToBottomLeft();

    // Set the attribute to delete the dialog when it's closed
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Connect the accepted signal to a slot to handle dialog input
    connect(dialog, &AddPointDialog::accepted, this, [this, dialog]() {
        double xCoordinate = dialog->getXCoordinate();
        double zCoordinate = dialog->getZCoordinate();

        qDebug() << "Adding Point to Database:" << xCoordinate << zCoordinate;

        dataBasePointsManager->addObjectToDataBase(xCoordinate, zCoordinate);

        dataBasePointsManager->iterateOverTable();
        points.clear();

        for (const auto &point : dataBasePointsManager->getPointsMap()) {
            points.push_back({point.second.first, point.second.second, point.first});
        }

        Gui::on_refreshButton_clicked();

        Gui::on_addPointButton_clicked();
    });

    // Connect the rejected signal to delete the dialog
    connect(dialog, &AddPointDialog::rejected, dialog, &AddPointDialog::deleteLater);

    // Show the dialog non-modally
    dialog->show();
}

void Gui::on_addLineButton_clicked()
{
    // Iterate over the table to ensure the latest data is available
    dataBasePointsManager->iterateOverTable();

    // Create a new AddLineDialog instance
    AddLineDialog *dialog = new AddLineDialog(this);

    // Move the dialog to the bottom-left corner of the main window
    dialog->moveToBottomLeft();

    // Set the attribute to delete the dialog when it's closed
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Connect the accepted signal to handle dialog input
    connect(dialog, &AddLineDialog::accepted, this, [this, dialog]() {
        int startId = dialog->getFirstPointId();
        int endId = dialog->getSecondPointId();

        auto pointsMap = dataBasePointsManager->getPointsMap();

        if (pointsMap.find(startId) != pointsMap.end() && pointsMap.find(endId) != pointsMap.end()) {
            auto startPoint = pointsMap[startId];
            auto endPoint = pointsMap[endId];

            // Generate a unique ID for the new line
            int newLineId = lines.empty() ? 1 : lines.back().id + 1;

            // Add the new line to the database
            dataBaseLinesManager->addObjectToDataBase(startId, endId);

            // Add the new line to the lines vector
            lines.push_back(
                {startPoint.first, startPoint.second, endPoint.first, endPoint.second, newLineId});
        }

        // Update the UI
        Gui::on_refreshButton_clicked();
        // Optionally, you might want to call another method here
        Gui::on_addLineButton_clicked(); // Be cautious with recursion
    });

    // Connect the rejected signal to delete the dialog
    connect(dialog, &AddLineDialog::rejected, dialog, &AddLineDialog::deleteLater);

    // Show the dialog non-modally
    dialog->show();
}
void Gui::on_addSupportButton_clicked()
{
    // Create a new AddBoundariesDialog instance
    AddBoundariesDialog *dialog = new AddBoundariesDialog(this);

    // Move the dialog to the bottom-left corner of the main window
    dialog->moveToBottomLeft();

    // Set the attribute to delete the dialog when it's closed
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Connect the accepted signal to handle dialog input
    connect(dialog, &AddBoundariesDialog::accepted, this, [this, dialog]() {
        int pointId = dialog->getPointId();
        bool ry = dialog->getRy();
        bool tx = dialog->getTx();
        bool tz = dialog->getTz();

        qDebug() << "Support added for:" << pointId << " ry:" << ry << " tx:" << tx << " tz:" << tz;

        dataBaseSupportsManager->addObjectToDataBase(pointId, ry, tz, tx);

        dataBaseSupportsManager->iterateOverTable();
        boundaries.clear();

        auto supportsMap = dataBaseSupportsManager->getSupportsMap();
        for (const auto &support : supportsMap) {
            int pointId = std::get<0>(support.second);
            bool ry = std::get<1>(support.second);
            bool tx = std::get<2>(support.second);
            bool tz = std::get<3>(support.second);

            qDebug() << "Support added for pointId:" << pointId << " ry:" << ry << " tx:" << tx
                     << " tz:" << tz;

            boundaries.push_back({pointId, ry, tx, tz});
        }

        Gui::on_refreshButton_clicked();

        Gui::on_addSupportButton_clicked(); // Be cautious with recursion
    });

    // Connect the rejected signal to delete the dialog
    connect(dialog, &AddBoundariesDialog::rejected, dialog, &AddBoundariesDialog::deleteLater);

    // Show the dialog non-modally
    dialog->show();
}
void Gui::on_deleteObjectButton_clicked()
{
    // Tworzymy instancję dialogu do usuwania obiektów
    DeleteObjectDialog *dialog = new DeleteObjectDialog(this);

    // Przemieszczamy dialog do lewego dolnego rogu głównego okna

    dialog->initializeWithType(deleteLastSelectedType);

    dialog->moveToBottomLeft();

    // Ustawiamy atrybut, aby dialog został usunięty po jego zamknięciu
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Po zamknięciu dialogu, przetwarzamy wybór użytkownika
    connect(dialog, &DeleteObjectDialog::accepted, this, [this, dialog]() {
        QString selectedType = dialog->getSelectedObjectType();

        if (selectedType == "Points") {
            int pointId = dialog->getPointId();
            if (pointId != -1) {
                // Usuwamy punkt z bazy danych
                dataBasePointsManager->deleteObjectFromDataBase(pointId);
            }
        } else if (selectedType == "Lines") {
            int lineId = dialog->getLineId();
            if (lineId != -1) {
                // Usuwamy linię z bazy danych
                dataBaseLinesManager->deleteObjectFromDataBase(lineId);
            }
        } else if (selectedType == "Supports") {
            int supportPointId = dialog->getSupportPointId();
            if (supportPointId != -1) {
                // Usuwamy podporę z bazy danych
                dataBaseSupportsManager->deleteObjectFromDataBase(supportPointId);
            }
        }
        on_refreshButton_clicked();
        deleteLastSelectedType = selectedType;
        Gui::on_deleteObjectButton_clicked();
    });

    // Po odrzuceniu dialogu, usuwamy go
    connect(dialog, &DeleteObjectDialog::rejected, dialog, &DeleteObjectDialog::deleteLater);

    // Pokazujemy dialog nienachalnie
    dialog->show();
}

void Gui::on_editObjectButton_clicked()
{
    // Create an instance of the edit dialog
    EditObjectDialog *dialog = new EditObjectDialog(this);

    // Initialize the dialog with the last selected type
    dialog->initializeWithType(lastSelectedType);

    // Move the dialog to the bottom left of the main window
    dialog->moveToBottomLeft();

    // Set the attribute to delete the dialog on close
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Handle user acceptance
    connect(dialog, &EditObjectDialog::accepted, this, [this, dialog]() {
        QString selectedType = dialog->getSelectedObjectType();

        // Handle editing based on the selected type
        if (selectedType == "Points") {
            int pointId = dialog->getPointToEditId();
            int newX = dialog->getNewXCord();
            int newZ = dialog->getNewZCord();
            dataBasePointsManager->editPoint(pointId, newX, newZ);
        } else if (selectedType == "Lines") {
            int lineId = dialog->getLineId();
            int newStartPoint = dialog->getNewStartPoint();
            int newEndPoint = dialog->getNewEndPoint();
            dataBaseLinesManager->editLine(lineId, newStartPoint, newEndPoint);
        } else if (selectedType == "Supports") {
            int supportPointId = dialog->getSupportPointId();
            bool ry = dialog->getRy();
            bool tx = dialog->getTx();
            bool tz = dialog->getTz();
            dataBaseSupportsManager->editSupport(supportPointId, ry, tz, tx);
        }

        // Refresh the UI
        on_refreshButton_clicked();

        // Update the last selected type
        lastSelectedType = selectedType;
        Gui::on_editObjectButton_clicked();
        // Reopen the dialog with the updated selected type
    });
    connect(dialog, &EditObjectDialog::rejected, dialog, &EditObjectDialog::deleteLater);
    // Show the dialog non-modally
    dialog->show();
}

void Gui::on_refreshButton_clicked()
{
    // Update the data from the database managers
    dataBasePointsManager->iterateOverTable();
    dataBaseLinesManager->iterateOverTable();
    dataBaseSupportsManager->iterateOverTable();
    dataBaseLineLoadsManager->iterateOverTable();

    // Clear current points, lines, and boundaries
    points.clear();
    lines.clear();
    boundaries.clear();
    nodalLoads.clear();

    // Update points from the database manager
    for (const auto &point : dataBasePointsManager->getPointsMap()) {
        points.push_back({point.second.first, point.second.second, point.first});
    }

    // Update lines from the database manager
    for (const auto &lineEntry : dataBaseLinesManager->getLinesMap()) {
        int lineId = lineEntry.first; // ID linii
        int startId = std::get<0>(lineEntry.second);
        int endId = std::get<1>(lineEntry.second);

        auto pointsMap = dataBasePointsManager->getPointsMap();

        if (pointsMap.find(startId) != pointsMap.end() && pointsMap.find(endId) != pointsMap.end()) {
            auto startPoint = pointsMap[startId];
            auto endPoint = pointsMap[endId];
            lines.push_back(
                {startPoint.first, startPoint.second, endPoint.first, endPoint.second, lineId});
        }
    }

    // Update supports from the database manager
    for (const auto &support : dataBaseSupportsManager->getSupportsMap()) {
        int pointId = std::get<0>(support.second);
        bool ry = std::get<1>(support.second);
        bool tx = std::get<2>(support.second);
        bool tz = std::get<3>(support.second);

        boundaries.push_back({pointId, ry, tx, tz});
    }

    //Update nodal loads from the database manager
    for (const auto &nodalLoad : dataBaseNodalLoadsManager->getNodalLoadsMap()) {
        nodalLoads.push_back({std::get<0>(nodalLoad.second), std::get<1>(nodalLoad.second), std::get<2>(nodalLoad.second), std::get<3>(nodalLoad.second)});


    // Update the widget to trigger a repaint
    update();
}
}

void Gui::on_clearButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Clear Data",
                                  "This operation will lead to losing all data about the "
                                  "structure. Are you sure you want to proceed?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // Clear all data from the database managers
        dataBasePointsManager->dropTable(TableType::POINTS);
        dataBaseLinesManager->dropTable(TableType::LINES);
        dataBaseSupportsManager->dropTable(TableType::SUPPORTS);
        dataBaseMaterialsManager->dropTable(TableType::MATERIALS);
        dataBaseCrossSectionsManager->dropTable(TableType::CROSS_SECTIONS);
        dataBaseNodalLoadsManager->dropTable(TableType::NODAL_LOADS);
        dataBaseLineLoadsManager->dropTable(TableType::LINE_LOADS);

        dataBaseStarter->createPointsTable();
        dataBaseStarter->createLinesTable();
        dataBaseStarter->createSupportsTable();
        dataBaseStarter->createMaterialsTable();
        dataBaseStarter->createCrossSectionsTable();
        dataBaseStarter->createNodalLoadsTable();
        dataBaseStarter->createLineLoadsTable();

        // Refresh the UI to reflect changes
        on_refreshButton_clicked();

        qDebug() << "Data has been cleared.";
    } else {
        qDebug() << "Clear operation canceled.";
    }
}

void Gui::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw axes with labels
    drawAxes(painter);

    // Apply transformations for points, lines, and supports
    painter.setTransform(QTransform()
                             .translate(translationOffset.x(), translationOffset.y())
                             .scale(scaleFactor, scaleFactor));

    // Draw points, lines, and supports
    paintLines(painter);
    paintPoints(painter);
    paintSupports(painter);
    drawLoads(painter);
}
void Gui::paintPoints(QPainter &painter)
{
    painter.setBrush(Qt::blue);
    painter.setPen(Qt::blue);

    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    for (const auto &point : points) {
        painter.drawEllipse(QPointF(point.x, -point.z), 5, 5);
        painter.drawText(QPointF(point.x + 10, -point.z - 10), QString::number(point.id));
    }
}

void Gui::paintLines(QPainter &painter)
{
    painter.setPen(Qt::black);

    for (const auto &line : lines) {
        painter.drawLine(QPointF(line.startX, -line.startZ), QPointF(line.endX, -line.endZ));

        // Ustawienie czcionki i rysowanie ID
        QFont font = painter.font();
        font.setPointSize(10);
        painter.setFont(font);

        QPointF textPosition((line.startX + line.endX) / 2, (-line.startZ + -line.endZ) / 2);
        painter.drawText(textPosition + QPointF(10, -10), QString::number(line.id));
    }
}

void Gui::paintSupports(QPainter &painter)
{
    painter.setPen(QPen(Qt::darkBlue, 1));

    for (const auto &boundary : boundaries) {
        int pointId = boundary.pointId;

        if (dataBasePointsManager->getPointsMap().find(pointId)
            == dataBasePointsManager->getPointsMap().end()) {
            qDebug() << "Point ID not found in pointsMap: " << pointId;
            continue;
        }

        auto point = dataBasePointsManager->getPointsMap()[pointId];
        qreal x = point.first;
        qreal z = -point.second;

        qreal eccentricity = 30; // Adjust as needed

        painter.setBrush(Qt::magenta);

        // draw ry
        if (boundary.ry) {
            // Save the current painter state
            painter.save();

            // Translate to the point (x, z)
            painter.translate(QPointF(x, z));

            // Rotate the painter's coordinate system by 230 degrees
            painter.rotate(-230);

            // Draw the triangle with one angle of 20 degrees and the other two of 80 degrees
            double angleRad = 20 * M_PI / 180.0;                // Convert angle to radians
            double height = (4 * eccentricity) * tan(angleRad); // Calculate height of the triangle

            QPointF point1(0, 0);
            QPointF point2(-eccentricity / 4, height);
            QPointF point3(eccentricity / 4, height);

            QVector<QPointF> points;
            points << point1 << point2 << point3;

            painter.drawPolygon(QPolygonF(points));

            // Restore the painter state to its original form
            painter.restore();
        }

        // draw tz
        if (boundary.tz) {
            painter.drawLine(QPointF(x, z), QPointF(x, z + eccentricity));
            painter.drawEllipse(QPointF(x, z + eccentricity), 5, 5);
        }

        // draw tx
        if (boundary.tx) {
            painter.drawLine(QPointF(x, z), QPointF(x - eccentricity, z));
            painter.drawEllipse(QPointF(x - eccentricity, z), 5, 5);
        }

        // if non checked draw hinge
        if (!boundary.ry && !boundary.tx && !boundary.tz) {
            painter.drawEllipse(QPointF(x, z), 5, 5);
        }
    }
}

void Gui::drawAxes(QPainter &painter)
{
    // Save the current transformation
    QTransform originalTransform = painter.transform();

    // Calculate the center of the widget
    qreal centerX = translationOffset.x();
    qreal centerZ = translationOffset.y(); //y bc this is from qt documentation

    // Set up the font for axis labels
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    // Define the step for axis labels
    qreal step = 50.0;

    // Set up the pen for drawing axes
    painter.setPen(QPen(Qt::black, 2));

    // Calculate the visible range of the widget in real coordinates
    qreal leftX = -centerX / scaleFactor;
    qreal rightX = (width() - centerX) / scaleFactor;
    qreal topZ = (height() - centerZ) / scaleFactor; // Top is positive
    qreal bottomZ = -centerZ / scaleFactor;          // Bottom is negative

    // Draw X-axis
    painter.drawLine(QPointF(leftX * scaleFactor + centerX, centerZ),
                     QPointF(rightX * scaleFactor + centerX, centerZ));

    // Draw labels at fixed real coordinates along the X-axis
    for (qreal x = std::floor(leftX / step) * step; x <= rightX; x += step) {
        QPointF labelPos(centerX + x * scaleFactor, centerZ + 10);
        painter.drawText(labelPos, QString::number(x, 'f', 0));
    }

    // Draw Z-axis (Y-axis in this case) with positive direction upwards
    painter.drawLine(QPointF(centerX, bottomZ * scaleFactor + centerZ),
                     QPointF(centerX, topZ * scaleFactor + centerZ));

    // Draw labels at fixed real coordinates along the Z-axis
    for (qreal z = std::floor(topZ / step) * step; z >= bottomZ; z -= step) {
        QPointF labelPos(centerX + 10, centerZ + z * scaleFactor);
        painter.drawText(labelPos, QString::number(-z, 'f', 0));
        // Print current z value
        // qDebug() << "z: " << z;
    }

    // Draw the grid
    drawGrid(painter, leftX, rightX, topZ, bottomZ, step, centerX, centerZ);

    // Restore the original transformation
    painter.setTransform(originalTransform);
}

void Gui::drawGrid(QPainter &painter,
                   qreal leftX,
                   qreal rightX,
                   qreal topZ,
                   qreal bottomZ,
                   qreal step,
                   qreal centerX,
                   qreal centerZ)
{
    QPen gridPen(Qt::lightGray, 1, Qt::DashLine);
    painter.setPen(gridPen);

    // Draw vertical grid lines
    for (qreal x = std::floor(leftX / step) * step; x <= rightX; x += step) {
        painter.drawLine(QPointF(centerX + x * scaleFactor, bottomZ * scaleFactor + centerZ),
                         QPointF(centerX + x * scaleFactor, topZ * scaleFactor + centerZ));
    }

    // Draw horizontal grid lines
    for (qreal z = std::floor(topZ / step) * step; z >= bottomZ; z -= step) {
        painter.drawLine(QPointF(leftX * scaleFactor + centerX, centerZ + z * scaleFactor),
                         QPointF(rightX * scaleFactor + centerX, centerZ + z * scaleFactor));
    }
}

void Gui::drawLoads(QPainter &painter) {
    painter.setPen(QPen(Qt::red, 1));

    for (const auto &nodalLoad : nodalLoads) {
        int pointId = nodalLoad.pointId;

        if (dataBasePointsManager->getPointsMap().find(pointId)
            == dataBasePointsManager->getPointsMap().end()) {
            qDebug() << "Point ID not found in pointsMap: " << pointId;
            continue;
        }

        auto point = dataBasePointsManager->getPointsMap()[pointId];
        qreal x = point.first;
        qreal z = -point.second;

        qreal scale = 10; // Skala do rysowania strzałek
        qreal eccentricity = 30; // Adjust as needed

        painter.setBrush(Qt::magenta);

        // Rysowanie strzałki dla Fx
        if (nodalLoad.Fx != 0) {
            qreal endX = x + scale * nodalLoad.Fx;
            painter.drawLine(QPointF(x, z), QPointF(endX, z));
            drawArrowHead(painter, QPointF(endX, z), QPointF(x, z));
        }

        // Rysowanie strzałki dla Fz
        if (nodalLoad.Fz != 0) {
            qreal endZ = z - scale * nodalLoad.Fz;
            painter.drawLine(QPointF(x, z), QPointF(x, endZ));
            drawArrowHead(painter, QPointF(x, endZ), QPointF(x, z));
        }

        // Rysowanie półkółka ze strzałką dla My
        if (nodalLoad.My != 0) {
            // Save the current painter state
            painter.save();

            // Translate to the point (x, z)
            painter.translate(QPointF(x, z));

            // Set the rotation direction based on the sign of My
            qreal rotationAngle = (nodalLoad.My > 0) ? 180 : -180;
            painter.rotate(rotationAngle);

            // Draw the arc
            QRectF rect(-eccentricity, -eccentricity, 2 * eccentricity, 2 * eccentricity);
            int startAngle = 0 * 16;
            int spanAngle = 180 * 16;
            painter.drawArc(rect, startAngle, spanAngle);

            // Draw the arrowhead
            qreal arrowHeadLength = 10;
            QPointF arrowP1 = QPointF(arrowHeadLength, -arrowHeadLength / 2);
            QPointF arrowP2 = QPointF(arrowHeadLength, arrowHeadLength / 2);
            painter.drawLine(QPointF(0, 0), arrowP1);
            painter.drawLine(QPointF(0, 0), arrowP2);

            // Restore the painter state
            painter.restore();
        }
    }
}

void Gui::drawArrowHead(QPainter &painter, const QPointF &start, const QPointF &end) {
    QLineF line(start, end);
    double angle = std::atan2(-line.dy(), line.dx());

    double arrowSize = 10;
    QPointF arrowP1 = start + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                      cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = start + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                      cos(angle + M_PI - M_PI / 3) * arrowSize);

    painter.drawLine(start, arrowP1);
    painter.drawLine(start, arrowP2);
}





void Gui::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0) {
        scaleFactor *= 1.1; // zoom in
    } else {
        scaleFactor /= 1.1; // zoom out
    }
    update();
}

void Gui::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        isDragging = true;
        lastMousePosition = event->localPos();
    }
}

void Gui::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        isDragging = false;
    }
}
void Gui::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging) {
        QPointF delta = event->localPos() - lastMousePosition;
        lastMousePosition = event->localPos();
        translationOffset += delta;
        update();
    }
}
