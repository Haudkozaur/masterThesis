#ifndef GUI_H
#define GUI_H

#include <QMainWindow>

namespace Ui {
class Gui;
}

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gui(QWidget *parent = nullptr);
    ~Gui();

private:
    Ui::Gui *ui;
};

#endif // GUI_H
