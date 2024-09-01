#ifndef ADDSURFACESUPPORTDIALOG_H
#define ADDSURFACESUPPORTDIALOG_H

#include <QDialog>

namespace Ui {
class AddSurfaceSupportDialog;
}

class AddSurfaceSupportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSurfaceSupportDialog(QWidget *parent = nullptr);
    ~AddSurfaceSupportDialog();

    void moveToBottomLeft();
    int getX1() const;
    int getZ1() const;
    int getX2() const;
    int getZ2() const;

private:
    Ui::AddSurfaceSupportDialog *ui;
};

#endif // ADDSURFACESUPPORTDIALOG_H
