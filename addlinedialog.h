#ifndef ADDLINEDIALOG_H
#define ADDLINEDIALOG_H

#include <QDialog>

namespace Ui {
class AddLineDialog;
}

class AddLineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddLineDialog(QWidget *parent = nullptr);
    ~AddLineDialog();

    int getFirstPointId() const;
    int getSecondPointId() const;
    void moveToBottomLeft();

private:
    Ui::AddLineDialog *ui;
};

#endif // ADDLINEDIALOG_H
