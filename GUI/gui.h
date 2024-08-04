#ifndef GUI_H
#define GUI_H

#include <QComboBox>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "../DataBaseManagers/DataBaseManagers.h"
#include <vector>

using namespace DataBaseManagers;
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
                 DataBaseNodalLoadsManager *nodalLoadsManager,
                 DataBaseLineLoadsManager *lineLoadsManager,
                 CrossSectionsAssistant *crossSectionsAssistant,
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

    void on_addPointAppliedForceButton_clicked();
    void on_addLineLoadButton_clicked();
    void on_openLoadsManagerButton_clicked();

private:
    std::map<int, std::string> materialsMap;
    Ui::Gui *ui;

    //to update leftverticallayout depending on the selected option
    void loadLayoutFromFile(const QString &fileName);
    void clearLayout(QLayout *layout);
    void loadStaticSchemeLayout();
    void loadPropertiesLayout();
    void loadLoadsLayout();
    void loadMeshLayout();
    void loadResultsLayout();

    // Pointers to DB managers used in the GUI
    DataBasePointsManager *dataBasePointsManager;
    DataBaseLinesManager *dataBaseLinesManager;
    DataBaseSupportsManager *dataBaseSupportsManager;
    DataBaseMaterialsManager *dataBaseMaterialsManager;
    DataBaseCrossSectionsManager *dataBaseCrossSectionsManager;
    DataBaseStarter *dataBaseStarter;
    DataBaseNodalLoadsManager *dataBaseNodalLoadsManager;
    DataBaseLineLoadsManager *dataBaseLineLoadsManager;
    CrossSectionsAssistant *crossSectionsAssistant;

    //using in drawing coordinates system
    int xCoordinate;
    int zCoordinate;

    // To save edit and delete dialog state after accept
    QString lastSelectedType;
    QString deleteLastSelectedType;
    QString csLastSelectedType;
    QString setPropLastSelectedType;

    //manually added pointers to buttons in .ui files to update leftverticallayout
    QPushButton *layoutAddPointButton;
    QPushButton *layoutAddLineButton;
    QPushButton *layoutAddSupportButton;

    QPushButton *addMaterialButton;
    QPushButton *addCrossSectionButton;
    QPushButton *setPropertiesButton;
    QPushButton *openCrossSectionManagerButton;

    QPushButton *addPointAppliedForceButton;
    QPushButton *addLineLoadButton;
    QPushButton *openLoadsManagerButton;

    QComboBox modelPhaseComboBox;
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
        int crossSectionId;
    };

    struct Boundary
    {
        int pointId;
        bool ry;
        bool tz;
        bool tx;
    };

    struct NodalLoad
    {
        int pointId;
        qreal My;
        qreal Fz;
        qreal Fx;
    };

    struct LineLoad
    {
        int lineId;
        qreal Fx;
        qreal Fz;
    };

    std::vector<Point> points;
    std::vector<Line> lines;
    std::vector<Boundary> boundaries;
    std::vector<NodalLoad> nodalLoads;
    std::vector<LineLoad> lineLoads;

    qreal scaleFactor = 20;

    QPointF translationOffset;
    QPointF lastMousePosition;
    bool isDragging;

    void paintPoints(QPainter &painter);
    void paintLines(QPainter &painter);
    void paintSupports(QPainter &painter);
    void drawNodalLoads(QPainter &painter);
    void drawLineLoads(QPainter &painter);
    void drawArrowHead(QPainter &painter, const QPointF &start, const QPointF &end);
    void paintAssignedCrossSections(QPainter &painter);
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
