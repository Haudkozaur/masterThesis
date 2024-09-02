#include "AddSurfaceLoadDialog.h"
#include "ui_AddSurfaceLoadDialog.h"

AddSurfaceLoadDialog::AddSurfaceLoadDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddSurfaceLoadDialog)
{
    ui->setupUi(this);
}

AddSurfaceLoadDialog::~AddSurfaceLoadDialog()
{
    delete ui;
}

void AddSurfaceLoadDialog::moveToBottomLeft()
{
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x + 5, y);
    }
}
int AddSurfaceLoadDialog::getX1() const
{
    return ui->X1LineEdit->text().toInt();
}

int AddSurfaceLoadDialog::getZ1() const
{
    return ui->Z1LineEdit->text().toInt();
}

int AddSurfaceLoadDialog::getX2() const
{
    return ui->X2LineEdit->text().toInt();
}

int AddSurfaceLoadDialog::getZ2() const
{
    return ui->Z2LineEdit->text().toInt();
}

double AddSurfaceLoadDialog::getF()
{
    return ui->FLineEdit->text().toDouble();
}
