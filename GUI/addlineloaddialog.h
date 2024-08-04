#ifndef ADDLINELOADDIALOG_H
#define ADDLINELOADDIALOG_H

#include "basedialog.h"
#include <QDialog>

namespace Ui {
class AddLineLoadDialog;
}

class AddLineLoadDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit AddLineLoadDialog(QWidget *parent = nullptr);
    ~AddLineLoadDialog();

    int getLineId();
    double getFz();
    double getFx();


private:
    Ui::AddLineLoadDialog *ui;
};

#endif // ADDLINELOADDIALOG_H
