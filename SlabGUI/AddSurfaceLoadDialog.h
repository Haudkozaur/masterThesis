#ifndef ADDSURFACELOADDIALOG_H
#define ADDSURFACELOADDIALOG_H

#include <QDialog>

namespace Ui {
class AddSurfaceLoadDialog;
}

class AddSurfaceLoadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSurfaceLoadDialog(QWidget *parent = nullptr);
    ~AddSurfaceLoadDialog();

    void moveToBottomLeft();
    int getX1() const;
    int getZ1() const;
    int getX2() const;
    int getZ2() const;
    double getF();

private:
    Ui::AddSurfaceLoadDialog *ui;
};

#endif // ADDSURFACELOADDIALOG_H
