// EditObjectDialog.cpp
#include "editobjectdialog.h"
#include "ui_editobjectdialog.h"
#include <QFile>
#include <QDebug>
#include <QUiLoader>

EditObjectDialog::EditObjectDialog(QWidget *parent)
    : BaseDialog(parent)
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
    } else if (type == "LineLoads") {
        fileName = ":/ui/EditLineLoadsLayout.ui";
    } else if (type == "NodalLoads") {
        fileName = ":/ui/EditNodalLoadsLayout.ui";
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

        pointLoadToEditLineEdit = currentOptionsWidget->findChild<QLineEdit *>("pointLoadToEditLineEdit");
        newFzNodalLineEdit = currentOptionsWidget->findChild<QLineEdit *>("newFzNodalLineEdit");
        newFxNodalLineEdit = currentOptionsWidget->findChild<QLineEdit *>("newFxNodalLineEdit");
        newMyNodalLineEdit = currentOptionsWidget->findChild<QLineEdit *>("newMyNodalLineEdit");


        lineLineLoadtToEditLineEdit = currentOptionsWidget->findChild<QLineEdit *>("lineLineLoadtToEditLineEdit");
        newFxLineEdit = currentOptionsWidget->findChild<QLineEdit *>("newFxLineEdit");
        newFzLineEdit = currentOptionsWidget->findChild<QLineEdit *>("newFzLineEdit");
    }
}

void EditObjectDialog::updateLayoutForType(const QString &type)
{
    onObjectTypeChanged(type);
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

int EditObjectDialog::getNodalLoadPointId()
{
    if (pointLoadToEditLineEdit) {
        return pointLoadToEditLineEdit->text().toInt();
    } else {
        qWarning() << "pointLoadToEditLineEdit not found!";
        return -1;
    }
}


double EditObjectDialog::getNewFz()
{
    if (newFzNodalLineEdit) {
        return newFzNodalLineEdit->text().toDouble();
    } else {
        qWarning() << "newFzNodalLineEdit not found!";
        return -1.0;
    }
}

double EditObjectDialog::getNewFx()
{
    if (newFxNodalLineEdit) {
        return newFxNodalLineEdit->text().toDouble();
    } else {
        qWarning() << "newFxNodalLineEdit not found!";
        return -1.0;
    }
}


double EditObjectDialog::getNewMy()
{
    if (newMyNodalLineEdit) {
        return newMyNodalLineEdit->text().toDouble();
    } else {
        qWarning() << "newMyNodalLineEdit not found!";
        return -1.0;
    }
}


int EditObjectDialog::getLineLoadId()
{
    if (lineLineLoadtToEditLineEdit) {
        return lineLineLoadtToEditLineEdit->text().toInt();
    } else {
        qWarning() << "lineLineLoadtToEditLineEdit not found!";
        return -1;
    }
}


double EditObjectDialog::getNewFxLineLoad()
{
    if (newFxLineEdit) {
        return newFxLineEdit->text().toDouble();
    } else {
        qWarning() << "newFxLineEdit not found!";
        return -1.0;
    }
}


double EditObjectDialog::getNewFzLineLoad()
{
    if (newFzLineEdit) {
        return newFzLineEdit->text().toDouble();
    } else {
        qWarning() << "newFzLineEdit not found!";
        return -1.0;
    }
}
