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

    QString surfaceLayoutType;
    void drawAxes(QPainter &painter);
    void drawGrid(QPainter &painter, qreal leftX, qreal rightX, qreal topZ, qreal bottomZ, qreal step, qreal centerX, qreal centerZ);

    DataBasePointsManager *dataBasePointsManager;
    DataBaseLinesManager *dataBaseLinesManager;
    DataBaseMaterialsManager *dataBaseMaterialsManager;
    DataBaseSurfacesManager *dataBaseSurfacesManager;
    DataBaseStarter *dataBaseStarter;

};

#endif // SLABGUI_H
