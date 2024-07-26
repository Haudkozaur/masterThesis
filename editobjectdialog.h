#ifndef EDITOBJECTDIALOG_H
#define EDITOBJECTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QWidget>
#include <QUiLoader>

namespace Ui {
class EditObjectDialog;
}

class EditObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditObjectDialog(QWidget *parent = nullptr);
    ~EditObjectDialog();

    QString getSelectedObjectType() const;
    int getPointId();
    int getLineId();
    int getSupportPointId();
    void moveToBottomLeft();

private slots:
    void onObjectTypeChanged(const QString &type);

private:
    Ui::EditObjectDialog *ui;
    QWidget *currentOptionsWidget;

    QUiLoader *uiLoader;

    void loadLayoutFromFile(const QString &fileName);
    void updateLayoutForType(const QString &type);
    QLineEdit *pointToEditLineEdit;
    QLineEdit *lineToEditLineEdit;
    QLineEdit *pointSupportToEditLineEdit;
};

#endif // EDITOBJECTDIALOG_H
