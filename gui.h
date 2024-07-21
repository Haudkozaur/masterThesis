// gui.h
#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <vector>
#include <utility> // for std::pair

namespace Ui {
class Gui;
}

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gui(QWidget *parent = nullptr);
    ~Gui();

private slots:
    void on_addPointButton_clicked();
    void on_addLineButton_clicked(); // Slot for adding lines
    void on_addSupportButton_clicked(); // Slot for adding supports

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Gui *ui;

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
    std::vector<Boundary> boundaries; // Consider renaming to supports for clarity

    void drawPoint();
    void paintPoints(QPainter &painter);
    void paintLines(QPainter &painter);
    void paintSupports(QPainter &painter); // Add this line
};

#endif // GUI_H
