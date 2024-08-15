#include "ViewDialog.h"
#include "ui_ViewDialog.h"

ViewDialog::ViewDialog(QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::ViewDialog)
{
    ui->setupUi(this);
}

ViewDialog::~ViewDialog()
{
    delete ui;
}

bool ViewDialog::getShowPoints()
{
    return ui->showPointsCheckBox->isChecked();
}

bool ViewDialog::getShowPointsLabels()
{
    return ui->showPointsIDsCheckBox->isChecked();
}

bool ViewDialog::getShowLines()
{
    return ui->showLinesCheckBox->isChecked();
}


bool ViewDialog::getShowLinesLabels()
{
    return ui->showLinesIDsCheckBox->isChecked();
}

bool ViewDialog::getShowSupports()
{
    return ui->showSupportsCheckBox->isChecked();

}

bool ViewDialog::getShowAssignedCS()
{
    return ui->showAssignedCSCheckBox->isChecked();
}

bool ViewDialog::getShowMesh()
{
    return ui->showMeshCheckBox->isChecked();
}

bool ViewDialog::getShowMeshNodesCoords()
{
    return ui->showMeshNodesCoordsCheckBox->isChecked();
}

bool ViewDialog::getShowNodalLoads()
{
    return ui->showNodalLoadsCheckBox->isChecked();
}

bool ViewDialog::getShowNodalLoadsLabels()
{
    return ui->showNodalLoadsLabelsCheckBox->isChecked();
}

bool ViewDialog::getShowLineLoads()
{
    return ui->showLineLoadsCheckBox->isChecked();
}

bool ViewDialog::getShowLineLoadsLabels()
{
    return ui->showLineLoadsLabelsCheckBox->isChecked();
}
