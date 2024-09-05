#include "AddSlabSupportsDialog.h"
#include "ui_AddSlabSupportsDialog.h"

AddSlabSupportsDialog::AddSlabSupportsDialog(QWidget *parent)
    : SlabBaseDialog(parent)
    , ui(new Ui::AddSlabSupportsDialog)
{
    ui->setupUi(this);
}

AddSlabSupportsDialog::~AddSlabSupportsDialog()
{
    delete ui;
}

int AddSlabSupportsDialog::getLineID()
{
    return ui->lineToSupportLineEdit->text().toInt();
}


