#ifndef ADDSURFACELOADDIALOG_H
#define ADDSURFACELOADDIALOG_H
#include "SlabBaseDialog.h"
#include <QDialog>

namespace Ui {
class AddSurfaceLoadDialog;
}

class AddSurfaceLoadDialog : public SlabBaseDialog
{
    Q_OBJECT

public:
    explicit AddSurfaceLoadDialog(QWidget *parent = nullptr);
    ~AddSurfaceLoadDialog();


    int getX1() const;
    int getZ1() const;
    int getX2() const;
    int getZ2() const;
    double getF();

private:
    Ui::AddSurfaceLoadDialog *ui;
};

#endif // ADDSURFACELOADDIALOG_H
