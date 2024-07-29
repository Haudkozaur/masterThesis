#ifndef ADDCROSSSECTIONDIALOG_H
#define ADDCROSSSECTIONDIALOG_H

#include <QDialog>
#include <QUiLoader>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>

namespace Ui {
class AddCrossSectionDialog;
}

class AddCrossSectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddCrossSectionDialog(QWidget *parent = nullptr);
    ~AddCrossSectionDialog();

    void initializeWithType(const QString &selectedType);
    QString getSelectedObjectType() const;

    void moveToBottomLeft();

    double getInnertia();
    std::string getName();
    double getArea();
    int getHeight();
    int getWidth();
    int getRadius();

    std::string getNameForRect();
    std::string getNameForCirc();

    void updateMaterialsList();
    void setMaterials(const std::map<int, std::string> &materials);
    std::map<int, std::string> materialsMap;

    int materialId = -1;
    int getMaterialId() const;

private slots:

    void onMaterialIndexChanged(int index);
    void onMaterialChanged(const QString &materialName);
private:
    Ui::AddCrossSectionDialog *ui;
    QUiLoader *uiLoader;
    QWidget *currentOptionsWidget;

    void onObjectTypeChanged(const QString &type);
    void loadLayoutFromFile(const QString &fileName);
    void updateLayoutForType(const QString &type);

    QLineEdit *csInnertiaLineEdit;
    QLineEdit *csNameLineEdit;
    QLineEdit *csAreaLineEdit;

    QLineEdit *csHeightLineEdit;
    QLineEdit *csWidthtLineEdit;
    QLineEdit *csRNameLineEdit;

    QLineEdit *csRadiusLineEdit;
    QLineEdit *csCNameLineEdit;

    QComboBox *materialComboBox;

};

#endif // ADDCROSSSECTIONDIALOG_H
