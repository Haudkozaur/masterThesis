#include "addmaterialdialog.h"
#include "ui_addmaterialdialog.h"

AddMaterialDialog::AddMaterialDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddMaterialDialog)
{
    ui->setupUi(this);
}

AddMaterialDialog::~AddMaterialDialog()
{
    delete ui;
}

std::string AddMaterialDialog::getMaterialName() const
{
    return ui->materialNameLineEdit->text().toStdString();
}

double AddMaterialDialog::getYoungModulus() const
{
    return ui->elasticityModulusLineEdit->text().toDouble();
}

double AddMaterialDialog::getPoissonCoefficient() const
{
    return ui->poissonLineEdit->text().toDouble();
}

double AddMaterialDialog::getDensity() const
{
    return ui->densityLineEdit->text().toDouble();
}
void AddMaterialDialog::moveToBottomLeft()
{
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x + 5, y);
    }
}
