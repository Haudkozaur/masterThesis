#include "gui.h"
#include <QDebug>
#include <QFont>
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include "DataBaseLinesManager.h"
#include "DataBasePointsManager.h"
#include "DataBaseSupportsManager.h"
#include "addboundariesdialog.h"
#include "addlinedialog.h"
#include "addpointdialog.h"
#include "ui_gui.h"
#include "deleteobjectdialog.h"
#include <cmath>

// gui constructor
Gui::Gui(DataBasePointsManager *pointsManager,
         DataBaseLinesManager *linesManager,
         DataBaseSupportsManager *supportsManager,
         QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Gui)
    , xCoordinate(0)
    , zCoordinate(0)
    , pointSet(false)
    , dataBasePointsManager(pointsManager)
    , dataBaseLinesManager(linesManager)
    , dataBaseSupportsManager(supportsManager)
    , scaleFactor(1.0)
    , isDragging(false)
{
    ui->setupUi(this);

    // Calculate initial translation offset to center the origin
    translationOffset = QPoint(width() / 2, height() / 2);

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

        update();
        // Optionally, you might want to call another method here
        Gui::on_addPointButton_clicked(); // Be cautious with recursion
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
            dataBaseLinesManager->addObjectToDataBase(startId, endId);
            lines.push_back({startPoint.first, startPoint.second, endPoint.first, endPoint.second});
        }

        update();
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

        update();
        Gui::on_addSupportButton_clicked(); // Be cautious with recursion
    });

    // Connect the rejected signal to delete the dialog
    connect(dialog, &AddBoundariesDialog::rejected, dialog, &AddBoundariesDialog::deleteLater);

    // Show the dialog non-modally
    dialog->show();
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
}

void Gui::paintPoints(QPainter &painter)
{
    painter.setBrush(Qt::blue);
    painter.setPen(Qt::black);

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

        qDebug() << "Drawing support for pointId:" << pointId << " x:" << x << " z:" << z
                 << " ry:" << boundary.ry << " tx:" << boundary.tx << " tz:" << boundary.tz;

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
            painter.drawLine(QPointF(x, z), QPointF(x, z - eccentricity));
            painter.drawEllipse(QPointF(x, z - eccentricity), 5, 5);
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

void Gui::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging) {
        QPointF delta = event->localPos() - lastMousePosition;
        lastMousePosition = event->localPos();
        translationOffset += delta;
        update();
    }
}

void Gui::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        isDragging = false;
    }
}


void Gui::on_refreshButton_clicked()
{
    // Update the data from the database managers
    dataBasePointsManager->iterateOverTable();
    dataBaseLinesManager->iterateOverTable();
    dataBaseSupportsManager->iterateOverTable();

    // Clear current points, lines, and boundaries
    points.clear();
    lines.clear();
    boundaries.clear();

    // Update points from the database manager
    for (const auto &point : dataBasePointsManager->getPointsMap()) {
        points.push_back({point.second.first, point.second.second, point.first});
    }

    // Update lines from the database manager
    for (const auto &lineEntry : dataBaseLinesManager->getLinesMap()) {
        int startId = std::get<0>(lineEntry.second);
        int endId = std::get<1>(lineEntry.second);

        auto pointsMap = dataBasePointsManager->getPointsMap();

        if (pointsMap.find(startId) != pointsMap.end() && pointsMap.find(endId) != pointsMap.end()) {
            auto startPoint = pointsMap[startId];
            auto endPoint = pointsMap[endId];
            lines.push_back({startPoint.first, startPoint.second, endPoint.first, endPoint.second});
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

    // Update the widget to trigger a repaint
    update();
}
void Gui::on_deleteObjectButton_clicked()
{
    // Tworzymy instancję dialogu
    DeleteObjectDialog *dialog = new DeleteObjectDialog(this);

    // Przemieszczamy dialog do lewego dolnego rogu głównego okna
    dialog->moveToBottomLeft();

    // Ustawiamy atrybut, aby dialog został usunięty po jego zamknięciu
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Po zamknięciu dialogu, przetwarzamy wybór użytkownika
    connect(dialog, &DeleteObjectDialog::accepted, this, [this, dialog]() {
        QString selectedType = dialog->getSelectedObjectType();

        // Obsługuje usuwanie w zależności od wybranego typu
        if (selectedType == "Points") {
            // Logika usuwania punktów
        } else if (selectedType == "Lines") {
            // Logika usuwania linii
        } else if (selectedType == "Supports") {
            // Logika usuwania podpór
        }

        // Odświeżenie UI
        on_refreshButton_clicked();
    });

    // Po odrzuceniu dialogu, usuwamy go
    connect(dialog, &DeleteObjectDialog::rejected, dialog, &DeleteObjectDialog::deleteLater);

    // Pokazujemy dialog nienachalnie
    dialog->show();
}

