#ifndef ADDSLABSUPPORTSDIALOG_H
#define ADDSLABSUPPORTSDIALOG_H

#include <QDialog>

namespace Ui {
class AddSlabSupportsDialog;
}

class AddSlabSupportsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSlabSupportsDialog(QWidget *parent = nullptr);
    ~AddSlabSupportsDialog();
    void moveToBottomLeft();



    int getLineID();
private:
    Ui::AddSlabSupportsDialog *ui;
};

#endif // ADDSLABSUPPORTSDIALOG_H
