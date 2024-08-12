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
#include <QLabel>            // Include for QLabel
#include <QSlider>           // Include for QSlider
#include <QVBoxLayout>       // Include for QVBoxLayout
#include <QHBoxLayout>       // Include for QHBoxLayout
#include <QScrollArea>
#include <QScrollBar>
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
    , scaleFactor(0.1)
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
            nodalLoads.push_back({std::get<0>(nodalLoad.second),
                                  std::get<1>(nodalLoad.second),
                                  std::get<2>(nodalLoad.second),
                                  std::get<3>(nodalLoad.second)});
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
        dataBaseLineLoadsManager->addObjectToDataBase(lineId, Fx, Fz);
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
    case 3:
        on_refreshButton_clicked();
        loadMeshLayout();
        break;
    case 4:
        loadResultsLayout();

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
void Gui::loadMeshLayout()
{
    qDebug() << "Starting loadMeshLayout";

    // Load the UI layout from the file
    loadLayoutFromFile(":/ui/mesh_layout.ui");
    qDebug() << "UI layout loaded from file";

    // Find the main layout and widgets in the loaded UI
    QVBoxLayout *mainLayout = findChild<QVBoxLayout*>("mainLayout");
    QScrollArea *scrollArea = findChild<QScrollArea*>("scrollArea");
    QWidget *scrollAreaWidgetContents = findChild<QWidget*>("scrollAreaWidgetContents");
    QVBoxLayout *dynamicLayout = findChild<QVBoxLayout*>("dynamicLayout");
    QLabel *numberOfFeLabel = findChild<QLabel*>("numberOfFeLabel");
    QPushButton *applyButton = findChild<QPushButton*>("applyButton");

    if (!mainLayout || !scrollArea || !scrollAreaWidgetContents || !dynamicLayout || !numberOfFeLabel || !applyButton) {
        qWarning() << "Layouts or widgets not found!";
        return;
    }

    qDebug() << "Layouts and widgets found, clearing existing widgets";

    // Clear any existing widgets in the dynamic layout
    QLayoutItem *child;
    while ((child = dynamicLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    qDebug() << "Populating dynamic layout with new content";

    // Apply a consistent style to the horizontal sliders
    QString sliderStyle = R"(
        QSlider::groove:horizontal {
            border: 1px solid #999999;
            height: 8px; /* Adjust height for horizontal */
            background: #b0b0b0;
        }
        QSlider::handle:horizontal {
            background: #007bff;
            border: 1px solid #5c5c5c;
            width: 18px; /* Adjust width for horizontal */
            margin: -5px 0; /* Move handle a bit */
            border-radius: 9px; /* Rounded corners */
        }
    )";

    // Populate the dynamic layout with QLabel and QSlider for each Line's id
    for (const auto &line : lines) {
        qDebug() << "Processing Line ID:" << line.id;

        QHBoxLayout *hLayout = new QHBoxLayout();

        QLabel *label = new QLabel(QString("ID %1:").arg(line.id), this);
        hLayout->addWidget(label);
        qDebug() << "Label created and added for Line ID:" << line.id;

        QSlider *slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(1, 100);
        slider->setValue(10);  // Set the default slider value to 10
        slider->setStyleSheet(sliderStyle);  // Apply the same style to horizontal sliders
        hLayout->addWidget(slider);
        qDebug() << "Slider created and added for Line ID:" << line.id;

        QLabel *valueLabel = new QLabel(QString::number(slider->value()), this);
        hLayout->addWidget(valueLabel);
        qDebug() << "Value label created and added for Line ID:" << line.id;

        // Connect the slider's valueChanged signal to update the value label and call the slot
        connect(slider, &QSlider::valueChanged, [this, line, valueLabel](int value) {
            valueLabel->setText(QString::number(value));
            handleSliderValueChanged(value, line.id);
        });
        qDebug() << "Slider valueChanged signal connected for Line ID:" << line.id;

        // Add the horizontal layout to the dynamic layout
        dynamicLayout->addLayout(hLayout);
        qDebug() << "Horizontal layout added to dynamic layout for Line ID:" << line.id;

        // Initially add the default value (10) to meshNodesVector
        handleSliderValueChanged(10, line.id);
    }

    dynamicLayout->addStretch();  // Add stretch at the end for better layout
    qDebug() << "Stretch added to dynamic layout";

    // Set the Apply button behavior
    connect(applyButton, &QPushButton::clicked, this, &Gui::on_applyButton_clicked);

    qDebug() << "Load mesh layout complete.";
}


void Gui::handleSliderValueChanged(int value, int lineId)
{
    qDebug() << "Slider value changed for Line ID" << lineId << ": " << value;

    // Find the line with the given lineId
    auto it = std::find_if(lines.begin(), lines.end(), [lineId](const Line &line) {
        return line.id == lineId;
    });

    if (it != lines.end()) {
        // Clear existing nodes for this lineId
        meshNodesVector.erase(std::remove_if(meshNodesVector.begin(), meshNodesVector.end(),
                                             [lineId](const MeshNode &node) { return node.lineId == lineId; }),
                              meshNodesVector.end());

        // Calculate the length of the line
        double length = sqrt(pow(it->endX - it->startX, 2) + pow(it->endZ - it->startZ, 2));

        // Avoid division by zero by checking the length and value
        if (length == 0 || value <= 1) {
            qWarning() << "Invalid line length or number of elements.";
            return;
        }

        // Generate new nodes and store them in the vector
        for (int i = 1; i < value; ++i) { // We already have start and end points
            double factor = static_cast<double>(i) / value;
            double newX = it->startX + factor * (it->endX - it->startX);
            double newZ = it->startZ + factor * (it->endZ - it->startZ);
            meshNodesVector.push_back({lineId, newX, newZ});
        }
    } else {
        qWarning() << "Line ID " << lineId << " not found!";
    }
}

void Gui::on_applyButton_clicked()
{
    qDebug() << "Apply button clicked! Processing mesh nodes...";

    for (const auto &node : meshNodesVector) {
        qDebug() << "Line ID:" << node.lineId << ", Node X:" << node.x << ", Node Z:" << node.z;
    }

    // Further processing or use the meshNodesVector as needed
    Gui::on_refreshButton_clicked();
}



void Gui::loadResultsLayout()
{
    loadLayoutFromFile(":/ui/results_layout.ui");
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

            // Calculate the length of the line using the distance formula
            double length = sqrt(pow(endPoint.first - startPoint.first, 2) + pow(endPoint.second - startPoint.second, 2));

            // Generate a unique ID for the new line
            int newLineId = lines.empty() ? 1 : lines.back().id + 1;

            // Add the new line to the database
            dataBaseLinesManager->addObjectToDataBase(startId, endId);

            // Add the new line to the lines vector with the calculated length
            lines.push_back(
                {startPoint.first, startPoint.second, endPoint.first, endPoint.second, newLineId, 0, length});

            // Debugging output
            cout << "startPoint.first: " << startPoint.first
                 << " startPoint.second: " << startPoint.second
                 << " endPoint.first: " << endPoint.first << " endPoint.second: " << endPoint.second
                 << " newLineId: " << newLineId
                 << " length: " << length << endl;
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
        } else if (selectedType == "LineLoads") {
            int loadLineId = dialog->getLineLoadId();
            if (loadLineId != -1) {
                // Usuwamy obciążenie liniowe z bazy danych
                dataBaseLineLoadsManager->deleteObjectFromDataBase(loadLineId);
            }
        } else if (selectedType == "NodalLoads") {
            int loadPointId = dialog->getNodalLoadId();
            if (loadPointId != -1) {
                // Usuwamy obciążenie węzłowe z bazy danych
                dataBaseNodalLoadsManager->deleteObjectFromDataBase(loadPointId);
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
        } else if (selectedType == "NodalLoads") {
            int loadPointId = dialog->getNodalLoadPointId();
            double newFz = dialog->getNewFz();
            double newFx = dialog->getNewFx();
            double newMy = dialog->getNewMy();
            dataBaseNodalLoadsManager->editObjectInDataBase(loadPointId, newMy, newFz, newFx);
        } else if (selectedType == "LineLoads") {
            int loadLineId = dialog->getLineLoadId();
            double newFx = dialog->getNewFxLineLoad();
            double newFz = dialog->getNewFzLineLoad();
            dataBaseLineLoadsManager->editObjectInDataBase(loadLineId, newFx, newFz);
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
    dataBaseNodalLoadsManager->iterateOverTable();

    // Clear current points, lines, and boundaries
    points.clear();
    lines.clear();
    boundaries.clear();
    nodalLoads.clear();
    lineLoads.clear();

    // Debug: Print the status before populating the vectors
    qDebug() << "Cleared vectors, starting to populate them";

    // Update points from the database manager
    for (const auto &point : dataBasePointsManager->getPointsMap()) {
        points.push_back({point.second.first, point.second.second, point.first});
    }
    qDebug() << "Points populated, size:" << points.size();

    // Update lines from the database manager
    for (const auto &lineEntry : dataBaseLinesManager->getLinesMap()) {
        int lineId = lineEntry.first; // ID linii
        int startId = std::get<0>(lineEntry.second);
        int endId = std::get<1>(lineEntry.second);
        int crossSectionId = std::get<2>(lineEntry.second);

        auto pointsMap = dataBasePointsManager->getPointsMap();

        if (pointsMap.find(startId) != pointsMap.end() && pointsMap.find(endId) != pointsMap.end()) {
            auto startPoint = pointsMap[startId];
            auto endPoint = pointsMap[endId];
            lines.push_back({startPoint.first,
                             startPoint.second,
                             endPoint.first,
                             endPoint.second,
                             lineId,
                             crossSectionId});
        }
    }
    qDebug() << "Lines populated, size:" << lines.size();

    // Update supports from the database manager
    for (const auto &support : dataBaseSupportsManager->getSupportsMap()) {
        int pointId = std::get<0>(support.second);
        bool ry = std::get<1>(support.second);
        bool tx = std::get<2>(support.second);
        bool tz = std::get<3>(support.second);

        boundaries.push_back({pointId, ry, tx, tz});
    }
    qDebug() << "Boundaries populated, size:" << boundaries.size();

    // Update nodal loads from the database manager
    for (const auto &nodalLoad : dataBaseNodalLoadsManager->getNodalLoadsMap()) {
        nodalLoads.push_back({std::get<0>(nodalLoad.second),
                              std::get<1>(nodalLoad.second),
                              std::get<2>(nodalLoad.second),
                              std::get<3>(nodalLoad.second)});
    }
    qDebug() << "Nodal Loads populated, size:" << nodalLoads.size();

    // Update line loads from the database manager
    for (const auto &lineLoad : dataBaseLineLoadsManager->getLineLoadsMap()) {
        lineLoads.push_back({std::get<0>(lineLoad.second),
                             std::get<1>(lineLoad.second),
                             std::get<2>(lineLoad.second)});
    }
    qDebug() << "Line Loads populated, size:" << lineLoads.size();

    // Update the widget to trigger a repaint
    update();
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
        // dataBaseMaterialsManager->dropTable(TableType::MATERIALS);
        // dataBaseCrossSectionsManager->dropTable(TableType::CROSS_SECTIONS);
        dataBaseNodalLoadsManager->dropTable(TableType::NODAL_LOADS);
        dataBaseLineLoadsManager->dropTable(TableType::LINE_LOADS);

        dataBaseStarter->createPointsTable();
        dataBaseStarter->createLinesTable();
        dataBaseStarter->createSupportsTable();
        // dataBaseStarter->createMaterialsTable();
        // dataBaseStarter->createCrossSectionsTable();
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

    drawAxes(painter);
    painter.setTransform(QTransform()
                             .translate(translationOffset.x(), translationOffset.y())
                             .scale(scaleFactor, scaleFactor));
    paintLines(painter);
    paintPoints(painter);
    paintSupports(painter);
    drawNodalLoads(painter);
    drawLineLoads(painter);
    paintMeshNodes(painter);
    // paintAssignedCrossSections(painter);
}

void Gui::paintPoints(QPainter &painter)
{
    painter.setBrush(Qt::blue);
    painter.setPen(Qt::blue);

    QFont font = painter.font();
    font.setPointSize(150); // Powiększ rozmiar czcionki 15 razy
    painter.setFont(font);

    for (const auto &point : points) {
        // Powiększ promień elipsy 15 razy, ale współrzędne pozostają takie same
        painter.drawEllipse(QPointF(point.x, -point.z), 75, 75); // Zwiększ promień elipsy do 75
        // Delikatnie bardziej odsunięta etykieta od środka elipsy
        painter.drawText(QPointF(point.x + 85, -point.z - 85),
                         QString::number(point.id)); // Zwiększ przesunięcie etykiety
    }
}

void Gui::paintLines(QPainter &painter)
{
    // Ustawienie grubości linii 15 razy większej
    QPen pen(Qt::black, 15); // Zwiększ grubość linii i ustaw kolor na czarny
    painter.setPen(pen);

    for (const auto &line : lines) {
        // Współrzędne linii pozostają takie same
        painter.drawLine(QPointF(line.startX, -line.startZ), QPointF(line.endX, -line.endZ));

        // Ustawienie czcionki i rysowanie ID
        QFont font = painter.font();
        font.setPointSize(150); // Powiększ rozmiar czcionki 15 razy
        painter.setFont(font);

        // Współrzędne tekstu pozostają takie same
        QPointF textPosition((line.startX + line.endX) / 2 + 15,
                             (-line.startZ + -line.endZ) / 2 - 15);
        painter.drawText(textPosition, QString::number(line.id));
    }
}
void Gui::paintAssignedCrossSections(QPainter &painter)
{
    painter.setPen(Qt::darkGreen);

    QFont font = painter.font();
    font.setPointSize(150); // Powiększ rozmiar czcionki 15 razy
    painter.setFont(font);

    qreal offset = 350; // Ustaw wartość offsetu wzdłuż linii

    for (const auto &line : lines) {
        if (line.crossSectionId != -1) {
            auto crossSectionsMap = dataBaseCrossSectionsManager->getCrossSectionsMap();
            auto crossSectionIt = crossSectionsMap.find(line.crossSectionId);

            auto crossSectionName = QString::fromStdString(crossSectionIt->second);

            auto linesMap = dataBaseLinesManager->getLinesMap();
            auto lineIt = linesMap.find(line.id);

            if (lineIt == linesMap.end()) {
                qDebug() << "Line ID not found in linesMap: " << line.id;
                continue;
            }

            auto lineData = lineIt->second;
            auto pointsMap = dataBasePointsManager->getPointsMap();

            int pointId1 = std::get<0>(lineData);
            int pointId2 = std::get<1>(lineData);

            auto point1It = pointsMap.find(pointId1);
            auto point2It = pointsMap.find(pointId2);

            if (point1It == pointsMap.end() || point2It == pointsMap.end()) {
                qDebug() << "Point ID not found in pointsMap: " << pointId1 << " or " << pointId2;
                continue;
            }

            auto point1 = point1It->second;
            auto point2 = point2It->second;

            qreal x1 = point1.first;
            qreal z1 = -point1.second;
            qreal x2 = point2.first;
            qreal z2 = -point2.second;

            // Compute the midpoint of the line
            qreal midX = (x1 + x2) / 2;
            qreal midZ = (z1 + z2) / 2;

            // Compute the direction vector of the line
            qreal dx = x2 - x1;
            qreal dz = z2 - z1;
            qreal length = std::sqrt(dx * dx + dz * dz);
            dx /= length;
            dz /= length;

            // Adjust the midpoint by the offset
            qreal textX = midX - offset * dx;
            qreal textZ = midZ - offset * dz;

            // Convert qreal to QPointF
            QPointF textPosition(textX, textZ);

            // Draw the cross-section name at the adjusted position
            painter.drawText(textPosition, crossSectionName);
        }
    }
}

void Gui::paintMeshNodes(QPainter &painter)
{
    // Step 1: Set up the brush and pen for drawing nodes
    painter.setBrush(Qt::gray);
    painter.setPen(Qt::gray);

    // Step 2: Adjust the font size for node labels
    QFont font = painter.font();
    font.setPointSize(30); // 1/5 of the font size used in paintPoints
    painter.setFont(font);

    // Step 3: Iterate over meshNodesVector and draw each node
    for (const auto &node : meshNodesVector) {
        // Draw the node as an ellipse with 1/5 of the size of points
        painter.drawEllipse(QPointF(node.x, -node.z), 15, 15); // 1/5 of the size of points

        // Draw the label with coordinates (x, z)
        QString label = QString("(%1, %2)").arg(node.x).arg(node.z);
        painter.drawText(QPointF(node.x + 18, -node.z - 18), label); // Adjust the label position slightly
    }
}



void Gui::paintSupports(QPainter &painter)
{
    painter.setPen(QPen(Qt::darkBlue, 15)); // Powiększ grubość linii 15 razy

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

        qreal eccentricity = 450; // Powiększ wartość ekcentryczności 15 razy

        painter.setBrush(Qt::magenta);

        // Draw ry
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

        // Draw tz
        if (boundary.tz) {
            painter.drawLine(QPointF(x, z), QPointF(x, z + eccentricity));
            painter.drawEllipse(QPointF(x, z + eccentricity),
                                75,
                                75); // Powiększ promień elipsy 15 razy
        }

        // Draw tx
        if (boundary.tx) {
            painter.drawLine(QPointF(x, z), QPointF(x - eccentricity, z));
            painter.drawEllipse(QPointF(x - eccentricity, z),
                                75,
                                75); // Powiększ promień elipsy 15 razy
        }

        // If non-checked draw hinge
        if (!boundary.ry && !boundary.tx && !boundary.tz) {
            painter.drawEllipse(QPointF(x, z), 75, 75); // Powiększ promień elipsy 15 razy
        }
    }
}

void Gui::drawAxes(QPainter &painter)
{
    // Save the current transformation
    QTransform originalTransform = painter.transform();

    // Calculate the center of the widget
    qreal centerX = translationOffset.x();
    qreal centerZ = translationOffset.y(); // y bc this is from qt documentation

    // Set up the font for axis labels
    QFont font = painter.font();
    font.setPointSize(12); // Increase the font size
    painter.setFont(font);

    // Define the step for grid lines (100 mm)
    qreal step = 100.0;

    // Define the step for axis labels (500 mm)
    qreal labelStep = 500.0;

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

    // Draw labels at fixed real coordinates along the X-axis (every 500 mm)
    for (qreal x = std::floor(leftX / labelStep) * labelStep; x <= rightX; x += labelStep) {
        QPointF labelPos(centerX + x * scaleFactor, centerZ + 10);
        painter.drawText(labelPos, QString::number(x / 1000.0, 'f', 1)); // Convert mm to meters
    }

    // Draw Z-axis (Y-axis in this case) with positive direction upwards
    painter.drawLine(QPointF(centerX, bottomZ * scaleFactor + centerZ),
                     QPointF(centerX, topZ * scaleFactor + centerZ));

    // Draw labels at fixed real coordinates along the Z-axis (every 500 mm)
    for (qreal z = std::floor(topZ / labelStep) * labelStep; z >= bottomZ; z -= labelStep) {
        QPointF labelPos(centerX + 10, centerZ + z * scaleFactor);
        painter.drawText(labelPos, QString::number(-z / 1000.0, 'f', 1)); // Convert mm to meters
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
void Gui::drawNodalLoads(QPainter &painter)
{
    painter.setPen(QPen(Qt::red, 8)); // Ustawienie grubości linii

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

        qreal scale = 20;         // Skala dla długości strzałek
        qreal minScale = 75;      // Minimalna długość strzałki
        qreal eccentricity = 225; // Eccentricity dla półkółka

        painter.setBrush(Qt::magenta);

        // Rysowanie strzałki dla Fx
        if (nodalLoad.Fx != 0) {
            qreal length = qMax(scale * qAbs(nodalLoad.Fx), minScale);
            qreal endX = x + length * (nodalLoad.Fx / qAbs(nodalLoad.Fx));
            painter.drawLine(QPointF(x, z), QPointF(endX, z));
            drawArrowHead(painter, QPointF(endX, z), QPointF(x, z));

            // Dodanie wartości Fx
            QString fxText = QString::number(nodalLoad.Fx, 'f', 2) + " kN";
            painter.drawText(QPointF(endX + 30, z + 75), fxText); // Skala tekstu dopasowana
        }

        // Rysowanie strzałki dla Fz
        if (nodalLoad.Fz != 0) {
            qreal length = qMax(scale * qAbs(nodalLoad.Fz), minScale);
            qreal endZ = z - length * (nodalLoad.Fz / qAbs(nodalLoad.Fz));

            // Jeżeli Fz jest ujemne, przesuwamy strzałki w górę, ale zachowujemy kierunek
            if (nodalLoad.Fz < 0) {
                qreal offset = length; // Długość strzałki jako offset
                painter.drawLine(QPointF(x, z - offset), QPointF(x, endZ - offset));
                drawArrowHead(painter, QPointF(x, endZ - offset), QPointF(x, z - offset));

                // Dodanie wartości Fz
                QString fzText = QString::number(nodalLoad.Fz, 'f', 2) + " kN";
                painter.drawText(QPointF(x + 30, endZ - 75 - 2 * offset),
                                 fzText); // Skala tekstu dopasowana
            } else {
                // Rysowanie normalnych strzałek gdy Fz nie jest ujemne
                painter.drawLine(QPointF(x, endZ), QPointF(x, z));
                drawArrowHead(painter, QPointF(x, endZ), QPointF(x, z));

                // Dodanie wartości Fz
                QString fzText = QString::number(nodalLoad.Fz, 'f', 2) + " kN";
                painter.drawText(QPointF(x + 30, endZ - 75), fzText); // Skala tekstu dopasowana
            }
        }

        // Rysowanie półkółka ze strzałką dla My
        if (nodalLoad.My != 0) {
            painter.save();

            painter.translate(QPointF(x, z));

            qreal rotationAngle = (nodalLoad.My > 0) ? 0 : 180;
            painter.rotate(rotationAngle);

            QRectF rect(-eccentricity, -eccentricity, 2 * eccentricity, 2 * eccentricity);
            int startAngle = 0 * 16;
            int spanAngle = 180 * 16;
            painter.drawArc(rect, startAngle, spanAngle);

            // Grot strzałki skierowany w stronę końca łuku
            qreal arrowHeadLength = 150; // Długość głowy strzałki 15x większa
            QPointF arrowBase, arrowP1, arrowP2;
            if (nodalLoad.My < 0) {
                arrowBase = QPointF(eccentricity, 0); // Koniec półkola po lewej
                arrowP1 = arrowBase + QPointF((arrowHeadLength) / 2, arrowHeadLength);
                arrowP2 = arrowBase + QPointF((-arrowHeadLength - 150) / 2, arrowHeadLength);
            } else {
                arrowBase = QPointF(-eccentricity, 0); // Koniec półkola po prawej
                arrowP1 = arrowBase + QPointF((arrowHeadLength + 150) / 2, arrowHeadLength);
                arrowP2 = arrowBase + QPointF((-arrowHeadLength) / 2, arrowHeadLength);
            }

            painter.rotate(180);
            painter.drawLine(arrowBase, arrowP1);
            painter.drawLine(arrowBase, arrowP2);

            painter.restore();

            // Dodanie wartości My
            QString myText = QString::number(nodalLoad.My, 'f', 2) + " kNm";
            painter.drawText(QPointF(x - eccentricity - 590, z - 250),
                             myText); // Skala tekstu dopasowana
        }
    }
}

void Gui::drawLineLoads(QPainter &painter)
{
    // Ustawienie grubości linii
    painter.setPen(QPen(Qt::red, 8));

    for (const auto &lineLoad : lineLoads) {
        int lineId = lineLoad.lineId;

        if (dataBaseLinesManager->getLinesMap().find(lineId)
            == dataBaseLinesManager->getLinesMap().end()) {
            qDebug() << "Line ID not found in linesMap: " << lineId;
            continue;
        }

        auto line = dataBaseLinesManager->getLinesMap()[lineId];
        auto pointsMap = dataBasePointsManager->getPointsMap();

        int pointId1 = std::get<0>(line);
        int pointId2 = std::get<1>(line);

        if (pointsMap.find(pointId1) == pointsMap.end()
            || pointsMap.find(pointId2) == pointsMap.end()) {
            qDebug() << "Point ID not found in pointsMap: " << pointId1 << " or " << pointId2;
            continue;
        }

        auto point1 = pointsMap[pointId1];
        auto point2 = pointsMap[pointId2];

        qreal x1 = point1.first;
        qreal z1 = -point1.second;
        qreal x2 = point2.first;
        qreal z2 = -point2.second;

        // Obliczenie wektora kierunku linii
        qreal dx = x2 - x1;
        qreal dz = z2 - z1;
        qreal length = std::sqrt(dx * dx + dz * dz);
        qreal unitDx = dx / length;
        qreal unitDz = dz / length;

        // Liczba strzałek do narysowania na linii
        int numArrows = 10;
        qreal arrowSpacing = length / (numArrows + 1);
        qreal fxScale = 30;                 // Skala dla długości strzałek Fx
        qreal fxMinScale = 75;              // Minimalna długość strzałki Fx
        qreal arrowLength = 150;            // Długość strzałki
        qreal offset = arrowLength + 150;   // Odległość wartości od środka linii

        qreal fzScale = 15;                 // Skala dla długości strzałek Fz (zmniejszona)
        qreal fzMinScale = 50;              // Minimalna długość strzałki Fz (zmniejszona)

        QVector<QPointF> fxArrowStarts; // Wektor do przechowywania punktów startowych strzałek dla Fx
        QVector<QPointF> fzArrowStarts; // Wektor do przechowywania punktów startowych strzałek dla Fz

        // Rysowanie strzałek wzdłuż linii dla Fx
        if (lineLoad.Fx != 0) {
            painter.setPen(QPen(Qt::red, 8)); // Ustawienie grubości linii na 8

            for (int i = 0; i <= numArrows + 1; ++i) {
                qreal px = x1 + i * arrowSpacing * unitDx;
                qreal pz = z1 + i * arrowSpacing * unitDz;

                qreal startX = px; // Start point is directly on the line
                qreal endX = startX + fxScale * (lineLoad.Fx / qAbs(lineLoad.Fx)); // Draw arrow in direction of Fx

                painter.drawLine(QPointF(startX, pz), QPointF(endX, pz));
                drawArrowHead(painter, QPointF(endX, pz), QPointF(startX, pz));

                // Dodanie punktu startowego strzałki do wektora Fx
                fxArrowStarts.append(QPointF(startX, pz));
            }

            // Dodanie wartości Fx w środku linii, odsunięte o długość strzałek + offset
            qreal midX = x1 + (1.0 / 2.0 * length * unitDx);
            qreal midZ = z1 + (1.0 / 2.0 * length * unitDz);
            QString fxText = QString::number(lineLoad.Fx, 'f', 2) + " kN/m";
            painter.drawText(QPointF(midX + offset * unitDx, midZ + offset * unitDz + 150), fxText);
        }

        // Rysowanie strzałek wzdłuż linii dla Fz
        if (lineLoad.Fz != 0) {
            painter.setPen(QPen(Qt::red, 8)); // Ustawienie grubości linii na 8

            for (int i = 0; i <= numArrows + 1; ++i) {
                qreal px = x1 + i * arrowSpacing * unitDx;
                qreal pz = z1 + i * arrowSpacing * unitDz;

                qreal length = qMax(fzScale * qAbs(lineLoad.Fz), fzMinScale);
                qreal startZ
                    = pz
                      + length
                            * (lineLoad.Fz
                               / qAbs(lineLoad.Fz)); // Original logic: offset based on load value
                painter.drawLine(QPointF(px, startZ), QPointF(px, pz));
                drawArrowHead(painter, QPointF(px, pz), QPointF(px, startZ));

                // Dodanie punktu startowego strzałki do wektora Fz
                fzArrowStarts.append(QPointF(px, startZ));
            }

            // Dodanie wartości Fz w środku linii, odsunięte o długość strzałek + offset
            qreal midX = x1 + (1.0 / 2.0 * length * unitDx);
            qreal midZ = z1 + (1.0 / 2.0 * length * unitDz);
            QString fzText = QString::number(lineLoad.Fz, 'f', 2) + " kN/m";
            painter.drawText(QPointF(midX - offset * unitDx, midZ - offset * unitDz - 150), fzText);
        }

        // Rysowanie linii łączącej wszystkie początki strzałek Fx
        if (fxArrowStarts.size() > 1) {
            painter.setPen(QPen(Qt::red, 8)); // Ustawienie grubości linii
            for (int i = 1; i < fxArrowStarts.size(); ++i) {
                painter.drawLine(fxArrowStarts[i - 1], fxArrowStarts[i]);
            }
        }

        // Rysowanie linii łączącej wszystkie początki strzałek Fz
        if (fzArrowStarts.size() > 1) {
            painter.setPen(QPen(Qt::red, 8)); // Ustawienie grubości linii
            for (int i = 1; i < fzArrowStarts.size(); ++i) {
                painter.drawLine(fzArrowStarts[i - 1], fzArrowStarts[i]);
            }
        }
    }
}

void Gui::drawArrowHead(QPainter &painter, const QPointF &start, const QPointF &end)
{
    QLineF line(start, end);
    double angle = std::atan2(-line.dy(), line.dx());

    double arrowSize = 120; // Skala 15x większa
    QPointF arrowP1 = start
                      + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = start
                      + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                cos(angle + M_PI - M_PI / 3) * arrowSize);

    painter.drawLine(start, arrowP1);
    painter.drawLine(start, arrowP2);
}



void Gui::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0) {
        scaleFactor *= 1.2; // zoom in
    } else {
        scaleFactor /= 1.2; // zoom out
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
