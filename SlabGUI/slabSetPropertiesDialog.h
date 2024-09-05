#ifndef SLABSETPROPERTIESDIALOG_H
#define SLABSETPROPERTIESDIALOG_H
#include "SlabBaseDialog.h"
#include <QComboBox>
#include <QDialog>

namespace Ui {
class slabSetPropertiesDialog;
}

class slabSetPropertiesDialog : public SlabBaseDialog
{
    Q_OBJECT

public:
    explicit slabSetPropertiesDialog(QWidget *parent = nullptr);
    ~slabSetPropertiesDialog();

    void updateMaterialsList();
    void setMaterials(const std::map<int, std::string> &materials);
    std::map<int, std::string> materialsMap;

    int materialId = -1;

    QComboBox *materialComboBox;

    void onMaterialChanged(const QString &materialName);
    void onMaterialIndexChanged(int index);

    int getMaterialId() const;
    int getThickness() const;
private:
    Ui::slabSetPropertiesDialog *ui;
};

#endif // SLABSETPROPERTIESDIALOG_H
