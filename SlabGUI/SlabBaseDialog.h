#ifndef SLABBASEDIALOG_H
#define SLABBASEDIALOG_H

#include <QDialog>

namespace Ui {
class SlabBaseDialog;
}

class SlabBaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SlabBaseDialog(QWidget *parent = nullptr);
    ~SlabBaseDialog();

    void moveToBottomLeft();

private:
    Ui::SlabBaseDialog *ui;
};

#endif // SLABBASEDIALOG_H
