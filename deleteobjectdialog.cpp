#include "DeleteObjectDialog.h"
#include <QVBoxLayout>
#include <QLabel>

DeleteObjectDialog::DeleteObjectDialog(QWidget *parent)
    : QDialog(parent), currentOptionsWidget(nullptr)
{
    ui.setupUi(this);

    connect(ui.comboBoxObjectType, &QComboBox::currentTextChanged, this, &DeleteObjectDialog::onObjectTypeChanged);

    // Initialize layout for the current type
    updateLayoutForType(ui.comboBoxObjectType->currentText());

    // Connect dialog buttons
    connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString DeleteObjectDialog::getSelectedObjectType() const
{
    return ui.comboBoxObjectType->currentText();
}

void DeleteObjectDialog::onObjectTypeChanged(const QString &type)
{
    updateLayoutForType(type);
}

void DeleteObjectDialog::updateLayoutForType(const QString &type)
{
    if (currentOptionsWidget) {
        ui.mainLayout->removeWidget(currentOptionsWidget);
        delete currentOptionsWidget;
        currentOptionsWidget = nullptr;
    }

    currentOptionsWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(currentOptionsWidget);

    if (type == "Points") {
        layout->addWidget(new QLabel("Option for Points:", currentOptionsWidget));
        // Add more widgets as needed
    } else if (type == "Lines") {
        layout->addWidget(new QLabel("Option for Lines:", currentOptionsWidget));
        // Add more widgets as needed
    } else if (type == "Supports") {
        layout->addWidget(new QLabel("Option for Supports:", currentOptionsWidget));
        // Add more widgets as needed
    }

    currentOptionsWidget->setLayout(layout);
    ui.mainLayout->addWidget(currentOptionsWidget);
}
void DeleteObjectDialog::moveToBottomLeft()
{
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x, y);
    }
}
