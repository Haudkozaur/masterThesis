#include "addlineloaddialog.h"
#include "ui_addlineloaddialog.h"

AddLineLoadDialog::AddLineLoadDialog(QWidget *parent)
    : QDialog(parent)
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

void AddLineLoadDialog::moveToBottomLeft()
{
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x + 5, y);
    }
}
