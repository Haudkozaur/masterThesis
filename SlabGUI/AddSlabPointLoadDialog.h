#ifndef ADDSLABPOINTLOADDIALOG_H
#define ADDSLABPOINTLOADDIALOG_H
#include "SlabBaseDialog.h"
#include <QDialog>

namespace Ui {
class AddSlabPointLoadDialog;
}

class AddSlabPointLoadDialog : public SlabBaseDialog
{
    Q_OBJECT

public:
    explicit AddSlabPointLoadDialog(QWidget *parent = nullptr);
    ~AddSlabPointLoadDialog();

    int getX();
    int getZ();
    double getF();

private:
    Ui::AddSlabPointLoadDialog *ui;
};

#endif // ADDSLABPOINTLOADDIALOG_H
