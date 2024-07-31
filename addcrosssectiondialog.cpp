#include "addcrosssectiondialog.h"
#include <QDebug>
#include <QFile>
#include <QUiLoader>
#include <QVariant>
#include <QWidget>
#include "ui_addcrosssectiondialog.h"

AddCrossSectionDialog::AddCrossSectionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddCrossSectionDialog)
    , uiLoader(new QUiLoader(this))
    , currentOptionsWidget(nullptr)
    , csInnertiaLineEdit(nullptr)
    , csNameLineEdit(nullptr)
    , csAreaLineEdit(nullptr)
    , csHeightLineEdit(nullptr)
    , csWidthtLineEdit(nullptr)
    , csRNameLineEdit(nullptr)
    , csRadiusLineEdit(nullptr)
    , csCNameLineEdit(nullptr)
    , materialComboBox(nullptr)
{
    ui->setupUi(this);

    connect(ui->comboBoxObjectType,
            &QComboBox::currentTextChanged,
            this,
            &AddCrossSectionDialog::onObjectTypeChanged);

    connect(ui->materialComboBox,
            QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            this,
            &AddCrossSectionDialog::onMaterialChanged);
    connect(ui->materialComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &AddCrossSectionDialog::onMaterialIndexChanged);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
void AddCrossSectionDialog::onMaterialChanged(const QString &materialName)
{
    // Handle material change here
    qDebug() << "Material changed to:" << materialName;
    // You can perform any necessary actions when the material is changed
}
AddCrossSectionDialog::~AddCrossSectionDialog()
{
    delete ui;
}
void AddCrossSectionDialog::initializeWithType(const QString &selectedType)
{
    if (!selectedType.isEmpty()) {
        ui->comboBoxObjectType->setCurrentText(selectedType);
        updateLayoutForType(selectedType); // Ensure layout is updated
    } else {
        updateLayoutForType(ui->comboBoxObjectType->currentText());
    }
}
QString AddCrossSectionDialog::getSelectedObjectType() const
{
    return ui->comboBoxObjectType->currentText();
}

void AddCrossSectionDialog::onObjectTypeChanged(const QString &type)
{
    if (currentOptionsWidget) {
        ui->mainLayout->removeWidget(currentOptionsWidget);
        delete currentOptionsWidget;
        currentOptionsWidget = nullptr;
    }

    QString fileName;
    if (type == "General") {
        fileName = ":/ui/generalCrossSectionLayout.ui";
    } else if (type == "Rectangular") {
        fileName = ":/ui/rectangularCrossSectionLayout.ui";
    } else if (type == "Circular") {
        fileName = ":/ui/circularCrossSectionLayout.ui";
    }

    loadLayoutFromFile(fileName);
}

void AddCrossSectionDialog::loadLayoutFromFile(const QString &fileName)
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

        csInnertiaLineEdit = currentOptionsWidget->findChild<QLineEdit *>("csInnertiaLineEdit");
        csNameLineEdit = currentOptionsWidget->findChild<QLineEdit *>("csNameLineEdit");
        csAreaLineEdit = currentOptionsWidget->findChild<QLineEdit *>("csAreaLineEdit");

        csHeightLineEdit = currentOptionsWidget->findChild<QLineEdit *>("csHeightLineEdit");
        csWidthtLineEdit = currentOptionsWidget->findChild<QLineEdit *>("csWidthtLineEdit");
        csRNameLineEdit = currentOptionsWidget->findChild<QLineEdit *>("csRNameLineEdit");

        csRadiusLineEdit = currentOptionsWidget->findChild<QLineEdit *>("csRadiusLineEdit");
        csCNameLineEdit = currentOptionsWidget->findChild<QLineEdit *>("csCNameLineEdit");
    }
}

void AddCrossSectionDialog::updateLayoutForType(const QString &type)
{
    onObjectTypeChanged(type);
}

void AddCrossSectionDialog::moveToBottomLeft()
{
    if (parentWidget()) {
        auto host = parentWidget();
        auto hostRect = host->geometry();
        int x = hostRect.left();
        int y = hostRect.top() + 250;
        move(x + 5, y);
    }
}

double AddCrossSectionDialog::getInnertia()
{
    if (csInnertiaLineEdit) {
        return csInnertiaLineEdit->text().toDouble();
    } else {
        qWarning() << "csInnertiaLineEdit not found!";
        return -1;
    }
}

std::string AddCrossSectionDialog::getName()
{
    if (csNameLineEdit) {
        return csNameLineEdit->text().toStdString();
    } else {
        qWarning() << "csNameLineEdit not found!";
        return "";
    }
}

double AddCrossSectionDialog::getArea()
{
    if (csAreaLineEdit) {
        return csAreaLineEdit->text().toDouble();
    } else {
        qWarning() << "csAreaLineEdit not found!";
        return -1;
    }
}

int AddCrossSectionDialog::getHeight()
{
    if (csHeightLineEdit) {
        return csHeightLineEdit->text().toInt();
    } else {
        qWarning() << "csHeightLineEdit not found!";
        return -1;
    }
}

int AddCrossSectionDialog::getWidth()
{
    if (csWidthtLineEdit) {
        return csWidthtLineEdit->text().toInt();
    } else {
        qWarning() << "csWidthtLineEdit not found!";
        return -1;
    }
}

int AddCrossSectionDialog::getRadius()
{
    if (csRadiusLineEdit) {
        return csRadiusLineEdit->text().toInt();
    } else {
        qWarning() << "csRadiusLineEdit not found!";
        return -1;
    }
}

std::string AddCrossSectionDialog::getNameForRect()
{
    if (csRNameLineEdit) {
        return csRNameLineEdit->text().toStdString();
    } else {
        qWarning() << "csRNameLineEdit not found!";
        return "";
    }
}

std::string AddCrossSectionDialog::getNameForCirc()
{
    if (csCNameLineEdit) {
        return csCNameLineEdit->text().toStdString();
    } else {
        qWarning() << "csCNameLineEdit not found!";
        return "";
    }
}

void AddCrossSectionDialog::updateMaterialsList()
{
    if (ui->materialComboBox) {
        ui->materialComboBox->clear();
        for (const auto &material : materialsMap) {
            ui->materialComboBox->addItem(QString::fromStdString(material.second),
                                          QVariant(material.first));
        }
    } else {
        qWarning() << "materialComboBox not found!";
    }
}

void AddCrossSectionDialog::setMaterials(const std::map<int, std::string> &materials)
{
    materialsMap = materials;
    updateMaterialsList();
}

int AddCrossSectionDialog::getMaterialId() const
{
    return materialId;
}

void AddCrossSectionDialog::onMaterialIndexChanged(int index)
{
    materialId = index + 1;
    // You can perform any necessary actions with the selected material here
}
