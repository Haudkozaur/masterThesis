#include "DeleteObjectDialog.h"
#include <QFile>
#include "ui_DeleteObjectDialog.h"

DeleteObjectDialog::DeleteObjectDialog(QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::DeleteObjectDialog)
    , currentOptionsWidget(nullptr)
    , uiLoader(new QUiLoader(this))
{
    ui->setupUi(this);

    connect(ui->comboBoxObjectType,
            &QComboBox::currentTextChanged,
            this,
            &DeleteObjectDialog::onObjectTypeChanged);

    // Connect dialog buttons
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

DeleteObjectDialog::~DeleteObjectDialog()
{
    delete ui;
}

void DeleteObjectDialog::initializeWithType(const QString &selectedType)
{
    if (!selectedType.isEmpty()) {
        ui->comboBoxObjectType->setCurrentText(selectedType);
        updateLayoutForType(selectedType); // Ensure layout is updated
    } else {
        updateLayoutForType(ui->comboBoxObjectType->currentText());
    }
}
QString DeleteObjectDialog::getSelectedObjectType() const
{
    return ui->comboBoxObjectType->currentText();
}

void DeleteObjectDialog::onObjectTypeChanged(const QString &type)
{
    if (currentOptionsWidget) {
        ui->mainLayout->removeWidget(currentOptionsWidget);
        delete currentOptionsWidget;
        currentOptionsWidget = nullptr;
    }

    QString fileName;
    if (type == "Points") {
        fileName = ":/ui/deletepointslayout.ui";
    } else if (type == "Lines") {
        fileName = ":/ui/deletelineslayout.ui";
    } else if (type == "Supports") {
        fileName = ":/ui/deletesupportslayout.ui";
    } else if (type == "LineLoads") {
        fileName = ":/ui/DeleteLineLoadsLayout.ui";
    } else if (type == "NodalLoads") {
        fileName = ":/ui/DeleteNodalLoadsLayout.ui";
    }

    loadLayoutFromFile(fileName);
}

void DeleteObjectDialog::loadLayoutFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Cannot open file" << fileName;
        return;
    }

    currentOptionsWidget = uiLoader->load(&file, this);
    file.close();

    if (currentOptionsWidget) {
        ui->mainLayout->insertWidget(1, currentOptionsWidget); // Insert widget at position 1

        // Find and store pointers to the elements in the loaded layout
        pointToDeleteLineEdit = currentOptionsWidget->findChild<QLineEdit *>(
            "pointToDeleteLineEdit");
        lineToDeleteLineEdit = currentOptionsWidget->findChild<QLineEdit *>("lineToDeleteLineEdit");
        pointSupportToDeleteLineEdit = currentOptionsWidget->findChild<QLineEdit *>(
            "pointSupportToDeleteLineEdit");
        nodalLoadToDeleteLineEdit = currentOptionsWidget->findChild<QLineEdit *>(
            "nodalLoadToDeleteLineEdit");
        lineLoadToDeleteLineEdit = currentOptionsWidget->findChild<QLineEdit *>(
            "lineLoadToDeleteLineEdit");
    }
}

int DeleteObjectDialog::getPointId()
{
    if (pointToDeleteLineEdit) {
        return pointToDeleteLineEdit->text().toInt();
    } else {
        qWarning() << "pointToDeleteLineEdit not found!";
        return -1; // Return an invalid ID or handle the error appropriately
    }
}

int DeleteObjectDialog::getLineId()
{
    if (lineToDeleteLineEdit) {
        return lineToDeleteLineEdit->text().toInt();
    } else {
        qWarning() << "lineToDeleteLineEdit not found!";
        return -1; // Return an invalid ID or handle the error appropriately
    }
}

int DeleteObjectDialog::getSupportPointId()
{
    if (pointSupportToDeleteLineEdit) {
        return pointSupportToDeleteLineEdit->text().toInt();
    } else {
        qWarning() << "pointSupportToDeleteLineEdit not found!";
        return -1; // Return an invalid ID or handle the error appropriately
    }
}
int DeleteObjectDialog::getNodalLoadId()
{
    if (nodalLoadToDeleteLineEdit) {
        return nodalLoadToDeleteLineEdit->text().toInt();
    } else {
        qWarning() << "nodalLoadToDeleteLineEdit not found!";
        return -1; // Return an invalid ID or handle the error appropriately
    }
}

int DeleteObjectDialog::getLineLoadId()
{
    if (lineLoadToDeleteLineEdit) {
        return lineLoadToDeleteLineEdit->text().toInt();
    } else {
        qWarning() << "lineLoadToDeleteLineEdit not found!";
        return -1; // Return an invalid ID or handle the error appropriately
    }
}

void DeleteObjectDialog::updateLayoutForType(const QString &type)
{
    onObjectTypeChanged(type);
}


