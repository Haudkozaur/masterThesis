// EditObjectDialog.h
#ifndef EDITOBJECTDIALOG_H
#define EDITOBJECTDIALOG_H

#include "basedialog.h"
#include <QDialog>
#include <QString>
#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QUiLoader>

namespace Ui {
class EditObjectDialog;
}

class EditObjectDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit EditObjectDialog(QWidget *parent = nullptr);
    ~EditObjectDialog();

    void initializeWithType(const QString &selectedType);
    QString getSelectedObjectType() const;
    int getPointToEditId();
    int getNewXCord();
    int getNewZCord();
    int getLineId();
    int getNewStartPoint();
    int getNewEndPoint();
    int getSupportPointId();
    bool getTz();
    bool getTx();
    bool getRy();


private:
    Ui::EditObjectDialog *ui;
    QUiLoader *uiLoader;
    QWidget *currentOptionsWidget;
    QLineEdit *pointToEditLineEdit;
    QLineEdit *xEditLineEdit;
    QLineEdit *zEditLineEdit;
    QLineEdit *lineToEditLineEdit;
    QLineEdit *fEditLineEdit;
    QLineEdit *eEditLineEdit;
    QLineEdit *pointSupportToEditLineEdit;
    QCheckBox *Tz;
    QCheckBox *Tx;
    QCheckBox *Ry;

    void onObjectTypeChanged(const QString &type);
    void loadLayoutFromFile(const QString &fileName);
    void updateLayoutForType(const QString &type);

};

#endif // EDITOBJECTDIALOG_H
