#ifndef ADDSLABSUPPORTSDIALOG_H
#define ADDSLABSUPPORTSDIALOG_H
#include "SlabBaseDialog.h"
#include <QDialog>

namespace Ui {
class AddSlabSupportsDialog;
}

class AddSlabSupportsDialog : public SlabBaseDialog
{
    Q_OBJECT

public:
    explicit AddSlabSupportsDialog(QWidget *parent = nullptr);
    ~AddSlabSupportsDialog();




    int getLineID();
private:
    Ui::AddSlabSupportsDialog *ui;
};

#endif // ADDSLABSUPPORTSDIALOG_H
