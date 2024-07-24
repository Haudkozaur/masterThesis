#ifndef DELETEOBJECTDIALOG_H
#define DELETEOBJECTDIALOG_H

#include <QDialog>
#include "ui_DeleteObjectDialog.h"

class DeleteObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteObjectDialog(QWidget *parent = nullptr);
    QString getSelectedObjectType() const;
    void moveToBottomLeft();

private slots:
    void onObjectTypeChanged(const QString &type);

private:
    void updateLayoutForType(const QString &type);

    Ui::DeleteObjectDialog ui;
    QWidget *currentOptionsWidget;
};

#endif // DELETEOBJECTDIALOG_H
