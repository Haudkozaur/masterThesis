#include "SlabGUI.h"
#include "ui_SlabGUI.h"
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTransform>
#include <QUiLoader>
#include <QWheelEvent>
#include <QXmlStreamReader>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QApplication>
#include <QSet>
#include <QPainter>
#include <QPointF>
#include <QSettings>
#include "AddSurfaceDialog.h"
#include "AddSlabSupportsDialog.h"
#include "../GUI/AddMaterialDialog.h"
#include "slabSetPropertiesDialog.h"
#include "AddSurfaceSupportDialog.h"
#include "AddSlabPointLoadDialog.h"
#include "AddSlabLineLoadDialog.h"
#include "AddSurfaceLoadDialog.h"
#include "../FreeFemExecutor.h"


SlabGUI::SlabGUI(DataBasePointsManager *pointsManager,
                 DataBaseLinesManager *linesManager,
                 DataBaseMaterialsManager *materialsManager,
                 DataBaseSurfacesManager *surfacesManager,
                 DataBaseCircularLinesManager *circularLinesManager,
                 DataBaseLineSupportsManager *lineSupportsManager,
                 DataBaseSurfaceSupportsManager *surfaceSupportsManager,
                 DataBaseSlabPointLoadManager *slabPointLoadsManager,
                 DataBaseSlabLineLoadsManager *slabLineLoadsManager,
                 DataBaseSurfaceLoadsManager *surfaceLoadsManager,
                 DataBaseSlabMeshManager *slabMeshManager,
                 DataBaseFreeFEMPreparer *freeFEMPreparer,
                 DataBaseStarter *starter,
                 QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SlabGUI)
    , dataBasePointsManager(pointsManager)
    , dataBaseLinesManager(linesManager)
    , dataBaseMaterialsManager(materialsManager)
    , dataBaseSurfacesManager(surfacesManager)
    , dataBaseCircularLinesManager(circularLinesManager)
    , dataBaseLineSupportsManager(lineSupportsManager)
    , dataBaseSurfaceSupportsManager(surfaceSupportsManager)
    , dataBaseSlabPointLoadManager(slabPointLoadsManager)
    , dataBaseSlabLineLoadsManager(slabLineLoadsManager)
    , dataBaseSurfaceLoadsManager(surfaceLoadsManager)
    , dataBaseSlabMeshManager(slabMeshManager)
    , dataBaseFreeFEMPreparer(freeFEMPreparer)
    , dataBaseStarter(starter)
    , xCoordinate(0)
    , zCoordinate(0)
    , scaleFactor(0.1)
    , isDragging(false)
{
    ui->setupUi(this);
    connect(ui->modelPhaseComboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(onComboBoxIndexChanged(int)));
    translationOffset = QPoint(width() / 2, height() / 2);
    QWidget *leftButtonContainer = ui->leftverticalLayout->parentWidget();
    QWidget *upperButtonContainer = ui->uphorizontalLayout->parentWidget();
    if (leftButtonContainer && upperButtonContainer) {
        QString backgroundStyle = "background-color: rgba(255, 255, 255, 150);";
        leftButtonContainer->setStyleSheet(backgroundStyle);
        upperButtonContainer->setStyleSheet(backgroundStyle);
    }
}

SlabGUI::~SlabGUI()
{
    delete ui;
}

void SlabGUI::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0) {
        scaleFactor *= 1.2; // zoom in
    } else {
        scaleFactor /= 1.2; // zoom out
    }
    update();
}

void SlabGUI::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        isDragging = true;
        lastMousePosition = event->localPos();
    }
}

void SlabGUI::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        isDragging = false;
    }
}

void SlabGUI::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging) {
        QPointF delta = event->localPos() - lastMousePosition;
        lastMousePosition = event->localPos();
        translationOffset += delta;
        update();
    }
}

void SlabGUI::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawAxes(painter);
    painter.setTransform(QTransform()
                             .translate(translationOffset.x(), translationOffset.y())
                             .scale(scaleFactor, scaleFactor));
    paintPoints(painter);
    paintPointsLabels(painter);
    paintLines(painter);
    paintLinesLabels(painter);
    paintCircularLines(painter);
    paintCircularLinesLabels(painter);
    paintSurfaces(painter);
    paintLineSupports(painter);
    paintSurfaceSupports(painter);
    paintPointLoads(painter);
    paintLineLoads(painter);
    paintSurfaceLoads(painter);
    // Uncomment and implement the drawing functions as needed
    // if (showLines){
    //     paintLines(painter);
    // } if (showLinesLabels){
    //     paintLinesLabels(painter);
    // } if (showPoints){
    //     paintPoints(painter);
    // } if (showPointsLabels){
    //     paintPointsLabels(painter);
    // } if (showSupports){
    //     paintSupports(painter);
    // } if (showNodalLoads){
    //     drawNodalLoads(painter);
    // } if (showLineLoads){
    //     drawLineLoads(painter);
    // } if (showLineLoadsLabels){
    //     drawLineLoadsLabels(painter);
    // } if (showMesh){
    //     paintMeshNodes(painter);
    // } if (showAssignedCS){
    //     //paintAssignedCrossSections(painter);
    // }
    // paintResults(painter);
}

void SlabGUI::paintLineLoads(QPainter &painter)
{
    QPen pen(Qt::red, 16, Qt::DashLine); // Red color, thickness 16, dashed line
    painter.setPen(pen);

    QFont font = painter.font();
    font.setPointSize(150); // Set the font size similar to other labels
    painter.setFont(font);

    for (const auto &load : lineLoads) {
        // Draw the main line between (x1, z1) and (x2, z2)
        QPointF startPoint(load.x1, -load.z1);
        QPointF endPoint(load.x2, -load.z2);
        painter.drawLine(startPoint, endPoint);

        // Calculate the direction vector of the line
        QLineF line(startPoint, endPoint);
        QPointF direction = (endPoint - startPoint) / line.length();

        // Perpendicular direction vector for the short lines
        QPointF perpDirection(-direction.y(), direction.x());

        // Spacing of the short lines along the main line
        double spacing = 200.0;

        // Length of the perpendicular short lines
        double shortLineLength = 100.0;

        // Iterate along the line and draw short perpendicular lines
        for (double i = 0; i <= line.length(); i += spacing) {
            QPointF position = startPoint + i * direction;
            QLineF shortLine(position - perpDirection * (shortLineLength / 2),
                             position + perpDirection * (shortLineLength / 2));
            painter.drawLine(shortLine);
        }

        // Draw the load label next to the line's midpoint with the unit kN/m
        QPointF midPoint = (startPoint + endPoint) / 2;
        QString label = QString::number(load.F, 'f', 2) + " kN/m"; // Format the force value with unit
        painter.drawText(midPoint + QPointF(20, -20), label); // Adjust the offset as needed
    }
}


void SlabGUI::paintSurfaceLoads(QPainter &painter)
{
    QPen pen(Qt::red, 8);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);  // No brush for the rectangle itself

    for (const auto &load : surfaceLoads) {
        QRectF loadRect(QPointF(load.x1, -load.z1), QPointF(load.x2, -load.z2));
        painter.drawRect(loadRect);

        int numArrowsX = 10;
        int numArrowsZ = 10;

        qreal arrowSpacingX = loadRect.width() / (numArrowsX + 1);
        qreal arrowSpacingZ = loadRect.height() / (numArrowsZ + 1);

        qreal arrowLength = 100;
        qreal arrowHeadSize = 30; // Adjust arrow head size

        // Adjust the grid to start slightly inside the rectangle
        qreal startX = loadRect.left() + arrowSpacingX / 2;
        qreal startZ = loadRect.top() + arrowSpacingZ / 2;

        for (int i = 0; i <= numArrowsX; ++i) {
            for (int j = 0; j <= numArrowsZ; ++j) {
                qreal x = startX + i * arrowSpacingX;
                qreal z = startZ + j * arrowSpacingZ;

                if (load.F < 0) {
                    painter.drawLine(QPointF(x, z), QPointF(x, z + arrowLength));
                    drawArrowHead(painter, QPointF(x, z + arrowLength), QPointF(x, z));
                } else {
                    painter.drawLine(QPointF(x, z), QPointF(x, z - arrowLength));
                    drawArrowHead(painter, QPointF(x, z - arrowLength), QPointF(x, z));
                }
            }
        }

        // Draw labels with force value
        QString forceText = QString::number(load.F, 'f', 2) + " kN/m²";
        painter.drawText(QPointF(loadRect.center().x(), loadRect.center().y()), forceText);
    }
}

void SlabGUI::drawArrowHead(QPainter &painter, const QPointF &start, const QPointF &end)
{
    QLineF line(start, end);
    double angle = std::atan2(-line.dy(), line.dx());

    double arrowSize = 60; // Adjusted arrow size (smaller)
    QPointF arrowP1 = start + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                      cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = start + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                      cos(angle + M_PI - M_PI / 3) * arrowSize);

    painter.drawLine(start, arrowP1);
    painter.drawLine(start, arrowP2);
}





