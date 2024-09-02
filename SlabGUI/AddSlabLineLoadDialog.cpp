#include "AddSlabLineLoadDialog.h"
#include "ui_AddSlabLineLoadDialog.h"

AddSlabLineLoadDialog::AddSlabLineLoadDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddSlabLineLoadDialog)
{
    ui->setupUi(this);
}

AddSlabLineLoadDialog::~AddSlabLineLoadDialog()
{
    delete ui;
}

void AddSlabLineLoadDialog::moveToBottomLeft()
{
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x + 5, y);
    }
}

int AddSlabLineLoadDialog::getX1()
{
    return ui->X1LineEdit->text().toInt();
}

int AddSlabLineLoadDialog::getZ1()
{
    return ui->Z1LineEdit->text().toInt();
}

int AddSlabLineLoadDialog::getX2()
{
    return ui->X2LineEdit->text().toInt();
}

int AddSlabLineLoadDialog::getZ2()
{
    return ui->Z2LineEdit->text().toInt();
}

double AddSlabLineLoadDialog::getF()
{
    return ui->FLineEdit->text().toDouble();
}
