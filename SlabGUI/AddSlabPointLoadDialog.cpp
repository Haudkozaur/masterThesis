#include "AddSlabPointLoadDialog.h"
#include "ui_AddSlabPointLoadDialog.h"

AddSlabPointLoadDialog::AddSlabPointLoadDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddSlabPointLoadDialog)
{
    ui->setupUi(this);
}

AddSlabPointLoadDialog::~AddSlabPointLoadDialog()
{
    delete ui;
}

void AddSlabPointLoadDialog::moveToBottomLeft()
{
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x + 5, y);
    }
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