void SlabGUI::drawAxes(QPainter &painter)
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

void SlabGUI::drawGrid(QPainter &painter,
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

void SlabGUI::paintSurfaces(QPainter &painter)
{
    // Create a path to represent the main surface
    QPainterPath mainSurfacePath;

    for (const auto &surface : surfaces) {
        if (!surface.isOpening) {
            const std::string &surfaceType = surface.surfaceType;

            if (surfaceType == "rectangle" || surfaceType == "triangle") {
                QPolygonF polygon;
                auto addPointToPolygon = [&](int lineId) {
                    if (lineId == -1) return;

                    const auto &lineEntry = std::find_if(lines.begin(), lines.end(),
                                                         [&](const Line &line) { return line.id == lineId; });

                    if (lineEntry != lines.end()) {
                        polygon.append(QPointF(lineEntry->startX, -lineEntry->startZ));
                        polygon.append(QPointF(lineEntry->endX, -lineEntry->endZ));
                    }
                };

                addPointToPolygon(surface.line1Id);
                addPointToPolygon(surface.line2Id);
                addPointToPolygon(surface.line3Id);
                if (surfaceType == "rectangle") {
                    addPointToPolygon(surface.line4Id);
                }

                if (polygon.size() >= 3) {
                    mainSurfacePath.addPolygon(polygon);
                }
            } else if (surfaceType == "circle") {
                if (surface.circularLineId != -1) {
                    const auto &circularLineEntry = std::find_if(circularLines.begin(), circularLines.end(),
                                                                 [&](const CircularLine &circularLine) { return circularLine.id == surface.circularLineId; });

                    if (circularLineEntry != circularLines.end()) {
                        QPointF center(circularLineEntry->centreX, -circularLineEntry->centreZ);
                        QRectF boundingRect(center.x() - circularLineEntry->diameter / 2,
                                            center.y() - circularLineEntry->diameter / 2,
                                            circularLineEntry->diameter,
                                            circularLineEntry->diameter);
                        mainSurfacePath.addEllipse(boundingRect);
                    }
                }
            }
        }
    }

    // Create a path to represent the openings
    QPainterPath openingsPath;

    for (const auto &surface : surfaces) {
        if (surface.isOpening) {
            const std::string &surfaceType = surface.surfaceType;

            if (surfaceType == "rectangle" || surfaceType == "triangle") {
                QPolygonF polygon;
                auto addPointToPolygon = [&](int lineId) {
                    if (lineId == -1) return;

                    const auto &lineEntry = std::find_if(lines.begin(), lines.end(),
                                                         [&](const Line &line) { return line.id == lineId; });

                    if (lineEntry != lines.end()) {
                        polygon.append(QPointF(lineEntry->startX, -lineEntry->startZ));
                        polygon.append(QPointF(lineEntry->endX, -lineEntry->endZ));
                    }
                };

                addPointToPolygon(surface.line1Id);
                addPointToPolygon(surface.line2Id);
                addPointToPolygon(surface.line3Id);
                if (surfaceType == "rectangle") {
                    addPointToPolygon(surface.line4Id);
                }

                if (polygon.size() >= 3) {
                    openingsPath.addPolygon(polygon);
                }
            } else if (surfaceType == "circle") {
                if (surface.circularLineId != -1) {
                    const auto &circularLineEntry = std::find_if(circularLines.begin(), circularLines.end(),
                                                                 [&](const CircularLine &circularLine) { return circularLine.id == surface.circularLineId; });

                    if (circularLineEntry != circularLines.end()) {
                        QPointF center(circularLineEntry->centreX, -circularLineEntry->centreZ);
                        QRectF boundingRect(center.x() - circularLineEntry->diameter / 2,
                                            center.y() - circularLineEntry->diameter / 2,
                                            circularLineEntry->diameter,
                                            circularLineEntry->diameter);
                        openingsPath.addEllipse(boundingRect);
                    }
                }
            }
        }
    }

    // Set the painter's clip path to exclude the openings from being filled
    QPainterPath combinedPath = mainSurfacePath.subtracted(openingsPath);
    painter.setClipPath(combinedPath);

    // Fill the main surface without the openings
    QColor fillColor(128, 128, 128, 128); // Semi-transparent grey for main surfaces
    painter.fillPath(mainSurfacePath, fillColor);
}




void SlabGUI::loadLayoutFromFile(const QString &fileName)
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

void SlabGUI::clearLayout(QLayout *layout)
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

void SlabGUI::loadStaticSchemeLayout()
{
    loadLayoutFromFile(":/ui/slabLinesDiagram.ui");

    layoutAddSurfaceButton = findChild<QPushButton *>("layoutAddSurfaceButton");
    if (layoutAddSurfaceButton) {
        connect(layoutAddSurfaceButton, &QPushButton::clicked, this, &SlabGUI::on_addSurfaceButton_clicked);
    } else {
        qWarning() << "Button 'layoutAddSurfaceButton' not found!";
    }
    layoutAddSupportConditionsButton = findChild<QPushButton *>("layoutAddSupportConditionsButton");
    if (layoutAddSupportConditionsButton) {
        connect(layoutAddSupportConditionsButton,
                &QPushButton::clicked,
                this,
                &SlabGUI::on_addSupportConditionsButton_clicked);
    } else {
        qWarning() << "Button 'layoutAddSupportConditionsButton' not found!";
    }
    layoutAddSurfaceSupportButton = findChild<QPushButton *>("layoutAddSurfaceSupportButton");
    if (layoutAddSurfaceSupportButton) {
        connect(layoutAddSurfaceSupportButton,
                &QPushButton::clicked,
                this,
                &SlabGUI::on_addSurfaceSupportButton_clicked);
    } else {
        qWarning() << "Button 'layoutAddSurfaceSupportButton' not found!";
    }

}




void SlabGUI::loadPropertiesLayout()
{
    {
        loadLayoutFromFile(":/ui/slabProperties_layout.ui");

        // Znajdź przycisk w załadowanym layoutcie
        addMaterialButton = findChild<QPushButton *>("addMaterialButton");
        if (addMaterialButton) {
            connect(addMaterialButton, &QPushButton::clicked, this, &SlabGUI::on_addMaterialButton_clicked);
        } else {
            qWarning() << "Button 'addMaterialButton' not found!";
        }

        setPropertiesButton = findChild<QPushButton *>("setPropertiesButton");
        if (setPropertiesButton) {
            connect(setPropertiesButton,
                    &QPushButton::clicked,
                    this,
                    &SlabGUI::on_setPropertiesButton_clicked);
        } else {
            qWarning() << "Button 'setPropertiesButton' not found!";
        }
    }
}
void SlabGUI::on_setPropertiesButton_clicked()
{
    cout << "setPropertiesButton button clicked" << endl;
    slabSetPropertiesDialog *dialog = new slabSetPropertiesDialog(this);
    dataBaseMaterialsManager->iterateOverTable();
    dialog->setMaterials(dataBaseMaterialsManager->getMaterialsMap());
    dialog->updateMaterialsList();
    dialog->moveToBottomLeft();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &slabSetPropertiesDialog::accepted, this, [this, dialog]() {
        int materialId = dialog->getMaterialId();
        int thickness = dialog->getThickness();

        dataBaseSurfacesManager->updateObjectInDataBase(TableType::SURFACES, 1, "material_id", std::to_string(materialId));
        dataBaseSurfacesManager->updateObjectInDataBase(TableType::SURFACES, 1, "thickness", std::to_string(thickness));

        SlabGUI::on_setPropertiesButton_clicked();
    });

    connect(dialog, &slabSetPropertiesDialog::rejected, dialog, &slabSetPropertiesDialog::deleteLater);

    dialog->show();
}

void SlabGUI::on_addMaterialButton_clicked()
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

        SlabGUI::on_addMaterialButton_clicked();
    });

    connect(dialog, &AddMaterialDialog::rejected, dialog, &AddMaterialDialog::deleteLater);
    dialog->show();
}

