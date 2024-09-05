#ifndef SLABDELETEOBJECTDIALOG_H
#define SLABDELETEOBJECTDIALOG_H

#include "SlabBaseDialog.h"
#include <QDialog>
#include <QLineEdit>
#include <QUiLoader>
#include <QMessageBox>

namespace Ui {
class SlabDeleteObjectDialog;
}

class SlabDeleteObjectDialog : public SlabBaseDialog
{
    Q_OBJECT

public:
    explicit SlabDeleteObjectDialog(QWidget *parent = nullptr);
    ~SlabDeleteObjectDialog();

    void initializeWithType(const QString &selectedType); // Uniwersalna metoda

private:
    void loadLayoutFromFile(const QString &fileName); // Uniwersalna metoda
    void updateLayoutForType(const QString &type);    // Uniwersalna metoda
    void onObjectTypeChanged(const QString &type);    // Uniwersalna metoda
    void showError(const QString &message);           // Uniwersalna metoda

    Ui::SlabDeleteObjectDialog *ui;
    QWidget *currentOptionsWidget;
    QUiLoader *uiLoader;

    QLineEdit *slabToDeleteLineEdit;   // Specyficzne pola dla Slab
};

#endif // SLABDELETEOBJECTDIALOG_H
