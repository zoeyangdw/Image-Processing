#include "dlgguassianblur.h"
#include "ui_dlgguassianblur.h"

DlgGuassianBlur::DlgGuassianBlur(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgGuassianBlur)
{
    ui->setupUi(this);
    btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->btn_zero,0);
    btnGroup->addButton(ui->btn_mirror,1);
    btnGroup->addButton(ui->btn_dupli,2);
    ui->btn_zero->setChecked(true);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(button_ok()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(button_cancel()));
}

DlgGuassianBlur::~DlgGuassianBlur()
{
    delete ui;
}

void DlgGuassianBlur::button_ok()
{
    int nStyle = btnGroup->checkedId();
    emit okClicked(nStyle);
}

void DlgGuassianBlur::button_cancel()
{

}
