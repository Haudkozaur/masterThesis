// EditObjectDialog.cpp
#include "editobjectdialog.h"
#include "ui_editobjectdialog.h"
#include <QFile>
#include <QDebug>
#include <QUiLoader>

EditObjectDialog::EditObjectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditObjectDialog)
    , uiLoader(new QUiLoader(this))
    , currentOptionsWidget(nullptr)
    , pointToEditLineEdit(nullptr)
    , xEditLineEdit(nullptr)
    , zEditLineEdit(nullptr)
    , lineToEditLineEdit(nullptr)
    , fEditLineEdit(nullptr)
    , eEditLineEdit(nullptr)
    , pointSupportToEditLineEdit(nullptr)
    , Tz(nullptr)
    , Tx(nullptr)
    , Ry(nullptr)

{
    ui->setupUi(this);
    connect(ui->comboBoxObjectType, &QComboBox::currentTextChanged, this, &EditObjectDialog::onObjectTypeChanged);

    // Connect dialog buttons
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void EditObjectDialog::initializeWithType(const QString &selectedType)
{
    if (!selectedType.isEmpty()) {
        ui->comboBoxObjectType->setCurrentText(selectedType);
        updateLayoutForType(selectedType); // Ensure layout is updated
    } else {
        updateLayoutForType(ui->comboBoxObjectType->currentText());
    }
}

EditObjectDialog::~EditObjectDialog()
{
    delete ui;
}

QString EditObjectDialog::getSelectedObjectType() const
{
    return ui->comboBoxObjectType->currentText();
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

        pointToEditLineEdit = currentOptionsWidget->findChild<QLineEdit *>("pointToEditLineEdit");
        xEditLineEdit = currentOptionsWidget->findChild<QLineEdit *>("xEditLineEdit");
        zEditLineEdit = currentOptionsWidget->findChild<QLineEdit *>("zEditLineEdit");

        lineToEditLineEdit = currentOptionsWidget->findChild<QLineEdit *>("lineToEditLineEdit");
        fEditLineEdit = currentOptionsWidget->findChild<QLineEdit *>("fEditLineEdit");
        eEditLineEdit = currentOptionsWidget->findChild<QLineEdit *>("eEditLineEdit");

        pointSupportToEditLineEdit = currentOptionsWidget->findChild<QLineEdit *>("pointSupportToEditLineEdit");
        Tz = currentOptionsWidget->findChild<QCheckBox *>("Tz");
        Tx = currentOptionsWidget->findChild<QCheckBox *>("Tx");
        Ry = currentOptionsWidget->findChild<QCheckBox *>("Ry");
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
        move(x + 10, y);
    }
}

int EditObjectDialog::getPointToEditId()
{
    if (pointToEditLineEdit) {
        return pointToEditLineEdit->text().toInt();
    } else {
        qWarning() << "pointToEditLineEdit not found!";
        return -1;
    }
}

int EditObjectDialog::getNewXCord()
{
    if (xEditLineEdit) {
        return xEditLineEdit->text().toInt();
    } else {
        qWarning() << "xEditLineEdit not found!";
        return -1;
    }
}

int EditObjectDialog::getNewZCord()
{
    if (zEditLineEdit) {
        return zEditLineEdit->text().toInt();
    } else {
        qWarning() << "zEditLineEdit not found!";
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

int EditObjectDialog::getNewStartPoint()
{
    if (fEditLineEdit) {
        return fEditLineEdit->text().toInt();
    } else {
        qWarning() << "fEditLineEdit not found!";
        return -1; // Return an invalid ID or handle the error appropriately
    }
}

int EditObjectDialog::getNewEndPoint()
{
    if (eEditLineEdit) {
        return eEditLineEdit->text().toInt();
    } else {
        qWarning() << "endPointLineEdit not found!";
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

bool EditObjectDialog::getTz()
{
    if (Tz) {
        return Tz->isChecked();
    } else {
        qWarning() << "Tz not found!";
        return false;
    }
}

bool EditObjectDialog::getTx()
{
    if (Tx) {
        return Tx->isChecked();
    } else {
        qWarning() << "Tx not found!";
        return false;
    }
}

bool EditObjectDialog::getRy()
{
    if (Ry) {
        return Ry->isChecked();
    } else {
        qWarning() << "Ry not found!";
        return false;
    }
}
