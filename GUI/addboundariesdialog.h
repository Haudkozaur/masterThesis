#ifndef ADDBOUNDARIESDIALOG_H
#define ADDBOUNDARIESDIALOG_H

#include "basedialog.h"
#include <QDialog>

namespace Ui {
class AddBoundariesDialog;
}

class AddBoundariesDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit AddBoundariesDialog(QWidget *parent = nullptr);
    ~AddBoundariesDialog();

    int getPointId() const;
    bool getRy() const;
    bool getTx() const;
    bool getTz() const;

private:
    Ui::AddBoundariesDialog *ui;
};

#endif // ADDBOUNDARIESDIALOG_H
