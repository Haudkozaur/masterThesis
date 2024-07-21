#include "addboundariesdialog.h"
#include "ui_addboundariesdialog.h"

AddBoundariesDialog::AddBoundariesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddBoundariesDialog)
{
    ui->setupUi(this);
}

AddBoundariesDialog::~AddBoundariesDialog()
{
    delete ui;
}

int AddBoundariesDialog::getPointId() const
{
    return ui->BoundaryPointLineEdit->text().toInt();
}

bool AddBoundariesDialog::getRy() const
{
    return ui->RyCheckBox->isChecked();
}

bool AddBoundariesDialog::getTx() const
{
    return ui->TxCheckBox->isChecked();
}

bool AddBoundariesDialog::getTz() const
{
    return ui->TzCheckBox->isChecked();
}
