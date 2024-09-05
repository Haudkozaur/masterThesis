#include "SlabDeleteObjectDialog.h"
#include <QFile>
#include "ui_SlabDeleteObjectDialog.h"

SlabDeleteObjectDialog::SlabDeleteObjectDialog(QWidget *parent)
    : SlabBaseDialog(parent)
    , ui(new Ui::SlabDeleteObjectDialog)
    , currentOptionsWidget(nullptr)
    , uiLoader(new QUiLoader(this))
{
    ui->setupUi(this);

    connect(ui->comboBoxObjectType,
            &QComboBox::currentTextChanged,
            this,
            &SlabDeleteObjectDialog::onObjectTypeChanged);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

SlabDeleteObjectDialog::~SlabDeleteObjectDialog()
{
    delete ui;
}

void SlabDeleteObjectDialog::initializeWithType(const QString &selectedType)
{
    if (!selectedType.isEmpty()) {
        ui->comboBoxObjectType->setCurrentText(selectedType);
        updateLayoutForType(selectedType);
    } else {
        updateLayoutForType(ui->comboBoxObjectType->currentText());
    }
}

void SlabDeleteObjectDialog::onObjectTypeChanged(const QString &type)
{
    if (currentOptionsWidget) {
        ui->mainLayout->removeWidget(currentOptionsWidget);
        delete currentOptionsWidget;
        currentOptionsWidget = nullptr;
    }

    QString fileName;
    if (type == "Slabs") {
        fileName = ":/ui/deleteSlabsLayout.ui"; // Specyficzne pliki dla Slabs
    }

    loadLayoutFromFile(fileName);
}

void SlabDeleteObjectDialog::loadLayoutFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        showError("Cannot open file: " + fileName);
        return;
    }

    currentOptionsWidget = uiLoader->load(&file, this);
    file.close();

    if (currentOptionsWidget) {
        ui->mainLayout->insertWidget(1, currentOptionsWidget);

        slabToDeleteLineEdit = currentOptionsWidget->findChild<QLineEdit *>("slabToDeleteLineEdit");
    } else {
        showError("Failed to load the layout from file: " + fileName);
    }
}

void SlabDeleteObjectDialog::updateLayoutForType(const QString &type)
{
    onObjectTypeChanged(type);
}

void SlabDeleteObjectDialog::showError(const QString &message)
{
    QMessageBox::warning(this, "Error", message);
}
