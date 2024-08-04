#ifndef ADDPOINTDIALOG_H
#define ADDPOINTDIALOG_H

#include "basedialog.h"
#include <QDialog>

namespace Ui {
class AddPointDialog;
}

class AddPointDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit AddPointDialog(QWidget *parent = nullptr);
    ~AddPointDialog();

    double getXCoordinate() const;
    double getZCoordinate() const;


private:
    Ui::AddPointDialog *ui;
};

#endif // ADDPOINTDIALOG_H