void SlabGUI::loadLoadsLayout()
{
    loadLayoutFromFile(":/ui/slabLoads_layout.ui");

    addPointAppliedForceButton = findChild<QPushButton *>("addPointAppliedForceButton");
    if (addPointAppliedForceButton) {
        connect(addPointAppliedForceButton,
                &QPushButton::clicked,
                this,
                &SlabGUI::on_addPointAppliedForceButton_clicked);
    } else {
        qWarning() << "Button 'addPointAppliedForceButton' not found!";
    }
    addLineLoadButton = findChild<QPushButton *>("addLineLoadButton");
    if (addLineLoadButton) {
        connect(addLineLoadButton, &QPushButton::clicked, this, &SlabGUI::on_addLineLoadButton_clicked);
    } else {
        qWarning() << "Button 'addLineLoadButton' not found!";
    }
    openLoadsManagerButton = findChild<QPushButton *>("openLoadsManagerButton");
    if (openLoadsManagerButton) {
        connect(openLoadsManagerButton,
                &QPushButton::clicked,
                this,
                &SlabGUI::on_openLoadsManagerButton_clicked);
    } else {
        qWarning() << "Button 'openLoadsManagerButton' not found!";
    }
    addSurfaceLoadButton = findChild<QPushButton *>("addSurfaceLoadButton");
    if (addSurfaceLoadButton) {
        connect(addSurfaceLoadButton,
                &QPushButton::clicked,
                this,
                &SlabGUI::on_addSurfaceLoadButton_clicked);
    } else {
        qWarning() << "Button 'addSurfaceLoadButton' not found!";
    }
}

void SlabGUI::loadMeshLayout() {
    // Load the UI layout from the file
    loadLayoutFromFile(":/ui/slabMesh_layout.ui");

    // Find the main layout and widgets in the loaded UI
    QVBoxLayout *mainLayout = findChild<QVBoxLayout*>("mainLayout");
    QScrollArea *scrollArea = findChild<QScrollArea*>("scrollArea");
    QWidget *scrollAreaWidgetContents = findChild<QWidget*>("scrollAreaWidgetContents");
    QVBoxLayout *dynamicLayout = findChild<QVBoxLayout*>("dynamicLayout");
    QLabel *numberOfFeLabel = findChild<QLabel*>("numberOfFeLabel");
    QPushButton *applyButton = findChild<QPushButton*>("applyButton");
    QPushButton *showMeshButton = findChild<QPushButton*>("showMeshButton");

    if (!mainLayout || !scrollArea || !scrollAreaWidgetContents || !dynamicLayout || !numberOfFeLabel || !applyButton || !showMeshButton) {
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

        QLabel *label = new QLabel(QString("Line ID %1:").arg(line.id), this);
        hLayout->addWidget(label);
        qDebug() << "Label created and added for Line ID:" << line.id;

        QSlider *slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(10, 200); // Set the range of the slider (10-200)

        // Set the slider value to the previously stored value or the default (10)
        int sliderValue = lineSliderValues.value(line.id, 10);
        slider->setValue(sliderValue);

        slider->setStyleSheet(sliderStyle);  // Apply the same style to horizontal sliders
        hLayout->addWidget(slider);
        qDebug() << "Slider created and added for Line ID:" << line.id;

        QLabel *valueLabel = new QLabel(QString::number(slider->value()), this);
        hLayout->addWidget(valueLabel);
        qDebug() << "Value label created and added for Line ID:" << line.id;

        // Connect the slider's valueChanged signal to update the value label and store the value
        connect(slider, &QSlider::valueChanged, [this, line, valueLabel](int value) {
            valueLabel->setText(QString::number(value));
            lineSliderValues[line.id] = value;  // Store the value in the map
            handleSliderValueChanged(value, line.id);
        });
        qDebug() << "Slider valueChanged signal connected for Line ID:" << line.id;

        // Add the horizontal layout to the dynamic layout
        dynamicLayout->addLayout(hLayout);
        qDebug() << "Horizontal layout added to dynamic layout for Line ID:" << line.id;

        // Optionally, call handleSliderValueChanged with the stored value
        handleSliderValueChanged(sliderValue, line.id);
    }

    // Populate the dynamic layout with QLabel and QSlider for each Circular Line's id
    for (const auto &circularLine : circularLines) {
        qDebug() << "Processing Circular Line ID:" << circularLine.id;

        QHBoxLayout *hLayout = new QHBoxLayout();

        QLabel *label = new QLabel(QString("Circular Line ID %1:").arg(circularLine.id), this);
        hLayout->addWidget(label);
        qDebug() << "Label created and added for Circular Line ID:" << circularLine.id;

        QSlider *slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(1, 12); // Set the range of the slider (1-12)

        // Set the slider value to the previously stored value or the default (10)
        int sliderValue = circularLineSliderValues.value(circularLine.id, 10);
        slider->setValue(sliderValue);

        slider->setStyleSheet(sliderStyle);  // Apply the same style to horizontal sliders
        hLayout->addWidget(slider);
        qDebug() << "Slider created and added for Circular Line ID:" << circularLine.id;

        QLabel *valueLabel = new QLabel(QString::number(slider->value()), this);
        hLayout->addWidget(valueLabel);
        qDebug() << "Value label created and added for Circular Line ID:" << circularLine.id;

        // Connect the slider's valueChanged signal to update the value label and store the value
        connect(slider, &QSlider::valueChanged, [this, circularLine, valueLabel](int value) {
            valueLabel->setText(QString::number(value));
            circularLineSliderValues[circularLine.id] = value;  // Store the value in the map
            handleSliderValueChanged(value, circularLine.id);
        });
        qDebug() << "Slider valueChanged signal connected for Circular Line ID:" << circularLine.id;

        // Add the horizontal layout to the dynamic layout
        dynamicLayout->addLayout(hLayout);
        qDebug() << "Horizontal layout added to dynamic layout for Circular Line ID:" << circularLine.id;

        // Optionally, call handleSliderValueChanged with the stored value
        handleSliderValueChanged(sliderValue, circularLine.id);
    }

    dynamicLayout->addStretch();  // Add stretch at the end for better layout
    qDebug() << "Stretch added to dynamic layout";

    // Set the Apply button behavior
    connect(applyButton, &QPushButton::clicked, this, &SlabGUI::on_applyButton_clicked);
    connect(showMeshButton, &QPushButton::clicked, this, &SlabGUI::on_showMeshButton_clicked);

    qDebug() << "Load mesh layout complete.";
}


void SlabGUI::saveSliderValues() {
    QSettings settings("MyCompany", "MyApp");

    // Save line slider values
    for (auto it = lineSliderValues.begin(); it != lineSliderValues.end(); ++it) {
        settings.setValue(QString("lineSlider/%1").arg(it.key()), it.value());
    }

    // Save circular line slider values
    for (auto it = circularLineSliderValues.begin(); it != circularLineSliderValues.end(); ++it) {
        settings.setValue(QString("circularLineSlider/%1").arg(it.key()), it.value());
    }
}

void SlabGUI::loadSliderValues() {
    QSettings settings("MyCompany", "MyApp");

    // Load line slider values
    for (const auto &line : lines) {
        int value = settings.value(QString("lineSlider/%1").arg(line.id), 10).toInt();
        lineSliderValues[line.id] = value;
    }

    // Load circular line slider values
    for (const auto &circularLine : circularLines) {
        int value = settings.value(QString("circularLineSlider/%1").arg(circularLine.id), 10).toInt();
        circularLineSliderValues[circularLine.id] = value;
    }
}

void SlabGUI::handleSliderValueChanged(int value, int lineId)
{
    qDebug() << "Slider value changed for Line ID (or Circular Line ID)" << lineId << ": " << value;

    // Find the line or circular line with the given lineId
    auto lineIt = std::find_if(lines.begin(), lines.end(), [lineId](const Line &line) {
        return line.id == lineId;
    });

    auto circularLineIt = std::find_if(circularLines.begin(), circularLines.end(), [lineId](const CircularLine &circularLine) {
        return circularLine.id == lineId;
    });

    if (lineIt != lines.end() || circularLineIt != circularLines.end()) {
        // Find the existing Mesh entry in meshVector for this lineId
        auto meshIt = std::find_if(meshVector.begin(), meshVector.end(), [lineId](const Mesh &mesh) {
            return mesh.lineId == lineId;
        });

        if (meshIt != meshVector.end()) {
            // Update the existing entry with the new number of finite elements (numberOfFE)
            meshIt->numberOfFE = value;
            qDebug() << "Updated Mesh entry for Line ID (or Circular Line ID)" << lineId << " with new numberOfFE: " << value;
        } else {
            // Add a new Mesh entry if it doesn't exist
            meshVector.push_back({lineId, value});
            qDebug() << "Added new Mesh entry for Line ID (or Circular Line ID)" << lineId << " with numberOfFE: " << value;
        }
    } else {
        qWarning() << "Line ID (or Circular Line ID)" << lineId << " not found!";
    }
}


void SlabGUI::on_showMeshButton_clicked() {}

void SlabGUI::on_applyButton_clicked()
{
    qDebug() << "Apply button clicked! Processing mesh nodes...";

    // Step 1: Validate the database connection
    if (!dataBaseSlabMeshManager) {
        qWarning() << "Database connection is not initialized!";
        return;
    }

    // Step 2: Clear the MESH table before populating it again
    dataBaseSlabMeshManager->clearTable();

    for (const auto &meshLine : meshVector) {

        dataBaseSlabMeshManager->addObjectToDataBase(meshLine.lineId, meshLine.numberOfFE);
    }

    SlabGUI::on_refreshButton_clicked();
}



