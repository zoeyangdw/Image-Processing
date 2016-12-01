#include "dlgtransform.h"
#include "ui_dlgtransform.h"

DlgTransform::DlgTransform(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgTransform)
{
    ui->setupUi(this);
}

DlgTransform::~DlgTransform()
{
    delete ui;
}

void DlgTransform::on_buttonBox_accepted()
{
    QMatrix tform;
    bool ok;
    qreal m11, m12, m21, m22, dx, dy;
    m11 = ui->lineEdit11->text().toDouble(&ok);
    if (!ok)    m11 = 1;
    m12 = ui->lineEdit12->text().toDouble(&ok);
    if (!ok)    m12 = 0;
    m21 = ui->lineEdit21->text().toDouble(&ok);
    if (!ok) m21 = 0;
    m22 = ui->lineEdit22->text().toDouble(&ok);
    if (!ok) m22 = 1;
    dx = ui->lineEdit31->text().toDouble(&ok);
    if (!ok) dx = 0;
    dy = ui->lineEdit32->text().toDouble(&ok);
    if (!ok) dy = 0;
    tform.setMatrix(m11, m12, m21, m22, dx, dy);
    emit okClicked(tform);
}
