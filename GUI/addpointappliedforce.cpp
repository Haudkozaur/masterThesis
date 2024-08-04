#include "addpointappliedforce.h"
#include "ui_addpointappliedforce.h"

AddPointAppliedForce::AddPointAppliedForce(QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::AddPointAppliedForce)
{
    ui->setupUi(this);
}

AddPointAppliedForce::~AddPointAppliedForce()
{
    delete ui;
}

int AddPointAppliedForce::getPointId()
{
    return ui->pointIdLoadLineEdit->text().toInt();

}

double AddPointAppliedForce::getFz()
{
    return ui->FzLineEdit->text().toDouble();
}

double AddPointAppliedForce::getFx()
{
    return ui->FxLineEdit->text().toDouble();
}

double AddPointAppliedForce::getMy()
{
    return ui->MyLineEdit->text().toDouble();
}


