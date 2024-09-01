#ifndef ADDSLABLINELOADDIALOG_H
#define ADDSLABLINELOADDIALOG_H

#include <QDialog>

namespace Ui {
class AddSlabLineLoadDialog;
}

class AddSlabLineLoadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSlabLineLoadDialog(QWidget *parent = nullptr);
    ~AddSlabLineLoadDialog();

    void moveToBottomLeft();
    int getX1();
    int getZ1();
    int getX2();
    int getZ2();
    double getF();

private:
    Ui::AddSlabLineLoadDialog *ui;
};

#endif // ADDSLABLINELOADDIALOG_H
