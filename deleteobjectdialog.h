#ifndef DELETEOBJECTDIALOG_H
#define DELETEOBJECTDIALOG_H

#include <QDialog>
#include <QUiLoader>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class DeleteObjectDialog; }
QT_END_NAMESPACE

class DeleteObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteObjectDialog(QWidget *parent = nullptr);
    ~DeleteObjectDialog();

    QString getSelectedObjectType() const;
    void moveToBottomLeft();  // Przenieś metodę tutaj do sekcji public

private slots:
    void onObjectTypeChanged(const QString &type);
    void loadLayoutFromFile(const QString &fileName);
    int getPointId();

private:
    void updateLayoutForType(const QString &type);  // Deklaracja metody

    Ui::DeleteObjectDialog *ui;
    QWidget *currentOptionsWidget;
    QUiLoader *uiLoader;
};

#endif // DELETEOBJECTDIALOG_H
