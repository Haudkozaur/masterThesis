#include "gui.h"
#include "ui_gui.h"
#include "DataBasePointsManager.h"
#include "DataBaseLinesManager.h"
#include "addpointdialog.h"
#include "addlinedialog.h"
#include <QPainter>
#include <QFont>

Gui::Gui(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Gui)
    , pointSet(false)
{
    ui->setupUi(this);
    connect(ui->addLineButton, &QPushButton::clicked, this, &Gui::on_addLineButton_clicked);
    connect(ui->addPointButton, &QPushButton::clicked, this, &Gui::on_addPointButton_clicked);
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
            for (const auto& point : dataBasePointsManager.getPointsMap()) {
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

    while (true) {
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
        } else {
            // If dialog is not accepted, break the loop
            break;
        }

        // Clean up the dialog
        dialog->deleteLater();
    }
}

void Gui::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    paintLines(painter);
    paintPoints(painter);
}

void Gui::paintPoints(QPainter &painter)
{
    painter.setBrush(Qt::blue);
    painter.setPen(Qt::black);

    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    for (const auto& point : points) {
        painter.drawEllipse(QPointF(point.x, point.z), 5, 5);
        painter.drawText(QPointF(point.x + 10, point.z - 10), QString::number(point.id));
    }
}

void Gui::paintLines(QPainter &painter)
{
    painter.setPen(Qt::black);

    for (const auto& line : lines) {
        painter.drawLine(QPointF(line.startX, line.startZ), QPointF(line.endX, line.endZ));
    }
}

void Gui::drawPoint()
{
    pointSet = true;
    update();
}
