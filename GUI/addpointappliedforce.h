#ifndef ADDPOINTAPPLIEDFORCE_H
#define ADDPOINTAPPLIEDFORCE_H

#include "basedialog.h"
#include <QDialog>

namespace Ui {
class AddPointAppliedForce;
}

class AddPointAppliedForce : public BaseDialog
{
    Q_OBJECT

public:
    explicit AddPointAppliedForce(QWidget *parent = nullptr);
    ~AddPointAppliedForce();

    int getPointId();
    double getFz();
    double getFx();
    double getMy();



private:
    Ui::AddPointAppliedForce *ui;
};

#endif // ADDPOINTAPPLIEDFORCE_H
