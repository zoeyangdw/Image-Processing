#include "dlgcontraststretch.h"
#include "ui_dlgcontraststretch.h"

DlgContrastStretch::DlgContrastStretch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgContrastStretch)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(button_ok()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(button_cancel()));

}

DlgContrastStretch::~DlgContrastStretch()
{
    delete ui;
}

void DlgContrastStretch::button_ok()
{
    bool ok;
    double E = ui->E_edit->text().toDouble(&ok);
    if(!ok) E = 2.0;
    int m = ui->m_edit->text().toInt(&ok);
    if(!ok) m = 128;
    emit okClicked(m,E);
}

void DlgContrastStretch::button_cancel()
{

}
