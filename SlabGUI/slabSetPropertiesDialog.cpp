#include "slabSetPropertiesDialog.h"
#include "ui_slabSetPropertiesDialog.h"

slabSetPropertiesDialog::slabSetPropertiesDialog(QWidget *parent)
    : SlabBaseDialog(parent)
    , ui(new Ui::slabSetPropertiesDialog)
    , materialComboBox(nullptr)
{
    ui->setupUi(this);
    // connect(ui->materialComboBox,
    //         QOverload<const QString &>::of(&QComboBox::currentTextChanged),
    //         this,
    //         &slabSetPropertiesDialog::onMaterialChanged);
    connect(ui->materialComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &slabSetPropertiesDialog::onMaterialIndexChanged);

    // connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    // connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void slabSetPropertiesDialog::onMaterialChanged(const QString &materialName)
{
    // Handle material change here
    qDebug() << "Material changed to:" << materialName;
    // You can perform any necessary actions when the material is changed
}
slabSetPropertiesDialog::~slabSetPropertiesDialog()
{
    delete ui;
}
void slabSetPropertiesDialog::updateMaterialsList()
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

void slabSetPropertiesDialog::setMaterials(const std::map<int, std::string> &materials)
{
    materialsMap = materials;
    updateMaterialsList();
}

int slabSetPropertiesDialog::getMaterialId() const
{
    return materialId;
}

int slabSetPropertiesDialog::getThickness() const
{
    return ui->thicknessLineEdit->text().toInt();
}

void slabSetPropertiesDialog::onMaterialIndexChanged(int index)
{
    materialId = index + 1;
}

