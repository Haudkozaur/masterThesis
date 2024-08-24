#include "SlabGUI.h"
#include "ui_SlabGUI.h"

SlabGUI::SlabGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SlabGUI)
{
    ui->setupUi(this);
}

SlabGUI::~SlabGUI()
{
    delete ui;
}
