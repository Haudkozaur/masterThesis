#ifndef ADDSURFACESUPPORTDIALOG_H
#define ADDSURFACESUPPORTDIALOG_H
#include "SlabBaseDialog.h"
#include <QDialog>

namespace Ui {
class AddSurfaceSupportDialog;
}

class AddSurfaceSupportDialog : public SlabBaseDialog
{
    Q_OBJECT

public:
    explicit AddSurfaceSupportDialog(QWidget *parent = nullptr);
    ~AddSurfaceSupportDialog();

    int getX1() const;
    int getZ1() const;
    int getX2() const;
    int getZ2() const;

private:
    Ui::AddSurfaceSupportDialog *ui;
};

#endif // ADDSURFACESUPPORTDIALOG_H
