#include "addpointappliedforce.h"
#include "ui_addpointappliedforce.h"

AddPointAppliedForce::AddPointAppliedForce(QWidget *parent)
    : QDialog(parent)
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

void AddPointAppliedForce::moveToBottomLeft()
{
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x + 5, y);
    }
}
