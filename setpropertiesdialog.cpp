#include "setpropertiesdialog.h"
#include "ui_setpropertiesdialog.h"

SetPropertiesDialog::SetPropertiesDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::SetPropertiesDialog)
{
    ui->setupUi(this);
    connect(ui->crossSectionsListComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &SetPropertiesDialog::onCrossSectionIndexChanged);
    // Debug statement to check lineToSetParamLineEdit initialization
    if (!ui->lineToSetParamLineEdit) {
        qWarning() << "lineToSetParamLineEdit is not properly initialized!";
    }
}

QString SetPropertiesDialog::getSelectedObjectType() const
{
    return ui->crossSectionsListComboBox->currentText();
}

SetPropertiesDialog::~SetPropertiesDialog()
{
    delete ui;
}

void SetPropertiesDialog::initializeWithType(const QString &selectedType)
{
    if (!selectedType.isEmpty()) {
        ui->crossSectionsListComboBox->setCurrentText(selectedType);
    }
}

int SetPropertiesDialog::getLineId() const
{
    if (ui->lineToSetParamLineEdit) {
        return ui->lineToSetParamLineEdit->text().toInt();
    } else {
        qWarning() << "lineToSetParamLineEdit not found!";
        return -1;
    }
}

void SetPropertiesDialog::moveToBottomLeft()
{
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x + 5, y);
    }
}

void SetPropertiesDialog::updateCrossSectionsList()
{
    if (ui->crossSectionsListComboBox) {
        ui->crossSectionsListComboBox->clear();
        for (const auto &crossSection : crossSectionsMap) {
            ui->crossSectionsListComboBox->addItem(QString::fromStdString(crossSection.second),
                                                   QVariant(crossSection.first));
        }
    } else {
        qWarning() << "crossSectionsListComboBox not found!";
    }
}

void SetPropertiesDialog::setCrossSections(const std::map<int, std::string> &crossSections)
{
    crossSectionsMap = crossSections;
    updateCrossSectionsList();
}

int SetPropertiesDialog::getCrossSectionId() const
{
    return crossSectionId;
}

void SetPropertiesDialog::onCrossSectionIndexChanged(int index)
{
    crossSectionId = index + 1;
}
