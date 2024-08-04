#include "basedialog.h"

BaseDialog::BaseDialog(QWidget *parent)
    : QDialog(parent) {
}

BaseDialog::~BaseDialog() {
}

void BaseDialog::moveToBottomLeft() {
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x + 5, y);
    }
}
