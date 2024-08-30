#include "AddSlabSupportsDialog.h"
#include "ui_AddSlabSupportsDialog.h"

AddSlabSupportsDialog::AddSlabSupportsDialog(QWidget *parent)
    : QDialog(parent)
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

void AddSlabSupportsDialog::moveToBottomLeft() {
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x + 5, y);
    }
}
