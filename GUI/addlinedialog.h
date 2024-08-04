#ifndef ADDLINEDIALOG_H
#define ADDLINEDIALOG_H

#include "basedialog.h"
#include <QDialog>

namespace Ui {
class AddLineDialog;
}

class AddLineDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit AddLineDialog(QWidget *parent = nullptr);
    ~AddLineDialog();

    int getFirstPointId() const;
    int getSecondPointId() const;


private:
    Ui::AddLineDialog *ui;
};

#endif // ADDLINEDIALOG_H
