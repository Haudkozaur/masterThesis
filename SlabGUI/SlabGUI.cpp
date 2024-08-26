#include "SlabGUI.h"
#include "ui_SlabGUI.h"
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
#include "AddSurfaceDialog.h"

SlabGUI::SlabGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SlabGUI)
    , xCoordinate(0)
    , zCoordinate(0)
    , scaleFactor(0.1)
    , isDragging(false)
{
    ui->setupUi(this);
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
void SlabGUI::on_editObjectButton_clicked()
{

}


void SlabGUI::on_modelPhaseComboBox_currentIndexChanged(int index)
{

}


void SlabGUI::on_addSurfaceButton_clicked()
{
    cout << "Surface button clicked" << endl;
    AddSurfaceDialog *dialog = new AddSurfaceDialog(this);
    dialog->moveToBottomLeft();
    dialog->initializeWithType(surfaceLayoutType);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &AddSurfaceDialog::accepted, this, [this, dialog]() {
        QString selectedType = dialog->getSelectedObjectType();
        if (selectedType == "Rectangle") {
            int x1 = dialog->getX1();
            int z1 = dialog->getZ1();
            int x2 = dialog->getX2();
            int z2 = dialog->getZ2();
            // print coordinates
            cout << "x1: " << x1 << " z1: " << z1 << " x2: " << x2 << " z2: " << z2 << endl;

        } else if (selectedType == "Circle"){
            int diameter = dialog->getDiameter();
            int centreX1 = dialog->getCentreX1();
            int centreZ1 = dialog->getCentreZ1();
            // print coordinates
            cout << "diameter: " << diameter << " centreX1: " << centreX1 << " centreZ1: " << centreZ1 << endl;

        } else if (selectedType == "Triangle") {
            int triX1 = dialog->getTriX1();
            int triZ1 = dialog->getTriZ1();
            int triX2 = dialog->getTriX2();
            int triZ2 = dialog->getTriZ2();
            int triX3 = dialog->getTriX3();
            int triZ3 = dialog->getTriZ3();
            // print coordinates
            cout << "triX1: " << triX1 << " triZ1: " << triZ1 << " triX2: " << triX2 << " triZ2: " << triZ2 << " triX3: " << triX3 << " triZ3: " << triZ3 << endl;

        }


        // dataBaseMaterialsManager->addObjectToDataBase(materialName,
        //                                               youngModulus * std::pow(10, 9),
        //                                               poissonCoefficient,
        //                                               density);

        // dataBaseMaterialsManager->iterateOverTable();

        SlabGUI::on_addSurfaceButton_clicked();
    });

    connect(dialog, &AddSurfaceDialog::rejected, dialog, &AddSurfaceDialog::deleteLater);
    dialog->show();

}

