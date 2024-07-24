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
#include <cmath>

// Konstruktor Gui z wskaźnikami do menedżerów baz danych
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
    AddPointDialog *dialog = new AddPointDialog(this);
    dialog->setWindowModality(Qt::NonModal);
    if (dialog->exec() == QDialog::Accepted) {
        xCoordinate = dialog->getXCoordinate();
        zCoordinate = dialog->getZCoordinate();

        qDebug() << "Adding Point to Database:" << xCoordinate << zCoordinate;

        dataBasePointsManager->addObjectToDataBase(xCoordinate, zCoordinate);

        dataBasePointsManager->iterateOverTable();
        points.clear();

        for (const auto &point : dataBasePointsManager->getPointsMap()) {
            points.push_back({point.second.first, point.second.second, point.first});
        }

        update();
        dialog->close();
    } else if (dialog->result() == QDialog::Rejected) {
        dialog->close();
    }
}

void Gui::on_addLineButton_clicked()
{
    dataBasePointsManager->iterateOverTable();

    AddLineDialog *dialog = new AddLineDialog(this);
    if (dialog->exec() == QDialog::Accepted) {
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
        dialog->close();
    } else if (dialog->result() == QDialog::Rejected) {
        dialog->close();
    }
}

void Gui::on_addSupportButton_clicked()
{
    AddBoundariesDialog *dialog = new AddBoundariesDialog(this);

    if (dialog->exec() == QDialog::Accepted) {
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
        dialog->close();
    } else if (dialog->result() == QDialog::Rejected) {
        dialog->close();
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
    painter.setTransform(QTransform().translate(translationOffset.x(), translationOffset.y())
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
    painter.setPen(QPen(Qt::green, 1));

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

        qreal eccentricity = 20; // Adjust as needed

        painter.setBrush(Qt::green);

        // Rysowanie ry (rysowanie w oparciu o rzeczywiste współrzędne)
        if (boundary.ry) {
            painter.drawLine(QPointF(x, z), QPointF(x + eccentricity, z + eccentricity));
            painter.drawLine(QPointF(x, z), QPointF(x - eccentricity, z + eccentricity));
            painter.drawLine(QPointF(x + eccentricity, z + eccentricity),
                             QPointF(x - eccentricity, z + eccentricity));
        }

        // Rysowanie tz (rysowanie w oparciu o rzeczywiste współrzędne)
        if (boundary.tz) {
            painter.drawLine(QPointF(x, z), QPointF(x, z - eccentricity));
            painter.drawEllipse(QPointF(x, z - eccentricity), 5, 5);
        }

        // Rysowanie tx (rysowanie w oparciu o rzeczywiste współrzędne)
        if (boundary.tx) {
            painter.drawLine(QPointF(x, z), QPointF(x - eccentricity, z));
            painter.drawEllipse(QPointF(x - eccentricity, z), 5, 5);
        }

        // Jeśli żaden z flagi nie jest ustawiony, rysuj tylko punkt
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
        painter.drawText(labelPos, QString::number(z, 'f', 0));
        // Print current z value
        qDebug() << "z: " << z;
    }

    // Draw the grid
    drawGrid(painter, leftX, rightX, topZ, bottomZ, step, centerX, centerZ);

    // Restore the original transformation
    painter.setTransform(originalTransform);
}

void Gui::drawGrid(QPainter &painter, qreal leftX, qreal rightX, qreal topZ, qreal bottomZ, qreal step, qreal centerX, qreal centerZ)
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
        scaleFactor *= 1.1; // Powiększenie
    } else {
        scaleFactor /= 1.1; // Oddalenie
    }
    update();
}

void Gui::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->modifiers() & Qt::ControlModifier) {
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
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
    }
}