void SlabGUI::loadResultsLayout()
{

}
void SlabGUI::on_editObjectButton_clicked()
{

}

void SlabGUI::on_addPointAppliedForceButton_clicked()
{
    cout << "Add point load clicked" << std::endl;
    AddSlabPointLoadDialog *dialog = new AddSlabPointLoadDialog(this);
    dialog->moveToBottomLeft();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &AddSlabPointLoadDialog::accepted, this, [this, dialog]() {
        int x = dialog->getX();
        int z = dialog->getZ();
        double force = dialog->getF();
        dataBaseSlabPointLoadManager->addObjectToDataBase(force, x, z);

        SlabGUI::on_refreshButton_clicked();
        SlabGUI::on_addPointAppliedForceButton_clicked();

    });

    connect(dialog, &AddSlabPointLoadDialog::rejected, dialog, &AddSlabPointLoadDialog::deleteLater);
    dialog->show();
}


void SlabGUI::on_addLineLoadButton_clicked()
{
    cout << "add line load button clicked"
         << std::endl;
    AddSlabLineLoadDialog *dialog = new AddSlabLineLoadDialog(this);
    dialog->moveToBottomLeft();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &AddSlabLineLoadDialog::accepted, this, [this, dialog]() {
        int x1 = dialog->getX1();
        int z1 = dialog->getZ1();
        int x2 = dialog->getX2();
        int z2 = dialog->getZ2();
        int force = dialog->getF();

        dataBaseSlabLineLoadsManager->addObjectToDataBase(x1,z1,x2,z2,force);

        SlabGUI::on_refreshButton_clicked();
        SlabGUI::on_addLineLoadButton_clicked();
    });
    connect(dialog, &AddSlabLineLoadDialog::rejected, dialog, &AddSlabLineLoadDialog::deleteLater);
    dialog->show();

}

void SlabGUI::on_addSurfaceLoadButton_clicked()
{
    cout << "Add surface load clicked" << std::endl;
    AddSurfaceLoadDialog *dialog = new AddSurfaceLoadDialog(this);
    dialog->moveToBottomLeft();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &AddSurfaceLoadDialog::accepted, this, [this, dialog]() {
        int x1 = dialog->getX1();
        int z1 = dialog->getZ1();
        int x2 = dialog->getX2();
        int z2 = dialog->getZ2();
        int force = dialog->getF();

        dataBaseSurfaceLoadsManager->addObjectToDataBase(x1,z1,x2,z2,force);

        SlabGUI::on_refreshButton_clicked();
        SlabGUI::on_addSurfaceLoadButton_clicked();
    });
    connect(dialog, &AddSurfaceLoadDialog::rejected, dialog, &AddSurfaceLoadDialog::deleteLater);
    dialog->show();
}


//TODO - implement loads manager
void SlabGUI::on_openLoadsManagerButton_clicked(){}

void SlabGUI::onComboBoxIndexChanged(int index)
{
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
            // on_refreshButton_clicked();
            loadMeshLayout();
            break;
        case 4:
            loadResultsLayout();

        default:
            break;
        }
    }
}


void SlabGUI::on_addSurfaceButton_clicked()
{
    std::cout << "Surface button clicked" << std::endl;
    AddSurfaceDialog *dialog = new AddSurfaceDialog(this);
    dialog->moveToBottomLeft();
    dialog->initializeWithType(surfaceLayoutType);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &AddSurfaceDialog::accepted, this, [this, dialog]() {
        QString selectedType = dialog->getSelectedObjectType();
        bool isOpening = dialog->getIsOpening();
        std::cout << "Selected type: " << selectedType.toStdString() << ", isOpening: " << isOpening << std::endl;

        if (!isOpening && dataBaseSurfacesManager->hasNonOpeningSurface()) {
            QMessageBox::warning(this, "Invalid Operation", "A main surface already exists. You cannot add another one.");
            std::cout << "A main surface already exists. Aborting." << std::endl;
            return;
        }

        if (isOpening && !dataBaseSurfacesManager->hasNonOpeningSurface()) {
            QMessageBox::warning(this, "Invalid Operation", "You cannot add an opening without a main surface defined.");
            std::cout << "No main surface exists. Aborting." << std::endl;
            return;
        }

        int x1 = dialog->getX1();
        int z1 = dialog->getZ1();
        int x2 = dialog->getX2();
        int z2 = dialog->getZ2();
        int x3 = 0, z3 = 0, x4 = 0, z4 = 0;

        int triX1 = dialog->getTriX1();
        int triZ1 = dialog->getTriZ1();
        int triX2 = dialog->getTriX2();
        int triZ2 = dialog->getTriZ2();
        int triX3 = dialog->getTriX3();
        int triZ3 = dialog->getTriZ3();

        int diameter = dialog->getDiameter();
        int centreX1 = dialog->getCentreX1();
        int centreZ1 = dialog->getCentreZ1();

        if (isOpening) {
            auto mainSurfaceTuple = dataBaseSurfacesManager->getMainSurface();

            Surface mainSurface;
            mainSurface.surfaceType = std::get<0>(mainSurfaceTuple);
            mainSurface.line1Id = std::get<1>(mainSurfaceTuple);
            mainSurface.line2Id = std::get<2>(mainSurfaceTuple);
            mainSurface.line3Id = std::get<3>(mainSurfaceTuple);
            mainSurface.line4Id = std::get<4>(mainSurfaceTuple);
            mainSurface.circularLineId = std::get<5>(mainSurfaceTuple);
            mainSurface.materialId = std::get<6>(mainSurfaceTuple);
            mainSurface.thickness = std::get<7>(mainSurfaceTuple);
            mainSurface.isOpening = std::get<8>(mainSurfaceTuple);

            std::cout << "Main surface type: " << mainSurface.surfaceType << std::endl;

            bool isWithinMainSurface = false;

            if (selectedType == "Rectangle") {
                x3 = x1;
                z3 = z2;
                x4 = x2;
                z4 = z1;
                std::cout << "New rectangle coordinates: (" << x1 << ", " << z1 << "), (" << x2 << ", " << z2 << ")" << std::endl;
                isWithinMainSurface = isRectangleWithin(mainSurface, x1, z1, x2, z2);
            } else if (selectedType == "Triangle") {
                std::cout << "New triangle coordinates: (" << triX1 << ", " << triZ1 << "), (" << triX2 << ", " << triZ2 << "), (" << triX3 << ", " << triZ3 << ")" << std::endl;
                isWithinMainSurface = isTriangleWithin(mainSurface, triX1, triZ1, triX2, triZ2, triX3, triZ3);
            } else if (selectedType == "Circle") {
                std::cout << "New circle: center (" << centreX1 << ", " << centreZ1 << "), diameter: " << diameter << std::endl;
                isWithinMainSurface = isCircleWithin(mainSurface, centreX1, centreZ1, diameter);
            }

            std::cout << "Is within main surface: " << (isWithinMainSurface ? "Yes" : "No") << std::endl;

            if (!isWithinMainSurface) {
                QMessageBox::warning(this, "Invalid Operation", "The opening must be within the area of the main surface.");
                return;
            }
        }

        if (selectedType == "Rectangle") {
            std::cout << "Adding a rectangle." << std::endl;
            x3 = x1;
            z3 = z2;
            x4 = x2;
            z4 = z1;

            dataBasePointsManager->addObjectToDataBase(x1, z1);
            int id1 = dataBasePointsManager->getLastInsertedRowID();
            std::cout << "Point 1 ID: " << id1 << std::endl;

            dataBasePointsManager->addObjectToDataBase(x2, z2);
            int id2 = dataBasePointsManager->getLastInsertedRowID();
            std::cout << "Point 2 ID: " << id2 << std::endl;

            dataBasePointsManager->addObjectToDataBase(x3, z3);
            int id3 = dataBasePointsManager->getLastInsertedRowID();
            std::cout << "Point 3 ID: " << id3 << std::endl;

            dataBasePointsManager->addObjectToDataBase(x4, z4);
            int id4 = dataBasePointsManager->getLastInsertedRowID();
            std::cout << "Point 4 ID: " << id4 << std::endl;

            dataBaseLinesManager->addObjectToDataBaseConsideringCircularLines(id1, id3);
            int line1 = dataBaseLinesManager->getLastInsertedRowID();
            std::cout << "Line 1 ID: " << line1 << std::endl;

            dataBaseLinesManager->addObjectToDataBaseConsideringCircularLines(id3, id2);
            int line2 = dataBaseLinesManager->getLastInsertedRowID();
            std::cout << "Line 2 ID: " << line2 << std::endl;

            dataBaseLinesManager->addObjectToDataBaseConsideringCircularLines(id2, id4);
            int line3 = dataBaseLinesManager->getLastInsertedRowID();
            std::cout << "Line 3 ID: " << line3 << std::endl;

            dataBaseLinesManager->addObjectToDataBaseConsideringCircularLines(id4, id1);
            int line4 = dataBaseLinesManager->getLastInsertedRowID();
            std::cout << "Line 4 ID: " << line4 << std::endl;

            dataBaseSurfacesManager->addObjectToDataBase(line1, line2, line3, line4, 1, 100, isOpening);
            std::cout << "Rectangle surface added." << std::endl;

        } else if (selectedType == "Circle") {
            std::cout << "Adding a circle." << std::endl;

            dataBaseCircularLinesManager->addObjectToDataBase(centreX1, centreZ1, diameter);
            int circularLineId = dataBaseCircularLinesManager->getLastInsertedRowID();
            std::cout << "Circular line ID: " << circularLineId << std::endl;

            dataBaseSurfacesManager->addObjectToDataBase(circularLineId, -1, 100, isOpening);
            std::cout << "Circular surface added." << std::endl;

        } else if (selectedType == "Triangle") {
            std::cout << "Adding a triangle." << std::endl;

            dataBasePointsManager->addObjectToDataBase(triX1, triZ1);
            int id1 = dataBasePointsManager->getLastInsertedRowID();
            std::cout << "Point 1 ID: " << id1 << std::endl;

            dataBasePointsManager->addObjectToDataBase(triX2, triZ2);
            int id2 = dataBasePointsManager->getLastInsertedRowID();
            std::cout << "Point 2 ID: " << id2 << std::endl;

            dataBasePointsManager->addObjectToDataBase(triX3, triZ3);
            int id3 = dataBasePointsManager->getLastInsertedRowID();
            std::cout << "Point 3 ID: " << id3 << std::endl;

            dataBaseLinesManager->addObjectToDataBaseConsideringCircularLines(id1, id2);
            int line1 = dataBaseLinesManager->getLastInsertedRowID();
            std::cout << "Line 1 ID: " << line1 << std::endl;

            dataBaseLinesManager->addObjectToDataBaseConsideringCircularLines(id2, id3);
            int line2 = dataBaseLinesManager->getLastInsertedRowID();
            std::cout << "Line 2 ID: " << line2 << std::endl;

            dataBaseLinesManager->addObjectToDataBaseConsideringCircularLines(id3, id1);
            int line3 = dataBaseLinesManager->getLastInsertedRowID();
            std::cout << "Line 3 ID: " << line3 << std::endl;

            dataBaseSurfacesManager->addObjectToDataBase(line1, line2, line3, -1, 100, isOpening);
            std::cout << "Triangle surface added." << std::endl;
        }

        std::cout << "Refreshing surfaces..." << std::endl;
        SlabGUI::on_addSurfaceButton_clicked();  // Consider revising this recursive call
        SlabGUI::on_refreshButton_clicked();
    });

    connect(dialog, &AddSurfaceDialog::rejected, dialog, &dialog->deleteLater);
    dialog->show();
}





