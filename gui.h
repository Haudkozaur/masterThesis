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

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void on_addPointButton_clicked();
    void on_addLineButton_clicked();
    void on_addSupportButton_clicked();

    void on_deleteObjectButton_clicked();

    void on_refreshButton_clicked();
    void on_editObjectButton_clicked();
    void on_clearButton_clicked();

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
        int id; // Nowe pole id
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

    qreal scaleFactor = 1.0;

    QPointF translationOffset;
    QPointF lastMousePosition;
    bool isDragging;

    void paintPoints(QPainter &painter);
    void paintLines(QPainter &painter);
    void paintSupports(QPainter &painter);
    void drawAxes(QPainter &painter);
    void drawGrid(QPainter &painter, qreal leftX, qreal rightX, qreal topZ, qreal bottomZ, qreal step, qreal centerX, qreal centerZ);
};

#endif // GUI_H
