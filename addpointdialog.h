#ifndef ADDPOINTDIALOG_H
#define ADDPOINTDIALOG_H

#include <QDialog>

namespace Ui {
class AddPointDialog;
}

class AddPointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPointDialog(QWidget *parent = nullptr);
    ~AddPointDialog();

    double getXCoordinate() const;
    double getZCoordinate() const;
    void moveToBottomLeft();

private:
    Ui::AddPointDialog *ui;
};

#endif // ADDPOINTDIALOG_H
