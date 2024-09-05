#include "AddSlabPointLoadDialog.h"
#include "ui_AddSlabPointLoadDialog.h"

AddSlabPointLoadDialog::AddSlabPointLoadDialog(QWidget *parent)
    : SlabBaseDialog(parent)
    , ui(new Ui::AddSlabPointLoadDialog)
{
    ui->setupUi(this);
}

AddSlabPointLoadDialog::~AddSlabPointLoadDialog()
{
    delete ui;
}


int AddSlabPointLoadDialog::getX()
{
    return ui->XLineEdit->text().toInt();
}

int AddSlabPointLoadDialog::getZ()
{
    return ui->ZLineEdit->text().toInt();
}

double AddSlabPointLoadDialog::getF()
{
    return ui->FLineEdit->text().toDouble();
}