bool SlabGUI::isRectangleWithin(const Surface &mainSurface, int x1, int z1, int x2, int z2)
{
    // Calculate the actual top-left and bottom-right points for the new rectangle
    QPointF newTopLeft(std::min(x1, x2), std::min(z1, z2));
    QPointF newBottomRight(std::max(x1, x2), std::max(z1, z2));

    // Handle different types of main surfaces
    if (mainSurface.surfaceType == "rectangle") {
        // Fetch the main rectangle's points from the database
        QPointF mainTopLeft = getCoordinatesFromLine(mainSurface.line1Id);   // Line1 corresponds to top-left to bottom-left
        QPointF mainBottomRight = getCoordinatesFromLine(mainSurface.line3Id); // Line3 corresponds to bottom-right to top-right

        // Ensure we are working with a positive coordinate system without flipping
        mainTopLeft.setY(-mainTopLeft.y());        // Convert to a positive coordinate system
        mainBottomRight.setY(-mainBottomRight.y()); // Convert to a positive coordinate system

        // Adjust the logic to compare the rectangles in the positive coordinate system
        bool withinX = newTopLeft.x() >= mainTopLeft.x() && newBottomRight.x() <= mainBottomRight.x();
        bool withinY = newTopLeft.y() >= mainTopLeft.y() && newBottomRight.y() <= mainBottomRight.y();

        return withinX && withinY;

    } else if (mainSurface.surfaceType == "triangle") {
        // Fetch the main triangle's points from the database
        QPointF mainP1 = getCoordinatesFromLine(mainSurface.line1Id);
        QPointF mainP2 = getCoordinatesFromLine(mainSurface.line2Id);
        QPointF mainP3 = getCoordinatesFromLine(mainSurface.line3Id);

        mainP1.setY(-mainP1.y());
        mainP2.setY(-mainP2.y());
        mainP3.setY(-mainP3.y());

        // Check if all four corners of the new rectangle are within the main triangle
        QPointF newP1 = newTopLeft;
        QPointF newP2(newTopLeft.x(), newBottomRight.y());
        QPointF newP3(newBottomRight.x(), newBottomRight.y());
        QPointF newP4(newBottomRight.x(), newTopLeft.y());

        return isPointInTriangle(newP1, mainP1, mainP2, mainP3) &&
               isPointInTriangle(newP2, mainP1, mainP2, mainP3) &&
               isPointInTriangle(newP3, mainP1, mainP2, mainP3) &&
               isPointInTriangle(newP4, mainP1, mainP2, mainP3);

    } else if (mainSurface.surfaceType == "circle") {
        // Fetch the main circle's center and radius from the database
        QPointF mainCenter = getCoordinatesFromLine(mainSurface.circularLineId);
        int mainRadius = getCircularLineRadius(mainSurface.circularLineId);

        mainCenter.setY(-mainCenter.y());

        // Check if all four corners of the new rectangle are within the main circle
        QPointF newP1 = newTopLeft;
        QPointF newP2(newTopLeft.x(), newBottomRight.y());
        QPointF newP3(newBottomRight.x(), newBottomRight.y());
        QPointF newP4(newBottomRight.x(), newTopLeft.y());

        return isPointInCircle(newP1, mainCenter, mainRadius) &&
               isPointInCircle(newP2, mainCenter, mainRadius) &&
               isPointInCircle(newP3, mainCenter, mainRadius) &&
               isPointInCircle(newP4, mainCenter, mainRadius);
    }

    return false; // In case the surface type is not recognized
}

bool SlabGUI::isPointInCircle(const QPointF &point, const QPointF &center, float radius)
{
    float distance = std::sqrt(std::pow(point.x() - center.x(), 2) + std::pow(point.y() - center.y(), 2));
    return distance <= radius;
}



QPointF SlabGUI::getCoordinatesFromLine(int lineId) {
    // Fetch the start point ID of the line
    std::string startPointIdStr = dataBaseLinesManager->selectObjectPropertyByID(TableType::LINES, lineId, "start_point");

    try {
        int startPointId = std::stoi(startPointIdStr);
        // Fetch the x and z coordinates from the points table
        std::string xCoordStr = dataBasePointsManager->selectObjectPropertyByID(TableType::POINTS, startPointId, "x_cord");
        std::string zCoordStr = dataBasePointsManager->selectObjectPropertyByID(TableType::POINTS, startPointId, "z_cord");

        float xCoord = std::stof(xCoordStr);
        float zCoord = std::stof(zCoordStr);

        return QPointF(xCoord, -zCoord);  // Convert to QPointF with correct sign for z

    } catch (const std::exception& e) {
        std::cerr << "Error: Exception in getCoordinatesFromLine: " << e.what() << std::endl;
        return QPointF(-1, -1); // Return a default value or handle the error as needed
    }
}

