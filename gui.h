#ifndef GUI_H
#define GUI_H

#include <QComboBox>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include "DataBaseLinesManager.h"
#include "DataBasePointsManager.h"
#include "DataBaseStarter.h"
#include "DataBaseSupportsManager.h"
#include "DataBaseMaterialsManager.h"
#include "DataBaseCrossSectionsManager.h"
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
                 DataBaseMaterialsManager *materialsManager,
                 DataBaseCrossSectionsManager *crossSectionsManager,
                 DataBaseStarter *starter,
                 QWidget *parent = nullptr);
    ~Gui();

protected:

    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override; // Add this line

private slots:
    //slots for buttons from basic gui layout
    void on_addPointButton_clicked();
    void on_addLineButton_clicked();
    void on_addSupportButton_clicked();
    void on_deleteObjectButton_clicked();
    void on_refreshButton_clicked();
    void on_clearButton_clicked();
    void on_editObjectButton_clicked();

    //cover changing layouts options
    void onComboBoxIndexChanged(int index);
public slots:
    //slots for buttons from static scheme
    //TODO: refactor it to not using functions from basic gui layout
    void on_layoutAddPointButton_clicked();
    void on_layoutAddLineButton_clicked();
    void on_layoutAddSupportButton_clicked();
    void on_addMaterialButton_clicked();
    void on_addCrossSectionButton_clicked();
    void on_openCrossSectionManagerButton();
    void on_setPropertiesButton_clicked();
private:
    Ui::Gui *ui;

    //to update leftverticallayout depending on the selected option
    void loadLayoutFromFile(const QString &fileName);
    void clearLayout(QLayout *layout);
    void loadStaticSchemeLayout();
    void loadPropertiesLayout();

    // Pointers to DB managers used in the GUI
    DataBasePointsManager *dataBasePointsManager;
    DataBaseLinesManager *dataBaseLinesManager;
    DataBaseSupportsManager *dataBaseSupportsManager;
    DataBaseMaterialsManager *dataBaseMaterialsManager;
    DataBaseCrossSectionsManager *dataBaseCrossSectionsManager;
    DataBaseStarter *dataBaseStarter;

    //using in drawing coordinates system
    int xCoordinate;
    int zCoordinate;


    // To save edit and delete dialog state after accept
    QString lastSelectedType;
    QString deleteLastSelectedType;

    //manually added pointers to buttons in .ui files to update leftverticallayout
    QPushButton *layoutAddPointButton;
    QPushButton *layoutAddLineButton;
    QPushButton *layoutAddSupportButton;
    QPushButton *addMaterialButton;
    QPushButton *addCrossSectionButton;
    QPushButton *setPropertiesButton;
    QPushButton *openCrossSectionManagerButton;


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
        int id; // New field id
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
