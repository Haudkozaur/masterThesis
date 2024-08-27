#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDialog>
#include "ui_StartWindow.h"

class StartWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);

    enum Module {
        None,
        Frame2D,
        Plate
    };

    Module selectedModule() const { return m_selectedModule; }

private slots:
    void onFrame2DClicked();
    void onPlateClicked();

private:
    Ui::StartWindow ui;
    Module m_selectedModule;
};

#endif // STARTWINDOW_H
