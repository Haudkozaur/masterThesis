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
    void on_addLineButton_clicked(); // New slot for adding lines

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

    std::vector<Point> points;
    std::vector<Line> lines;

    void drawPoint();
    void paintPoints(QPainter &painter);
    void paintLines(QPainter &painter);
};

#endif // GUI_H
