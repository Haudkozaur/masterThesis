#ifndef SLABGUI_H
#define SLABGUI_H

#include <QComboBox>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QCheckBox>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QTransform>
#include "../DataBaseManagers/DataBaseManagers.h"
#include <vector>

using namespace DataBaseManagers;

namespace Ui {
class SlabGUI;
}

class SlabGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit SlabGUI(DataBasePointsManager *pointsManager,
                     DataBaseLinesManager *linesManager,
                     DataBaseMaterialsManager *materialsManager,
                     DataBaseSurfacesManager *surfacesManager,
                     DataBaseCircularLinesManager *circularLinesManager,
                     DataBaseStarter *starter,
                     QWidget *parent = nullptr);
    ~SlabGUI();


protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;



private slots:
    void on_editObjectButton_clicked();

    void on_modelPhaseComboBox_currentIndexChanged(int index);

    void on_addSurfaceButton_clicked();

    void on_refreshButton_clicked();

private:
    Ui::SlabGUI *ui;

    void loadLayoutFromFile(const QString &fileName);
    void clearLayout(QLayout *layout);

    // Declare the missing member variables
    double scaleFactor;
    bool isDragging;
    QPointF lastMousePosition;
    QPointF translationOffset;
    int xCoordinate;
    int zCoordinate;

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
        int id;
        int crossSectionId;
        double length;
    };
    struct CircularLine
    {
        int id;
        int centreX;
        int centreZ;
        int diameter;
    };
    struct Surface
    {
        int id;
        std::string surfaceType;
        int line1Id;
        int line2Id;
        int line3Id;
        int line4Id;
        int circularLineId;
        int materialId;
        int thickness;
        bool isOpening;

    };

    vector<Point> points;
    vector<Line> lines;
    vector<CircularLine> circularLines;
    vector<Surface> surfaces;

    QString surfaceLayoutType;
    void drawAxes(QPainter &painter);
    void drawGrid(QPainter &painter, qreal leftX, qreal rightX, qreal topZ, qreal bottomZ, qreal step, qreal centerX, qreal centerZ);
    void paintSurfaces(QPainter &painter);
    void paintCircularLines(QPainter &painter);
    void paintPoints(QPainter &painter);
    void paintLines(QPainter &painter);
    void paintPointsLabels(QPainter &painter);
    void paintLinesLabels(QPainter &painter);
    void paintCircularLinesLabels(QPainter &painter);


    DataBasePointsManager *dataBasePointsManager;
    DataBaseLinesManager *dataBaseLinesManager;
    DataBaseMaterialsManager *dataBaseMaterialsManager;
    DataBaseSurfacesManager *dataBaseSurfacesManager;
    DataBaseCircularLinesManager *dataBaseCircularLinesManager;
    DataBaseStarter *dataBaseStarter;


    // Methods to check if opening is inside main surface
    bool isRectangleWithin(const Surface &mainSurface, int x1, int z1, int x2, int z2);
    bool isTriangleWithin(const Surface &mainSurface, int triX1, int triZ1, int triX2, int triZ2, int triX3, int triZ3);
    bool isCircleWithin(const Surface &mainSurface, int centreX1, int centreZ1, int diameter);


    // Utility methods
    QPointF getCoordinatesFromLine(int lineId);
    int getCircularLineRadius(int circularLineId);
    bool isPointInTriangle(const QPointF &pt, const QPointF &v1, const QPointF &v2, const QPointF &v3);
    float sign(const QPointF &p1, const QPointF &p2, const QPointF &p3);
};

#endif // SLABGUI_H
