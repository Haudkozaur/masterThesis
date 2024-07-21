#include "gui.h"
#include <QDebug>
#include <QFont>
#include <QPainter>
#include "DataBaseLinesManager.h"
#include "DataBasePointsManager.h"
#include "DataBaseSupportsManager.h"
#include "addboundariesdialog.h"
#include "addlinedialog.h"
#include "addpointdialog.h"
#include "ui_gui.h"

Gui::Gui(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Gui)
    , pointSet(false)
{
    ui->setupUi(this);
    connect(ui->addLineButton, &QPushButton::clicked, this, &Gui::on_addLineButton_clicked);
    connect(ui->addPointButton, &QPushButton::clicked, this, &Gui::on_addPointButton_clicked);
    connect(ui->addSupportButton,
            &QPushButton::clicked,
            this,
            &Gui::on_addSupportButton_clicked); // Connect button to slot
}

Gui::~Gui()
{
    delete ui;
}

void Gui::on_addPointButton_clicked()
{
    DataBasePointsManager dataBasePointsManager("mesosoic_test.db");

    while (true) {
        AddPointDialog *dialog = new AddPointDialog(this);
        if (dialog->exec() == QDialog::Accepted) {
            xCoordinate = dialog->getXCoordinate();
            zCoordinate = dialog->getZCoordinate();
            dataBasePointsManager.addObjectToDataBase(xCoordinate, zCoordinate);

            // Fetch all points from the database
            dataBasePointsManager.iterateOverTable();
            points.clear();

            // Update points vector with all points from the database
            for (const auto &point : dataBasePointsManager.getPointsMap()) {
                points.push_back({point.second.first, point.second.second, point.first});
            }

            // Trigger a repaint to draw all points
            update();
        } else {
            // If dialog is not accepted, break the loop
            break;
        }

        // Clean up the dialog
        dialog->deleteLater();
    }
}

void Gui::on_addLineButton_clicked()
{
    DataBasePointsManager dataBasePointsManager("mesosoic_test.db");
    DataBaseLinesManager dataBaseLinesManager("mesosoic_test.db");

    dataBasePointsManager.iterateOverTable();

    AddLineDialog *dialog = new AddLineDialog(this);
    if (dialog->exec() == QDialog::Accepted) {
        int startId = dialog->getFirstPointId();
        int endId = dialog->getSecondPointId();

        auto pointsMap = dataBasePointsManager.getPointsMap();

        if (pointsMap.find(startId) != pointsMap.end() && pointsMap.find(endId) != pointsMap.end()) {
            auto startPoint = pointsMap[startId];
            auto endPoint = pointsMap[endId];
            dataBaseLinesManager.addObjectToDataBase(startId, endId);
            lines.push_back({startPoint.first, startPoint.second, endPoint.first, endPoint.second});
        }

        // Trigger a repaint to draw all lines
        update();
    }

    // Clean up the dialog
    dialog->deleteLater();
}

void Gui::on_addSupportButton_clicked()
{
    AddBoundariesDialog *dialog = new AddBoundariesDialog(this);
    if (dialog->exec() == QDialog::Accepted) {
        int pointId = dialog->getPointId();
        bool ry = dialog->getRy();
        bool tx = dialog->getTx();
        bool tz = dialog->getTz();
        //print for debugging
        qDebug() << "podpora dodana dla:" << pointId << " ry:" << ry << " tx:" << tx << " tz:" << tz;
        DataBaseSupportsManager dataBaseSupportsManager("mesosoic_test.db");
        dataBaseSupportsManager.addObjectToDataBase(pointId, ry, tz, tx);

        // Fetch all supports from the database
        dataBaseSupportsManager.iterateOverTable();
        boundaries.clear();

        // Update boundaries vector with all supports from the database
        auto supportsMap = dataBaseSupportsManager.getSupportsMap();
        for (const auto &support : supportsMap) {
            // Assuming support.second is a tuple with elements in order: ry, tx, tz
            bool ry = std::get<1>(support.second);
            bool tx = std::get<2>(support.second);
            bool tz = std::get<3>(support.second);

            qDebug() << "Support added for pointId:" << support.first << " ry:" << ry
                     << " tx:" << tx << " tz:" << tz;

            boundaries.push_back({support.first, ry, tx, tz});
        }

        // Trigger a repaint to draw all supports
        update();
    }

    // Clean up the dialog
    dialog->deleteLater();
}

void Gui::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

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
        painter.drawEllipse(QPointF(point.x, point.z), 5, 5);
        painter.drawText(QPointF(point.x + 10, point.z - 10), QString::number(point.id));
    }
}

void Gui::paintLines(QPainter &painter)
{
    painter.setPen(Qt::black);

    for (const auto &line : lines) {
        painter.drawLine(QPointF(line.startX, line.startZ), QPointF(line.endX, line.endZ));
    }
}

void Gui::paintSupports(QPainter &painter)
{
    // Ustaw grubość pióra
    painter.setPen(QPen(Qt::green, 1));

    // Pobierz punkty z bazy danych
    DataBasePointsManager dataBasePointsManager("mesosoic_test.db");
    dataBasePointsManager.iterateOverTable();
    auto pointsMap = dataBasePointsManager.getPointsMap();

    for (const auto &boundary : boundaries) {
        int pointId = boundary.pointId;

        if (pointsMap.find(pointId) == pointsMap.end()) {
            qDebug() << "Point ID not found in pointsMap: " << pointId;
            continue; // Jeśli punkt o podanym ID nie istnieje w mapie, przejdź do następnego wsparcia
        }

        auto point = pointsMap[pointId];
        int x = point.first;
        int z = point.second;

        qDebug() << "Drawing support for pointId:" << pointId << " x:" << x << " z:" << z
                 << " ry:" << boundary.ry << " tx:" << boundary.tx << " tz:" << boundary.tz;

        int eccentricity = 20;

        painter.setBrush(Qt::green);

        if (boundary.ry) {
            painter.drawLine(QPointF(x, z), QPointF(x + eccentricity, z + eccentricity));
            painter.drawLine(QPointF(x, z), QPointF(x - eccentricity, z + eccentricity));
            painter.drawLine(QPointF(x + eccentricity, z + eccentricity), QPointF(x - eccentricity, z + eccentricity));
        }

        if (boundary.tz) {
            painter.drawLine(QPointF(x, z), QPointF(x, z - eccentricity));
            painter.drawEllipse(QPointF(x, z - eccentricity), 5, 5);
        }

        if (boundary.tx) {
            painter.drawLine(QPointF(x, z), QPointF(x - eccentricity, z));
            painter.drawEllipse(QPointF(x - eccentricity, z), 5, 5);
        }

        if (!boundary.ry && !boundary.tx && !boundary.tz) {
            painter.drawEllipse(QPointF(x, z), eccentricity/5, eccentricity/5);
        }
    }
}


