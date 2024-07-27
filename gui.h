#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include "DataBaseLinesManager.h"
#include "DataBasePointsManager.h"
#include "DataBaseStarter.h"
#include "DataBaseSupportsManager.h"
#include <utility> // for std::pair
#include <vector>

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
                 DataBaseStarter *starter,
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
    void on_clearButton_clicked();
    void on_editObjectButton_clicked();

private:
    Ui::Gui *ui;

    // pointers to db managers used in gui
    DataBasePointsManager *dataBasePointsManager;
    DataBaseLinesManager *dataBaseLinesManager;
    DataBaseSupportsManager *dataBaseSupportsManager;
    DataBaseStarter *dataBaseStarter;

    int xCoordinate;
    int zCoordinate;
    bool pointSet;

    // to save edit and delete dialog state after accept
    QString lastSelectedType;
    QString deleteLastSelectedType;

    struct Point
    {
        int x;
        int z;
        int id;
    };
    struct Line
    {
        int startX;
        int startZ;
        int endX;
        int endZ;
        int id; // Nowe pole id
    };

    struct Boundary
    {
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
    void drawGrid(QPainter &painter,
                  qreal leftX,
                  qreal rightX,
                  qreal topZ,
                  qreal bottomZ,
                  qreal step,
                  qreal centerX,
                  qreal centerZ);
};

#endif // GUI_H
