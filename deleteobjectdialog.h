#ifndef DELETEOBJECTDIALOG_H
#define DELETEOBJECTDIALOG_H

#include <QDialog>
#include <QLineEdit> // Dodaj ten nagłówek
#include <QUiLoader>

namespace Ui {
class DeleteObjectDialog;
}

class DeleteObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteObjectDialog(QWidget *parent = nullptr);
    ~DeleteObjectDialog();

    void initializeWithType(const QString &selectedType);

    QString getSelectedObjectType() const;
    int getPointId();
    int getLineId();
    int getSupportPointId();
    void moveToBottomLeft();


private:
    void loadLayoutFromFile(const QString &fileName);
    void updateLayoutForType(const QString &type);
    void onObjectTypeChanged(const QString &type);

    Ui::DeleteObjectDialog *ui;
    QWidget *currentOptionsWidget;
    QUiLoader *uiLoader;

    QLineEdit *pointToDeleteLineEdit;        // Dodaj wskaźnik do QLineEdit
    QLineEdit *lineToDeleteLineEdit;         // Dodajemy wskaźnik do QLineEdit dla ID linii
    QLineEdit *pointSupportToDeleteLineEdit; // Dodajemy wskaźnik do QLineEdit dla ID punktu podpor
};

#endif // DELETEOBJECTDIALOG_H
