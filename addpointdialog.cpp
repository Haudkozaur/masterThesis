#include "addpointdialog.h"
#include "ui_addpointdialog.h"

AddPointDialog::AddPointDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPointDialog)
{
    ui->setupUi(this);
}

AddPointDialog::~AddPointDialog()
{
    delete ui;
}

double AddPointDialog::getXCoordinate() const
{
    return ui->xLineEdit->text().toDouble();
}

double AddPointDialog::getZCoordinate() const
{
    return ui->zLineEdit->text().toDouble();
}
