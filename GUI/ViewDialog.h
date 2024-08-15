#ifndef VIEWDIALOG_H
#define VIEWDIALOG_H

#include "basedialog.h"
#include <QDialog>
#include <QCheckBox>

namespace Ui {
class ViewDialog;
}

class ViewDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit ViewDialog(QWidget *parent = nullptr);
    ~ViewDialog();

    bool getShowPoints();
    bool getShowPointsLabels();
    bool getShowLines();
    bool getShowLinesLabels();
    bool getShowSupports();
    bool getShowAssignedCS();
    bool getShowMesh();
    bool getShowMeshNodesCoords();
    bool getShowNodalLoads();
    bool getShowNodalLoadsLabels();
    bool getShowLineLoads();
    bool getShowLineLoadsLabels();

private:
    Ui::ViewDialog *ui;
};

#endif // VIEWDIALOG_H
