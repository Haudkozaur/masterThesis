#ifndef ADDPOINTAPPLIEDFORCE_H
#define ADDPOINTAPPLIEDFORCE_H

#include <QDialog>

namespace Ui {
class AddPointAppliedForce;
}

class AddPointAppliedForce : public QDialog
{
    Q_OBJECT

public:
    explicit AddPointAppliedForce(QWidget *parent = nullptr);
    ~AddPointAppliedForce();

    int getPointId();
    double getFz();
    double getFx();
    double getMy();

    void moveToBottomLeft();

private:
    Ui::AddPointAppliedForce *ui;
};

#endif // ADDPOINTAPPLIEDFORCE_H
