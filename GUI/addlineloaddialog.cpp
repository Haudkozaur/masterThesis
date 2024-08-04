#include "addlineloaddialog.h"
#include "ui_addlineloaddialog.h"

AddLineLoadDialog::AddLineLoadDialog(QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::AddLineLoadDialog)
{
    ui->setupUi(this);
}

AddLineLoadDialog::~AddLineLoadDialog()
{
    delete ui;
}

int AddLineLoadDialog::getLineId()
{
    return ui->lineIdLoadLineEdit->text().toInt();
}

double AddLineLoadDialog::getFz()
{
    return ui->lineFzLineEdit->text().toDouble();
}

double AddLineLoadDialog::getFx()
{
    return ui->lineFxLineEdit->text().toDouble();
}


