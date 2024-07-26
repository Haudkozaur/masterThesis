#include "editobjectdialog.h"
#include <QFile>
#include "ui_editobjectdialog.h"

EditObjectDialog::EditObjectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditObjectDialog)
    , uiLoader(new QUiLoader(this)) // Inicjalizacja uiLoader
    , currentOptionsWidget(nullptr)
    , pointToEditLineEdit(nullptr)
    , lineToEditLineEdit(nullptr)
    , pointSupportToEditLineEdit(nullptr)
{
    ui->setupUi(this);
    connect(ui->comboBoxObjectType,
            &QComboBox::currentTextChanged,
            this,
            &EditObjectDialog::onObjectTypeChanged);

    // Initialize layout for the current type
    updateLayoutForType(ui->comboBoxObjectType->currentText());

    // Connect dialog buttons
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

EditObjectDialog::~EditObjectDialog()
{
    delete ui;
}

QString EditObjectDialog::getSelectedObjectType() const
{
    return ui->comboBoxObjectType->currentText();
}

int EditObjectDialog::getPointId()
{
    if (pointToEditLineEdit) {
        return pointToEditLineEdit->text().toInt();
    } else {
        qWarning() << "pointToEditLineEdit not found!";
        return -1;
    }
}

int EditObjectDialog::getLineId()
{
    if (lineToEditLineEdit) {
        return lineToEditLineEdit->text().toInt();
    } else {
        qWarning() << "lineToEditLineEdit not found!";
        return -1;
    }
}

int EditObjectDialog::getSupportPointId()
{
    if (pointSupportToEditLineEdit) {
        return pointSupportToEditLineEdit->text().toInt();
    } else {
        qWarning() << "pointSupportToEditLineEdit not found!";
        return -1;
    }
}

void EditObjectDialog::onObjectTypeChanged(const QString &type)
{
    if (currentOptionsWidget) {
        ui->mainLayout->removeWidget(currentOptionsWidget);
        delete currentOptionsWidget;
        currentOptionsWidget = nullptr;
    }

    QString fileName;
    if (type == "Points") {
        fileName = ":/ui/editpointslayout.ui";
    } else if (type == "Lines") {
        fileName = ":/ui/editlineslayout.ui";
    } else if (type == "Supports") {
        fileName = ":/ui/editsupportslayout.ui";
    }

    loadLayoutFromFile(fileName);
}

void EditObjectDialog::loadLayoutFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Cannot open file" << fileName;
        return;
    }

    currentOptionsWidget = uiLoader->load(&file, this);
    file.close();

    if (currentOptionsWidget) {
        ui->mainLayout->insertWidget(1, currentOptionsWidget);

        pointToEditLineEdit = currentOptionsWidget->findChild<QLineEdit *>("pointToDeleteLineEdit");
        lineToEditLineEdit = currentOptionsWidget->findChild<QLineEdit *>("lineToDeleteLineEdit");
        pointSupportToEditLineEdit = currentOptionsWidget->findChild<QLineEdit *>("pointSupportToDeleteLineEdit");
    }
}

void EditObjectDialog::updateLayoutForType(const QString &type)
{
    onObjectTypeChanged(type);
}

void EditObjectDialog::moveToBottomLeft()
{
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x, y);
    }
}
