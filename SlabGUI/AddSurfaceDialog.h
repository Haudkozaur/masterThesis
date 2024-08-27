#ifndef ADDSURFACEDIALOG_H
#define ADDSURFACEDIALOG_H

#include <QDialog>
#include <QUiLoader>
#include <QLineEdit>

namespace Ui {
class AddSurfaceDialog;
}

class AddSurfaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSurfaceDialog(QWidget *parent = nullptr);
    ~AddSurfaceDialog();

    int getDiameter() const;
    int getCentreX1() const;
    int getCentreZ1() const;

    int getX1() const;
    int getZ1() const;
    int getX2() const;
    int getZ2() const;

    int getTriX1() const;
    int getTriZ1() const;
    int getTriX2() const;
    int getTriZ2() const;
    int getTriX3() const;
    int getTriZ3() const;

    bool getIsOpening();

    void moveToBottomLeft();
    void initializeWithType(const QString &selectedType);
    QString getSelectedObjectType() const;


private slots:
    void on_openingCheckBox_toggled(bool checked);

private:
    Ui::AddSurfaceDialog *ui;



    QUiLoader *uiLoader;
    QWidget *currentOptionsWidget;
    QLineEdit *diameterLineEdit;
    QLineEdit *centreX1LineEdit;
    QLineEdit *centreZ1LineEdit;

    QLineEdit *X1LineEdit;
    QLineEdit *Z1LineEdit;
    QLineEdit *X2LineEdit;
    QLineEdit *Z2LineEdit;

    QLineEdit *triX1LineEdit;
    QLineEdit *triZ1LineEdit;
    QLineEdit *triX2LineEdit;
    QLineEdit *triZ2LineEdit;
    QLineEdit *triX3LineEdit;
    QLineEdit *triZ3LineEdit;

    bool isOpening;

    void onObjectTypeChanged(const QString &type);
    void loadLayoutFromFile(const QString &fileName);
    void updateLayoutForType(const QString &type);


};

#endif // ADDSURFACEDIALOG_H
