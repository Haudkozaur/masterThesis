#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <vector>
#include <utility> // for std::pair
#include "DataBasePointsManager.h"
#include "DataBaseLinesManager.h"
#include "DataBaseSupportsManager.h"

namespace Ui {
class Gui;
}

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gui(DataBasePointsManager *pointsManager,
                 DataBaseLinesManager *linesManager,
                 DataBaseSupportsManager *supportsManager,
                 QWidget *parent = nullptr);
    ~Gui();

private slots:
    void on_addPointButton_clicked();
    void on_addLineButton_clicked();
    void on_addSupportButton_clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override; // Dodaj to

private:
    Ui::Gui *ui;

    DataBasePointsManager *dataBasePointsManager;
    DataBaseLinesManager *dataBaseLinesManager;
    DataBaseSupportsManager *dataBaseSupportsManager;

    int xCoordinate;
    int zCoordinate;
    bool pointSet;

    struct Point {
        int x;
        int z;
        int id;
    };
    struct Line {
        int startX;
        int startZ;
        int endX;
        int endZ;
    };
    struct Boundary {
        int pointId;
        bool ry;
        bool tz;
        bool tx;
    };

    std::vector<Point> points;
    std::vector<Line> lines;
    std::vector<Boundary> boundaries;

    double scaleFactor = 1.0; // Dodaj to

    void paintPoints(QPainter &painter);
    void paintLines(QPainter &painter);
    void paintSupports(QPainter &painter);
};

#endif // GUI_H
