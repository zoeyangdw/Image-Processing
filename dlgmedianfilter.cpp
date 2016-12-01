#include "dlgmedianfilter.h"
#include "ui_dlgmedianfilter.h"

DlgMedianFilter::DlgMedianFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMedianFilter)
{
    ui->setupUi(this);
    btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->bt_zero,0);
    btnGroup->addButton(ui->bt_mirror,1);
    btnGroup->addButton(ui->bt_dupli,2);
    ui->bt_zero->setChecked(true);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(button_ok()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(button_cancel()));
}

DlgMedianFilter::~DlgMedianFilter()
{
    delete ui;
}

void DlgMedianFilter::button_ok()
{
    bool ok1,ok2;
    int nRows = ui->edit_height->text().toInt(&ok1);
    int nCols = ui->edit_width->text().toInt(&ok2);
    if (!ok1 && !ok2)
        return;
    int nStyle = btnGroup->checkedId();
    emit okClicked(nRows, nCols, nStyle);
}

void DlgMedianFilter::button_cancel()
{

}


