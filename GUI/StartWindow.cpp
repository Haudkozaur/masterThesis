#include "StartWindow.h"

StartWindow::StartWindow(QWidget *parent)
    : QDialog(parent), m_selectedModule(None)
{
    ui.setupUi(this);  // Ładowanie pliku .ui

    // Łączenie przycisków z akcjami
    connect(ui.frame2DButton, &QPushButton::clicked, this, &StartWindow::onFrame2DClicked);
    connect(ui.plateButton, &QPushButton::clicked, this, &StartWindow::onPlateClicked);
}

void StartWindow::onFrame2DClicked()
{
    m_selectedModule = Frame2D;
    accept();
}

void StartWindow::onPlateClicked()
{
    m_selectedModule = Plate;
    accept();
}
