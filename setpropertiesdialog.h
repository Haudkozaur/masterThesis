#ifndef SETPROPERTIESDIALOG_H
#define SETPROPERTIESDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

namespace Ui {
class SetPropertiesDialog;
}

class SetPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetPropertiesDialog(QWidget *parent = nullptr);
    ~SetPropertiesDialog();

    int getLineId() const;
    void moveToBottomLeft();

    void setCrossSections(const std::map<int, std::string> &crossSections);

    int getCrossSectionId() const;

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
