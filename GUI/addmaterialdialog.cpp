#include "addmaterialdialog.h"
#include "ui_addmaterialdialog.h"

AddMaterialDialog::AddMaterialDialog(QWidget *parent)
    : BaseDialog(parent)
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

