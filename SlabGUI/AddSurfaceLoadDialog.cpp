#include "AddSurfaceLoadDialog.h"
#include "ui_AddSurfaceLoadDialog.h"

AddSurfaceLoadDialog::AddSurfaceLoadDialog(QWidget *parent)
    : SlabBaseDialog(parent)
    , ui(new Ui::AddSurfaceLoadDialog)
{
    ui->setupUi(this);
}

AddSurfaceLoadDialog::~AddSurfaceLoadDialog()
{
    delete ui;
}


int AddSurfaceLoadDialog::getX1() const
{
    return ui->X1LineEdit->text().toInt();
}

int AddSurfaceLoadDialog::getZ1() const
{
    return ui->Z1LineEdit->text().toInt();
}

int AddSurfaceLoadDialog::getX2() const
{
    return ui->X2LineEdit->text().toInt();
}

int AddSurfaceLoadDialog::getZ2() const
{
    return ui->Z2LineEdit->text().toInt();
}

double AddSurfaceLoadDialog::getF()
{
    return ui->FLineEdit->text().toDouble();
}
