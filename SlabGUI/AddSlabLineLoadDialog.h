#ifndef ADDSLABLINELOADDIALOG_H
#define ADDSLABLINELOADDIALOG_H
#include "SlabBaseDialog.h"
#include <QDialog>

namespace Ui {
class AddSlabLineLoadDialog;
}

class AddSlabLineLoadDialog : public SlabBaseDialog
{
    Q_OBJECT

public:
    explicit AddSlabLineLoadDialog(QWidget *parent = nullptr);
    ~AddSlabLineLoadDialog();


    int getX1();
    int getZ1();
    int getX2();
    int getZ2();
    double getF();

private:
    Ui::AddSlabLineLoadDialog *ui;
};

#endif // ADDSLABLINELOADDIALOG_H
