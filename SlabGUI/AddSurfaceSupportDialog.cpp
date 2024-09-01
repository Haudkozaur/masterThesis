#include "AddSurfaceSupportDialog.h"
#include "ui_AddSurfaceSupportDialog.h"

AddSurfaceSupportDialog::AddSurfaceSupportDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddSurfaceSupportDialog)
{
    ui->setupUi(this);
}

AddSurfaceSupportDialog::~AddSurfaceSupportDialog()
{
    delete ui;
}

void AddSurfaceSupportDialog::moveToBottomLeft() {
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x + 5, y);
    }
}

int AddSurfaceSupportDialog::getX1() const
{
    return ui->X1LineEdit->text().toInt();
}

int AddSurfaceSupportDialog::getZ1() const
{
    return ui->Z1LineEdit->text().toInt();
}

int AddSurfaceSupportDialog::getX2() const
{
    return ui->X2LineEdit->text().toInt();
}

int AddSurfaceSupportDialog::getZ2() const
{
    return ui->Z2LineEdit->text().toInt();
}
