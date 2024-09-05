#include "AddSlabLineLoadDialog.h"
#include "ui_AddSlabLineLoadDialog.h"

AddSlabLineLoadDialog::AddSlabLineLoadDialog(QWidget *parent)
    : SlabBaseDialog(parent)
    , ui(new Ui::AddSlabLineLoadDialog)
{
    ui->setupUi(this);
}

AddSlabLineLoadDialog::~AddSlabLineLoadDialog()
{
    delete ui;
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
