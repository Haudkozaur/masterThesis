#ifndef ADDLINELOADDIALOG_H
#define ADDLINELOADDIALOG_H

#include <QDialog>

namespace Ui {
class AddLineLoadDialog;
}

class AddLineLoadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddLineLoadDialog(QWidget *parent = nullptr);
    ~AddLineLoadDialog();

    int getLineId();
    double getFz();
    double getFx();


    void moveToBottomLeft();

private:
    Ui::AddLineLoadDialog *ui;
};

#endif // ADDLINELOADDIALOG_H
