#include "addlinedialog.h"
#include "ui_addlinedialog.h"

AddLineDialog::AddLineDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddLineDialog)
{
    ui->setupUi(this);
}

AddLineDialog::~AddLineDialog()
{
    delete ui;
}

int AddLineDialog::getFirstPointId() const
{
    return ui->fPointLineEdit->text().toInt();
}

int AddLineDialog::getSecondPointId() const
{
    return ui->sPointLineEdit->text().toInt();
}
void AddLineDialog::moveToBottomLeft()
{
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x, y);
    }
}
