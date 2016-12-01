#include "dlgedgeextraction.h"
#include "ui_dlgedgeextraction.h"

DlgEdgeExtraction::DlgEdgeExtraction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgEdgeExtraction)
{
    ui->setupUi(this);
    btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->btn_roberts,0);
    btnGroup->addButton(ui->btn_sobel,1);
    btnGroup->addButton(ui->btn_laps,2);
    ui->btn_roberts->setChecked(true);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(button_ok()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(button_cancel()));
}

DlgEdgeExtraction::~DlgEdgeExtraction()
{
    delete ui;
}

void DlgEdgeExtraction::button_ok()
{
    int nStyle = btnGroup->checkedId();
    emit okClicked(nStyle);
}

void DlgEdgeExtraction::button_cancel()
{

}
