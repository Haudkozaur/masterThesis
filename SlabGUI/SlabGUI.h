#ifndef SLABGUI_H
#define SLABGUI_H

#include <QMainWindow>

namespace Ui {
class SlabGUI;
}

class SlabGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit SlabGUI(QWidget *parent = nullptr);
    ~SlabGUI();

private:
    Ui::SlabGUI *ui;
};

#endif // SLABGUI_H