bool SlabGUI::isTriangleWithin(const Surface &mainSurface, int triX1, int triZ1, int triX2, int triZ2, int triX3, int triZ3)
{
    if (mainSurface.surfaceType == "rectangle") {
        // Get the bounding box of the rectangular main surface
        QPointF mainTopLeft = getCoordinatesFromLine(mainSurface.line1Id);
        QPointF mainBottomRight = getCoordinatesFromLine(mainSurface.line3Id);

        mainTopLeft.setY(-mainTopLeft.y());
        mainBottomRight.setY(-mainBottomRight.y());

        QPointF triP1(triX1, triZ1);
        QPointF triP2(triX2, triZ2);
        QPointF triP3(triX3, triZ3);

        // Check if all points are within the rectangle bounds
        bool withinP1 = (triP1.x() >= mainTopLeft.x() && triP1.x() <= mainBottomRight.x()) &&
                        (triP1.y() >= mainTopLeft.y() && triP1.y() <= mainBottomRight.y());

        bool withinP2 = (triP2.x() >= mainTopLeft.x() && triP2.x() <= mainBottomRight.x()) &&
                        (triP2.y() >= mainTopLeft.y() && triP2.y() <= mainBottomRight.y());

        bool withinP3 = (triP3.x() >= mainTopLeft.x() && triP3.x() <= mainBottomRight.x()) &&
                        (triP3.y() >= mainTopLeft.y() && triP3.y() <= mainBottomRight.y());

        return withinP1 && withinP2 && withinP3;

    } else if (mainSurface.surfaceType == "circle") {
        // Get the center and radius of the circular main surface
        QPointF mainCenter = getCoordinatesFromLine(mainSurface.circularLineId);
        int mainRadius = getCircularLineRadius(mainSurface.circularLineId);

        mainCenter.setY(-mainCenter.y());

        QPointF triP1(triX1, triZ1);
        QPointF triP2(triX2, triZ2);
        QPointF triP3(triX3, triZ3);

        // Check if all points are within the circle bounds
        bool withinP1 = (std::sqrt(std::pow(triP1.x() - mainCenter.x(), 2) + std::pow(triP1.y() - mainCenter.y(), 2)) <= mainRadius);
        bool withinP2 = (std::sqrt(std::pow(triP2.x() - mainCenter.x(), 2) + std::pow(triP2.y() - mainCenter.y(), 2)) <= mainRadius);
        bool withinP3 = (std::sqrt(std::pow(triP3.x() - mainCenter.x(), 2) + std::pow(triP3.y() - mainCenter.y(), 2)) <= mainRadius);

        return withinP1 && withinP2 && withinP3;

    } else if (mainSurface.surfaceType == "triangle") {
        // Get the main triangle points
        QPointF mainP1 = getCoordinatesFromLine(mainSurface.line1Id);
        QPointF mainP2 = getCoordinatesFromLine(mainSurface.line2Id);
        QPointF mainP3 = getCoordinatesFromLine(mainSurface.line3Id);

        mainP1.setY(-mainP1.y());
        mainP2.setY(-mainP2.y());
        mainP3.setY(-mainP3.y());

        QPointF newP1(triX1, triZ1);
        QPointF newP2(triX2, triZ2);
        QPointF newP3(triX3, triZ3);

        // Use the point-in-triangle test for the triangular main surface
        return isPointInTriangle(newP1, mainP1, mainP2, mainP3) &&
               isPointInTriangle(newP2, mainP1, mainP2, mainP3) &&
               isPointInTriangle(newP3, mainP1, mainP2, mainP3);
    }

    return false; // In case the surface type is not recognized
}


bool SlabGUI::isCircleWithin(const Surface &mainSurface, int centreX1, int centreZ1, int diameter)
{
    QPointF newCenter(centreX1, centreZ1);
    int newRadius = diameter / 2;

    if (mainSurface.surfaceType == "rectangle") {
        // Treat the main surface as a rectangle and check if the circle is within it
        QPointF mainTopLeft = getCoordinatesFromLine(mainSurface.line1Id);
        QPointF mainBottomRight = getCoordinatesFromLine(mainSurface.line3Id);

        // Ensure correct orientation
        mainTopLeft.setY(-mainTopLeft.y());
        mainBottomRight.setY(-mainBottomRight.y());

        // Check if the circle is within the rectangle bounds
        bool withinX = (newCenter.x() - newRadius >= mainTopLeft.x()) &&
                       (newCenter.x() + newRadius <= mainBottomRight.x());
        bool withinY = (newCenter.y() - newRadius >= mainTopLeft.y()) &&
                       (newCenter.y() + newRadius <= mainBottomRight.y());

        return withinX && withinY;

    } else if (mainSurface.surfaceType == "triangle") {
        // Treat the main surface as a triangle and check if the circle is within it
        QPointF mainP1 = getCoordinatesFromLine(mainSurface.line1Id);
        QPointF mainP2 = getCoordinatesFromLine(mainSurface.line2Id);
        QPointF mainP3 = getCoordinatesFromLine(mainSurface.line3Id);

        mainP1.setY(-mainP1.y());
        mainP2.setY(-mainP2.y());
        mainP3.setY(-mainP3.y());

        // For simplicity, we'll check if the circle's center is within the triangle
        // and that the circle fits inside the triangle without intersecting its edges.

        if (isPointInTriangle(newCenter, mainP1, mainP2, mainP3)) {
            // Check if the circle's radius does not cause it to go outside the triangle
            return isCircleWithinTriangle(mainP1, mainP2, mainP3, newCenter, newRadius);
        }
        return false;

    } else if (mainSurface.surfaceType == "circle") {
        // Handle the circle-within-circle check
        QPointF mainCenter = getCoordinatesFromLine(mainSurface.circularLineId);
        int mainRadius = getCircularLineRadius(mainSurface.circularLineId);

        mainCenter.setY(-mainCenter.y());

        float distance = std::sqrt(std::pow(newCenter.x() - mainCenter.x(), 2) + std::pow(newCenter.y() - mainCenter.y(), 2));

        return (distance + newRadius) <= mainRadius;
    }

    return false; // In case the surface type is not recognized
}

bool SlabGUI::isCircleWithinTriangle(const QPointF &v1, const QPointF &v2, const QPointF &v3, const QPointF &center, float radius)
{
    // Check if the circle fits inside the triangle by verifying the distance from the center to each side
    // Ensure that the circle does not intersect the triangle's sides

    return (distanceToLine(v1, v2, center) >= radius) &&
           (distanceToLine(v2, v3, center) >= radius) &&
           (distanceToLine(v3, v1, center) >= radius);
}

float SlabGUI::distanceToLine(const QPointF &v1, const QPointF &v2, const QPointF &p)
{
    // Calculate the perpendicular distance from point p to the line defined by v1 and v2
    return std::abs((v2.y() - v1.y()) * p.x() - (v2.x() - v1.x()) * p.y() + v2.x() * v1.y() - v2.y() * v1.x()) /
           std::sqrt(std::pow(v2.y() - v1.y(), 2) + std::pow(v2.x() - v1.x(), 2));
}



