#ifndef ADDMATERIALDIALOG_H
#define ADDMATERIALDIALOG_H

#include "basedialog.h"
#include <QDialog>

namespace Ui {
class AddMaterialDialog;
}

class AddMaterialDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit AddMaterialDialog(QWidget *parent = nullptr);
    ~AddMaterialDialog();


    std::string getMaterialName() const;
    double getYoungModulus() const;
    double getPoissonCoefficient() const;
    double getDensity() const;


private:
    Ui::AddMaterialDialog *ui;
};

#endif // ADDMATERIALDIALOG_H
