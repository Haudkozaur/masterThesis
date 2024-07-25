#include "DeleteObjectDialog.h"
#include "ui_DeleteObjectDialog.h"

DeleteObjectDialog::DeleteObjectDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::DeleteObjectDialog), currentOptionsWidget(nullptr), uiLoader(new QUiLoader(this))
{
    ui->setupUi(this);

    connect(ui->comboBoxObjectType, &QComboBox::currentTextChanged, this, &DeleteObjectDialog::onObjectTypeChanged);

    // Initialize layout for the current type
    updateLayoutForType(ui->comboBoxObjectType->currentText());

    // Connect dialog buttons
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

DeleteObjectDialog::~DeleteObjectDialog()
{
    delete ui;
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
        fileName = ":/ui/deletepointslayout.ui"; // Adjust path as needed
    } else if (type == "Lines") {
        fileName = ":/ui/deletelineslayout.ui"; // Adjust path as needed
    } else if (type == "Supports") {
        fileName = ":/ui/deletesupportslayout.ui"; // Adjust path as needed
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
    }
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

void DeleteObjectDialog::updateLayoutForType(const QString &type)
{
    // This method should update the layout based on the initial type
    // You can add any necessary logic here
    onObjectTypeChanged(type);
}
int DeleteObjectDialog::getPointId()
{
    return ui->pointToDeleteLineEdit->text().toInt();
}