bool SlabGUI::isPointInTriangle(const QPointF &pt, const QPointF &v1, const QPointF &v2, const QPointF &v3)
{
    // Calculate if the point pt is inside the triangle v1, v2, v3
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

float SlabGUI::sign(const QPointF &p1, const QPointF &p2, const QPointF &p3)
{
    return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
}


int SlabGUI::getCircularLineRadius(int circularLineId) {
    // Fetch the diameter from the circular_lines table
    std::string diameterStr = dataBaseCircularLinesManager->selectObjectPropertyByID(TableType::CIRCULAR_LINES, circularLineId, "diameter");

    try {
        int diameter = std::stoi(diameterStr);
        return diameter / 2;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid argument in getCircularLineRadius, diameterStr: " << diameterStr << std::endl;
        return -1; // Return a default value or handle the error as needed
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Out of range in getCircularLineRadius, diameterStr: " << diameterStr << std::endl;
        return -1; // Return a default value or handle the error as needed
    }
}


void SlabGUI::paintPoints(QPainter &painter)
{
    painter.setBrush(Qt::blue);
    painter.setPen(Qt::blue);

    for (const auto &point : points) {
        // Draw the point as a circle (ellipse)
        painter.drawEllipse(QPointF(point.x, -point.z), 75, 75); // Increase the radius of the ellipse to 75
    }
}

void SlabGUI::paintPointsLabels(QPainter &painter)
{
    painter.setPen(Qt::blue);

    QFont font = painter.font();
    font.setPointSize(150); // Increase the font size 15 times
    painter.setFont(font);

    for (const auto &point : points) {
        // Draw the text label slightly offset from the center of the circle
        painter.drawText(QPointF(point.x + 85, -point.z - 85),
                         QString::number(point.id)); // Increase the label offset
    }
}


void SlabGUI::paintLines(QPainter &painter)
{
    // Set the pen to draw thicker black lines
    QPen pen(Qt::black, 24); // Increase line thickness and set color to black
    painter.setPen(pen);

    for (const auto &line : lines) {
        // Draw the line between the start and end points
        painter.drawLine(QPointF(line.startX, -line.startZ), QPointF(line.endX, -line.endZ));
    }
}

void SlabGUI::paintLinesLabels(QPainter &painter)
{
    painter.setPen(Qt::black); // Use black color for text

    QFont font = painter.font();
    font.setPointSize(150); // Increase font size 15 times
    painter.setFont(font);

    for (const auto &line : lines) {
        // Calculate the position for the text label (line ID)
        QPointF textPosition((line.startX + line.endX) / 2 + 15,
                             (-line.startZ + -line.endZ) / 2 - 15);
        // Draw the line ID at the calculated position
        painter.drawText(textPosition, QString::number(line.id));
    }
}
void SlabGUI::paintCircularLines(QPainter &painter)
{
    // Set the pen to draw thicker black lines
    QPen pen(Qt::black, 24); // Line color: black, Thickness: 24
    painter.setPen(pen);

    // Set the brush to NoBrush to make the circle transparent (no fill)
    painter.setBrush(Qt::NoBrush);

    for (const auto &circularLine : circularLines) {
        // Calculate the center point (considering the coordinate system)
        QPointF center(circularLine.centreX, -circularLine.centreZ);

        // Define the bounding rectangle for the circle based on the diameter
        QRectF boundingRect(
            center.x() - static_cast<float>(circularLine.diameter) / 2.0f,
            center.y() - static_cast<float>(circularLine.diameter) / 2.0f,
            static_cast<float>(circularLine.diameter),
            static_cast<float>(circularLine.diameter)
            );

        // Draw the circle (ellipse with equal width and height)
        painter.drawEllipse(boundingRect);
    }
}
void SlabGUI::paintCircularLinesLabels(QPainter &painter)
{
    // Set pen color for the text
    painter.setPen(Qt::black); // Text color: black

    // Configure the font
    QFont font = painter.font();
    font.setPointSize(150); // Adjust font size as needed (original was 150, which might be too large)
    painter.setFont(font);

    const int offset = 20; // Offset in pixels to position the label outside the circle

    for (const auto &circularLine : circularLines) {
        // Calculate the label position to the right of the circle
        float labelX = circularLine.centreX + static_cast<float>(circularLine.diameter) / 2.0f + offset;
        float labelY = -circularLine.centreZ; // Keeping the same Y position

        QPointF textPosition(labelX, labelY);

        // Optionally, adjust the Y position slightly for better alignment
        // For example, to center the text vertically relative to the circle
        // You might need to calculate the text's height and adjust accordingly
        // Here, we'll add a small vertical offset for better aesthetics
        float verticalOffset = 5.0f; // Adjust as needed
        textPosition.setY(textPosition.y() + verticalOffset);

        // Draw the circular line ID at the calculated position
        painter.drawText(textPosition, QString::number(circularLine.id));
    }
}

void SlabGUI::paintLineSupports(QPainter &painter)
{
    QPen supportPen(Qt::magenta, 8); // Set the pen to magenta color and 8 units thick
    painter.setPen(supportPen);



    for (const auto &support : lineSupports) {
        if (support.lineId != -1) { // This is a straight line support
            qDebug() << "Processing line support with lineId:" << support.lineId;

            auto lineEntry = std::find_if(lines.begin(), lines.end(),
                                          [&](const Line &line) { return line.id == support.lineId; });
            if (lineEntry != lines.end()) {


                QPointF startPoint(lineEntry->startX, -lineEntry->startZ);
                QPointF endPoint(lineEntry->endX, -lineEntry->endZ);

                QLineF line(startPoint, endPoint);
                double lineLength = line.length();
                QPointF direction = (endPoint - startPoint) / lineLength;

                // Normalize the perpendicular direction vector manually
                QPointF perpDirection(-direction.y() + direction.x(), direction.x() + direction.y());
                double perpLength = std::sqrt(perpDirection.x() * perpDirection.x() + perpDirection.y() * perpDirection.y());
                perpDirection /= perpLength;
                perpDirection *= 160.0;  // Set the support line length to 160 units



                for (double i = 0; i < lineLength; i += 50.0) {
                    QPointF position = startPoint + i * direction;
                    QLineF supportLine(position - perpDirection, position + perpDirection);
                    painter.drawLine(supportLine);

                }
            } else {
                qDebug() << "Line with ID" << support.lineId << "not found in lines vector.";
            }
        } else if (support.circularLineId != -1) { // This is a circular line support


            auto circularLineEntry = std::find_if(circularLines.begin(), circularLines.end(),
                                                  [&](const CircularLine &circularLine) { return circularLine.id == support.circularLineId; });
            if (circularLineEntry != circularLines.end()) {


                QPointF center(circularLineEntry->centreX, -circularLineEntry->centreZ);
                double radius = circularLineEntry->diameter / 2.0;
                double circumference = 2.0 * M_PI * radius;

                int numberOfSupports = static_cast<int>(circumference / 50.0);

                for (int i = 0; i < numberOfSupports; ++i) {
                    double angle = 2.0 * M_PI * i / numberOfSupports;
                    QPointF supportPosition = center + QPointF(std::cos(angle) * radius, std::sin(angle) * radius);

                    QPointF tangentDirection(-std::sin(angle) + std::cos(angle), std::cos(angle) + std::sin(angle));
                    double tangentLength = std::sqrt(tangentDirection.x() * tangentDirection.x() + tangentDirection.y() * tangentDirection.y());
                    tangentDirection /= tangentLength;
                    tangentDirection *= 160.0;  // Set the support line length to 160 units

                    QLineF supportLine(supportPosition - tangentDirection, supportPosition + tangentDirection);
                    painter.drawLine(supportLine);

                }
            } else {
                qDebug() << "Circular line with ID" << support.circularLineId << "not found in circularLines vector.";
            }
        } else {
            qDebug() << "Support has neither a valid lineId nor a circularLineId.";
        }
    }

    qDebug() << "Finished painting line supports.";
}

void SlabGUI::paintSurfaceSupports(QPainter &painter)
{
    QPen pen(Qt::magenta, 2); // Set the pen to magenta color and 2 units thick
    QBrush brush(Qt::magenta, Qt::DiagCrossPattern); // Set the brush to magenta with diagonal hatching
    painter.setPen(pen);
    painter.setBrush(brush);

    for (const auto &support : surfaceSupports) {
        QRectF supportRect(QPointF(support.x1, -support.z1), QPointF(support.x2, -support.z2));
        painter.drawRect(supportRect);
    }
}

void SlabGUI::paintPointLoads(QPainter &painter)
{
    QPen textPen(Qt::red); // Red color for the text
    painter.setPen(textPen);

    QBrush brush(Qt::NoBrush); // No fill for the circle by default
    painter.setBrush(brush);

    QFont font = painter.font();
    font.setPointSize(150); // Adjust font size to match other labels
    painter.setFont(font);

    double circleRadius = 100.0;
    double crossLength = circleRadius * std::sqrt(2); // Length of the cross arm to touch the circle

    for (const auto &load : pointLoads) {
        QPointF position(load.x, -load.z);

        if (load.F > 0) {
            // Thicker circle when F > 0
            QPen thickCirclePen(Qt::red, 16); // Thicker red circle
            painter.setPen(thickCirclePen);
            painter.drawEllipse(position, circleRadius, circleRadius);

            // Draw the dot if F is positive
            QBrush dotBrush(Qt::red); // Red color for the dot
            painter.setBrush(dotBrush);
            painter.drawEllipse(position, 20, 20); // Dot with radius 20
        } else if (load.F < 0) {
            // Circle for negative F, but thinner than for positive F
            QPen thinCirclePen(Qt::red, 16); // Thinner red circle
            painter.setPen(thinCirclePen);
            painter.drawEllipse(position, circleRadius, circleRadius);

            // Draw the cross if F is negative
            QPen crossPen(Qt::red, 16); // Pen for the cross
            painter.setPen(crossPen);

            // Calculate the endpoints of the cross arms
            QPointF end1(position.x() - crossLength, position.y() - crossLength);
            QPointF end2(position.x() + crossLength, position.y() + crossLength);
            QPointF end3(position.x() - crossLength, position.y() + crossLength);
            QPointF end4(position.x() + crossLength, position.y() - crossLength);

            // Draw the cross
            painter.drawLine(position, end1);
            painter.drawLine(position, end2);
            painter.drawLine(position, end3);
            painter.drawLine(position, end4);
        }

        // Draw the F value label next to the circle with the unit kN
        painter.setPen(textPen); // Set pen for text drawing
        painter.drawText(position.x() + 120, position.y() - 120, QString::number(load.F, 'f', 2) + " kN");
    }
}


void SlabGUI::on_refreshButton_clicked()
{
    // Fetch all data from DB managers
    dataBasePointsManager->iterateOverTable();
    dataBaseLinesManager->iterateOverTable();
    dataBaseCircularLinesManager->iterateOverTable();
    dataBaseSurfacesManager->iterateOverTable();
    dataBaseLineSupportsManager->iterateOverTable(); // Fetch line supports
    dataBaseSurfaceSupportsManager->iterateOverTable(); // Fetch surface supports
    dataBaseSlabPointLoadManager->iterateOverTable(); // Fetch slab point loads
    dataBaseSlabLineLoadsManager->iterateOverTable(); // Fetch slab line loads
    dataBaseSurfaceLoadsManager->iterateOverTable(); // Fetch surface loads

    // Clear existing vectors
    points.clear();
    lines.clear();
    circularLines.clear();
    surfaces.clear();
    lineSupports.clear(); // Clear the lineSupports vector
    surfaceSupports.clear(); // Clear the surfaceSupports vector
    pointLoads.clear(); // Clear the pointLoads vector
    lineLoads.clear(); // Clear the lineLoads vector
    surfaceLoads.clear(); // Clear the surfaceLoads vector

    qDebug() << "Cleared vectors, starting to populate them";

    // Populate points vector
    for (const auto &point : dataBasePointsManager->getPointsMap()) {
        points.push_back({point.second.first, point.second.second, point.first});
    }
    qDebug() << "Points populated, size:" << points.size();

    // Populate lines vector
    for (const auto &lineEntry : dataBaseLinesManager->getLinesMap()) {
        int lineId = lineEntry.first;
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

    // Populate circularLines vector
    for (const auto &circularLineEntry : dataBaseCircularLinesManager->getCircularLinesMap()) {
        int id = circularLineEntry.first;
        int centreX = std::get<0>(circularLineEntry.second);
        int centreZ = std::get<1>(circularLineEntry.second);
        int diameter = std::get<2>(circularLineEntry.second);
        circularLines.push_back({id, centreX, centreZ, diameter});
    }
    qDebug() << "Circular lines populated, size:" << circularLines.size();

    // Populate surfaces vector
    for (const auto &surfaceEntry : dataBaseSurfacesManager->getSurfacesMap()) {
        int id = surfaceEntry.first;
        std::string surfaceType = std::get<0>(surfaceEntry.second);
        int line1Id = std::get<1>(surfaceEntry.second);
        int line2Id = std::get<2>(surfaceEntry.second);
        int line3Id = std::get<3>(surfaceEntry.second);
        int line4Id = std::get<4>(surfaceEntry.second);
        int circularLineId = std::get<5>(surfaceEntry.second);
        int materialId = std::get<6>(surfaceEntry.second);
        int thickness = std::get<7>(surfaceEntry.second);
        bool isOpening = std::get<8>(surfaceEntry.second);

        // Handle potential missing (NULL) values
        if (line1Id == -1) line1Id = 0;
        if (line2Id == -1) line2Id = 0;
        if (line3Id == -1) line3Id = 0;
        if (line4Id == -1) line4Id = 0;
        if (circularLineId == -1) circularLineId = 0;

        surfaces.push_back({id, surfaceType, line1Id, line2Id, line3Id, line4Id, circularLineId, materialId, thickness, isOpening});
    }
    qDebug() << "Surfaces populated, size:" << surfaces.size();

    // Populate lineSupports vector
    for (const auto &supportEntry : dataBaseLineSupportsManager->getLineSupportsMap()) {
        int supportId = supportEntry.first;
        int lineId = std::get<0>(supportEntry.second);
        int circularLineId = std::get<1>(supportEntry.second);

        lineSupports.push_back({supportId, lineId, circularLineId});
    }
    qDebug() << "Line Supports populated, size:" << lineSupports.size();

    // Populate surfaceSupports vector
    for (const auto &supportEntry : dataBaseSurfaceSupportsManager->getSurfaceSupportsMap()) {
        int supportId = supportEntry.first;
        int x1 = std::get<0>(supportEntry.second);
        int z1 = std::get<1>(supportEntry.second);
        int x2 = std::get<2>(supportEntry.second);
        int z2 = std::get<3>(supportEntry.second);

        surfaceSupports.push_back({supportId, x1, z1, x2, z2});
    }
    qDebug() << "Surface Supports populated, size:" << surfaceSupports.size();

    // Populate pointLoads vector
    for (const auto &loadEntry : dataBaseSlabPointLoadManager->getNodalLoadsMap()) {
        int loadId = loadEntry.first;
        double Fz = std::get<0>(loadEntry.second);
        int x = std::get<1>(loadEntry.second);
        int z = std::get<2>(loadEntry.second);

        pointLoads.push_back({loadId, x, z, Fz});
    }
    qDebug() << "Point Loads populated, size:" << pointLoads.size();

    // Populate lineLoads vector
    for (const auto &loadEntry : dataBaseSlabLineLoadsManager->getLineLoadsMap()) {
        int loadId = loadEntry.first;
        int x1 = std::get<0>(loadEntry.second);
        int z1 = std::get<1>(loadEntry.second);
        int x2 = std::get<2>(loadEntry.second);
        int z2 = std::get<3>(loadEntry.second);
        double Fz = std::get<4>(loadEntry.second);

        lineLoads.push_back({x1, z1, x2, z2, Fz, loadId});
    }
    qDebug() << "Line Loads populated, size:" << lineLoads.size();

    // Populate surfaceLoads vector
    for (const auto &loadEntry : dataBaseSurfaceLoadsManager->getSurfaceLoadsMap()) {
        int loadId = loadEntry.first;
        int x1 = std::get<0>(loadEntry.second);
        int z1 = std::get<1>(loadEntry.second);
        int x2 = std::get<2>(loadEntry.second);
        int z2 = std::get<3>(loadEntry.second);
        double F = std::get<4>(loadEntry.second);

        surfaceLoads.push_back({x1, z1, x2, z2, F, loadId});
    }
    qDebug() << "Surface Loads populated, size:" << surfaceLoads.size();

    update();
}







void SlabGUI::on_clearButton_clicked()
{
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
            dataBaseSurfacesManager->dropTable(TableType::SURFACES);
            dataBaseCircularLinesManager->dropTable(TableType::CIRCULAR_LINES);
            // dataBaseMaterialsManager->dropTable(TableType::MATERIALS);



            dataBaseStarter->createPointsTable();
            dataBaseStarter->createLinesTable();
            dataBaseStarter->createSurfacesTable();
            dataBaseStarter->createCircularLinesTable();


            // Refresh the UI to reflect changes

            on_refreshButton_clicked();

            qDebug() << "Data has been cleared.";
        } else {
            qDebug() << "Clear operation canceled.";
        }
    }
}


