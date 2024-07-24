#ifndef ADDBOUNDARIESDIALOG_H
#define ADDBOUNDARIESDIALOG_H

#include <QDialog>

namespace Ui {
class AddBoundariesDialog;
}

class AddBoundariesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBoundariesDialog(QWidget *parent = nullptr);
    ~AddBoundariesDialog();

    int getPointId() const;
    bool getRy() const;
    bool getTx() const;
    bool getTz() const;
    void moveToBottomLeft();

private:
    Ui::AddBoundariesDialog *ui;
};

#endif // ADDBOUNDARIESDIALOG_H
