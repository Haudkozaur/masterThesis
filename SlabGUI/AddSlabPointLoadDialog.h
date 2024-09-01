#ifndef ADDSLABPOINTLOADDIALOG_H
#define ADDSLABPOINTLOADDIALOG_H

#include <QDialog>

namespace Ui {
class AddSlabPointLoadDialog;
}

class AddSlabPointLoadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSlabPointLoadDialog(QWidget *parent = nullptr);
    ~AddSlabPointLoadDialog();

    void moveToBottomLeft();
    int getX();
    int getZ();
    double getF();

private:
    Ui::AddSlabPointLoadDialog *ui;
};

#endif // ADDSLABPOINTLOADDIALOG_H
