#include "SlabBaseDialog.h"
#include "ui_SlabBaseDialog.h"

SlabBaseDialog::SlabBaseDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SlabBaseDialog)
{
    ui->setupUi(this);
}

SlabBaseDialog::~SlabBaseDialog()
{
    delete ui;
}

void SlabBaseDialog::moveToBottomLeft() {
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 365;
        move(x + 5, y);
    }
}
