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
#include <QWheelEvent>

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
{
    ui->setupUi(this);
    /*    connect(ui->addLineButton, &QPushButton::clicked, this, &Gui::on_addLineButton_clicked);
    connect(ui->addPointButton, &QPushButton::clicked, this, &Gui::on_addPointButton_clicked);
    connect(ui->addSupportButton,
            &QPushButton::clicked,
            this,
            &Gui::on_addSupportButton_clicked);*/ // Connect button to slot
}

Gui::~Gui()
{
    delete ui;
}

void Gui::on_addPointButton_clicked()
{
    // new dialog
    AddPointDialog *dialog = new AddPointDialog(this);
    dialog->setWindowModality(Qt::NonModal);
    if (dialog->exec() == QDialog::Accepted) {
        xCoordinate = dialog->getXCoordinate();
        zCoordinate = dialog->getZCoordinate();

        qDebug() << "Adding Point to Database:" << xCoordinate << zCoordinate;

        dataBasePointsManager->addObjectToDataBase(xCoordinate, zCoordinate);

        // Select all points from database
        dataBasePointsManager->iterateOverTable();
        points.clear();

        // update points vector
        for (const auto &point : dataBasePointsManager->getPointsMap()) {
            points.push_back({point.second.first, point.second.second, point.first});
        }

        // redraw all points
        update();
        dialog->close();
        Gui::on_addPointButton_clicked();
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

        // redraw all lines
        update();
        dialog->close();
        Gui::on_addLineButton_clicked();
    } else if (dialog->result() == QDialog::Rejected) {
        dialog->close();
    }
}

void Gui::on_addSupportButton_clicked()
{
    // new dialog
    AddBoundariesDialog *dialog = new AddBoundariesDialog(this);

    if (dialog->exec() == QDialog::Accepted) {
        int pointId = dialog->getPointId();
        bool ry = dialog->getRy();
        bool tx = dialog->getTx();
        bool tz = dialog->getTz();

        qDebug() << "Support added for:" << pointId << " ry:" << ry << " tx:" << tx << " tz:" << tz;

        dataBaseSupportsManager->addObjectToDataBase(pointId, ry, tz, tx);

        // select all supports from database
        dataBaseSupportsManager->iterateOverTable();
        boundaries.clear();

        // update boundaries vector
        auto supportsMap = dataBaseSupportsManager->getSupportsMap();
        for (const auto &support : supportsMap) {
            bool ry = std::get<1>(support.second);
            bool tx = std::get<2>(support.second);
            bool tz = std::get<3>(support.second);

            qDebug() << "Support added for pointId:" << support.first << " ry:" << ry
                     << " tx:" << tx << " tz:" << tz;

            boundaries.push_back({support.first, ry, tx, tz});
        }

        // redraw all supports
        update();
        dialog->close();
        Gui::on_addSupportButton_clicked();

    } else if (dialog->result() == QDialog::Rejected) {
        dialog->close();
    }
}

void Gui::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // translation of coordinate system to start in left right corner
    QTransform transform;
    transform.translate(0, height());
    transform.scale(1, -1);
    transform.scale(scaleFactor, scaleFactor); // Dodaj to
    painter.setTransform(transform);

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

        // Zapamiętaj stan painter
        painter.save();

        // Cofnij transformację i ustaw tekst
        QTransform transformText;
        transformText.translate(point.x + 10, point.z - 10);
        transformText.scale(1, -1); // Odwróć tekst, aby był w standardowym układzie współrzędnych
        painter.setTransform(transformText, true);

        painter.drawText(0, 0, QString::number(point.id));

        // Przywróć stan painter
        painter.restore();
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
    painter.setPen(QPen(Qt::green, 1));

    for (const auto &boundary : boundaries) {
        int pointId = boundary.pointId;

        if (dataBasePointsManager->getPointsMap().find(pointId)
            == dataBasePointsManager->getPointsMap().end()) {
            qDebug() << "Point ID not found in pointsMap: " << pointId;
            continue; // If point with given ID does not exist in map, skip to next support
        }

        auto point = dataBasePointsManager->getPointsMap()[pointId];
        int x = point.first;
        int z = point.second;

        qDebug() << "Drawing support for pointId:" << pointId << " x:" << x << " z:" << z
                 << " ry:" << boundary.ry << " tx:" << boundary.tx << " tz:" << boundary.tz;

        int eccentricity = 20;

        painter.setBrush(Qt::green);

        if (boundary.ry) {
            painter.drawLine(QPointF(x, z), QPointF(x + eccentricity, z + eccentricity));
            painter.drawLine(QPointF(x, z), QPointF(x - eccentricity, z + eccentricity));
            painter.drawLine(QPointF(x + eccentricity, z + eccentricity),
                             QPointF(x - eccentricity, z + eccentricity));
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
            painter.drawEllipse(QPointF(x, z), eccentricity / 5, eccentricity / 5);
        }
    }
}
void Gui::wheelEvent(QWheelEvent *event)
{
    // Zoom in or out
    if (event->angleDelta().y() > 0) {
        scaleFactor *= 1.1;
    } else {
        scaleFactor /= 1.1;
    }

    // Redraw the widget
    update();
}
