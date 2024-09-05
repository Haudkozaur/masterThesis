#include "AddSurfaceDialog.h"
#include "ui_AddSurfaceDialog.h"

#include <QFile>

AddSurfaceDialog::AddSurfaceDialog(QWidget *parent)
    : SlabBaseDialog(parent)
    , ui(new Ui::AddSurfaceDialog)
    , uiLoader(new QUiLoader(this))
    , currentOptionsWidget(nullptr)
    , diameterLineEdit(nullptr)
    , centreX1LineEdit(nullptr)
    , centreZ1LineEdit(nullptr)
    , X1LineEdit(nullptr)
    , Z1LineEdit(nullptr)
    , X2LineEdit(nullptr)
    , Z2LineEdit(nullptr)
    , triX1LineEdit(nullptr)
    , triZ1LineEdit(nullptr)
    , triX2LineEdit(nullptr)
    , triZ2LineEdit(nullptr)
    , triX3LineEdit(nullptr)
    , triZ3LineEdit(nullptr)
    , isOpening(false)

{
    ui->setupUi(this);
    connect(ui->comboBoxObjectType, &QComboBox::currentTextChanged, this, &AddSurfaceDialog::onObjectTypeChanged);


    // Connect dialog buttons
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

AddSurfaceDialog::~AddSurfaceDialog()
{
    delete ui;
}

int AddSurfaceDialog::getDiameter() const
{
    if (diameterLineEdit) {
        return diameterLineEdit->text().toInt();
    } else {
        qWarning() << "diameterLineEdit not found!";
        return -1;
    }
}

int AddSurfaceDialog::getCentreX1() const
{
    if (centreX1LineEdit) {
        return centreX1LineEdit->text().toInt();
    } else {
        qWarning() << "centreX1LineEdit not found!";
        return -1;
    }
}

int AddSurfaceDialog::getCentreZ1() const
{
    if (centreZ1LineEdit) {
        return centreZ1LineEdit->text().toInt();
    } else {
        qWarning() << "centreZ1LineEdit not found!";
        return -1;
    }
}

int AddSurfaceDialog::getX1() const
{
    if (X1LineEdit) {
        return X1LineEdit->text().toInt();
    } else {
        qWarning() << "X1LineEdit not found!";
        return -1;
    }
}

int AddSurfaceDialog::getZ1() const
{
    if (Z1LineEdit) {
        return Z1LineEdit->text().toInt();
    } else {
        qWarning() << "Z1LineEdit not found!";
        return -1;
    }
}

int AddSurfaceDialog::getX2() const
{
    if (X2LineEdit) {
        return X2LineEdit->text().toInt();
    } else {
        qWarning() << "X2LineEdit not found!";
        return -1;
    }
}

int AddSurfaceDialog::getZ2() const
{
    if (Z2LineEdit) {
        return Z2LineEdit->text().toInt();
    } else {
        qWarning() << "Z2LineEdit not found!";
        return -1;
    }
}

int AddSurfaceDialog::getTriX1() const
{
    if (triX1LineEdit) {
        return triX1LineEdit->text().toInt();
    } else {
        qWarning() << "triX1LineEdit not found!";
        return -1;
    }
}

int AddSurfaceDialog::getTriZ1() const
{
    if (triZ1LineEdit) {
        return triZ1LineEdit->text().toInt();
    } else {
        qWarning() << "triZ1LineEdit not found!";
        return -1;
    }
}

int AddSurfaceDialog::getTriX2() const
{
    if (triX2LineEdit) {
        return triX2LineEdit->text().toInt();
    } else {
        qWarning() << "triX2LineEdit not found!";
        return -1;
    }
}


int AddSurfaceDialog::getTriZ2() const
{
    if (triZ2LineEdit) {
        return triZ2LineEdit->text().toInt();
    } else {
        qWarning() << "triZ2LineEdit not found!";
        return -1;
    }
}

int AddSurfaceDialog::getTriX3() const
{
    if (triX3LineEdit) {
        return triX3LineEdit->text().toInt();
    } else {
        qWarning() << "triX3LineEdit not found!";
        return -1;
    }
}

int AddSurfaceDialog::getTriZ3() const
{
    if (triZ3LineEdit) {
        return triZ3LineEdit->text().toInt();
    } else {
        qWarning() << "triZ3LineEdit not found!";
        return -1;
    }
}

void AddSurfaceDialog::initializeWithType(const QString &selectedType)
{
    if (!selectedType.isEmpty()) {
        ui->comboBoxObjectType->setCurrentText(selectedType);
        updateLayoutForType(selectedType); // Ensure layout is updated
    } else {
        updateLayoutForType(ui->comboBoxObjectType->currentText());
    }
}
QString AddSurfaceDialog::getSelectedObjectType() const
{
    return ui->comboBoxObjectType->currentText();
}

void AddSurfaceDialog::onObjectTypeChanged(const QString &type)
{
    if (currentOptionsWidget) {
        ui->mainLayout->removeWidget(currentOptionsWidget);
        delete currentOptionsWidget;
        currentOptionsWidget = nullptr;
    }

    QString fileName;
    if (type == "Rectangle") {
        fileName = ":/ui/rectangularSurface.ui";
    } else if (type == "Circle") {
        fileName = ":/ui/circularSurface.ui";
    } else if (type == "Triangle") {
        fileName = ":/ui/triangularSurface.ui";
    }


    loadLayoutFromFile(fileName);
}
void AddSurfaceDialog::loadLayoutFromFile(const QString &fileName)
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

        X1LineEdit = currentOptionsWidget->findChild<QLineEdit *>("X1LineEdit");
        X2LineEdit = currentOptionsWidget->findChild<QLineEdit *>("X2LineEdit");
        Z1LineEdit = currentOptionsWidget->findChild<QLineEdit *>("Z1LineEdit");
        Z2LineEdit = currentOptionsWidget->findChild<QLineEdit *>("Z2LineEdit");

        centreX1LineEdit = currentOptionsWidget->findChild<QLineEdit *>("centreX1LineEdit");
        centreZ1LineEdit = currentOptionsWidget->findChild<QLineEdit *>("centreZ1LineEdit");
        diameterLineEdit = currentOptionsWidget->findChild<QLineEdit *>("diameterLineEdit");

        triX1LineEdit = currentOptionsWidget->findChild<QLineEdit *>("triX1LineEdit");
        triX2LineEdit = currentOptionsWidget->findChild<QLineEdit *>("triX2LineEdit");
        triX3LineEdit = currentOptionsWidget->findChild<QLineEdit *>("triX3LineEdit");
        triZ1LineEdit = currentOptionsWidget->findChild<QLineEdit *>("triZ1LineEdit");
        triZ2LineEdit = currentOptionsWidget->findChild<QLineEdit *>("triZ2LineEdit");
        triZ3LineEdit = currentOptionsWidget->findChild<QLineEdit *>("triZ3LineEdit");

    }
}
void AddSurfaceDialog::updateLayoutForType(const QString &type)
{
    onObjectTypeChanged(type);
}



void AddSurfaceDialog::on_openingCheckBox_toggled(bool checked)
{
    isOpening = true;
}

bool AddSurfaceDialog::getIsOpening()
{
    return isOpening;
}

