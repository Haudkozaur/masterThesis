#ifndef SETPROPERTIESDIALOG_H
#define SETPROPERTIESDIALOG_H

#include "basedialog.h"
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

namespace Ui {
class SetPropertiesDialog;
}

class SetPropertiesDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit SetPropertiesDialog(QWidget *parent = nullptr);
    ~SetPropertiesDialog();

    int getLineId() const;
    int getCrossSectionId() const;

    void setCrossSections(const std::map<int, std::string> &crossSections);

    void onCrossSectionIndexChanged(int index);

    void updateCrossSectionsList();

    int crossSectionId;
    std::map<int, std::string> crossSectionsMap;

    void initializeWithType(const QString &selectedType);

    QString getSelectedObjectType() const;


    private:
    Ui::SetPropertiesDialog *ui;

};

#endif // SETPROPERTIESDIALOG_H