void SlabGUI::on_addSupportConditionsButton_clicked()
{
    {
        cout << "Add support conditions button clicked" << endl;
        AddSlabSupportsDialog *dialog = new AddSlabSupportsDialog(this);
        dialog->moveToBottomLeft();
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(dialog, &AddSlabSupportsDialog::accepted, this, [this, dialog]() {
            int lineID = dialog->getLineID();
            dataBaseLineSupportsManager->addObjectToDataBase(lineID);

            SlabGUI::on_refreshButton_clicked();
            SlabGUI::on_addSupportConditionsButton_clicked();

        });

        connect(dialog, &AddSlabSupportsDialog::rejected, dialog, &AddSlabSupportsDialog::deleteLater);
        dialog->show();
    }
}


void SlabGUI::on_addSurfaceSupportButton_clicked()
{
    AddSurfaceSupportDialog *dialog = new AddSurfaceSupportDialog(this);
    dialog->moveToBottomLeft();
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &AddSurfaceSupportDialog::accepted, this, [this, dialog]() {
        int x1 = dialog->getX1();
        int z1 = dialog->getZ1();
        int x2 = dialog->getX2();
        int z2 = dialog->getZ2();

        // Fetch the main surface
        auto mainSurfaceTuple = dataBaseSurfacesManager->getMainSurface();

        if (std::get<0>(mainSurfaceTuple) == "rectangle") {
            Surface mainSurface;
            mainSurface.surfaceType = std::get<0>(mainSurfaceTuple);
            mainSurface.line1Id = std::get<1>(mainSurfaceTuple);
            mainSurface.line2Id = std::get<2>(mainSurfaceTuple);
            mainSurface.line3Id = std::get<3>(mainSurfaceTuple);
            mainSurface.line4Id = std::get<4>(mainSurfaceTuple);
            mainSurface.circularLineId = std::get<5>(mainSurfaceTuple);
            mainSurface.materialId = std::get<6>(mainSurfaceTuple);
            mainSurface.thickness = std::get<7>(mainSurfaceTuple);
            mainSurface.isOpening = std::get<8>(mainSurfaceTuple);

            // Check if the support is within the main surface
            if (isRectangleWithin(mainSurface, x1, z1, x2, z2)) {
                // Add the support to the database
                dataBaseSurfaceSupportsManager->addObjectToDataBase(x1, z1, x2, z2);
                SlabGUI::on_refreshButton_clicked();
            } else {
                QMessageBox::warning(this, "Invalid Operation", "The support must be within the area of the main surface.");
            }
        } else {
            QMessageBox::warning(this, "Invalid Operation", "No valid rectangular main surface found.");
        }

        SlabGUI::on_addSurfaceSupportButton_clicked();
    });

    connect(dialog, &AddSurfaceSupportDialog::rejected, dialog, &AddSurfaceSupportDialog::deleteLater);
    dialog->show();
}



void SlabGUI::on_calculateButton_clicked()
{
    dataBaseFreeFEMPreparer->fetchAllData();
    FreeFemExecutor executor(*dataBaseFreeFEMPreparer);
    executor.runAnalysisAndDisplayResults();
}
