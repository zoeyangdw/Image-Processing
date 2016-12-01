#include "dlgspatialfilter.h"
#include "ui_dlgspatialfilter.h"

#include <QRegExp>
#include <vector>
#include <iostream>
using namespace std;

DlgSpacialFilter::DlgSpacialFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSpacialFilter)
{
    ui->setupUi(this);
    btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->bt_dupli,2);
    btnGroup->addButton(ui->bt_mirror,1);
    btnGroup->addButton(ui->bt_zeropad,0);
    ui->bt_zeropad->setChecked(true);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(button_ok()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(button_cancel()));
}

DlgSpacialFilter::~DlgSpacialFilter()
{
    delete ui;
}

void DlgSpacialFilter::button_ok()
{
    vector<double> data;
    QString strMat = ui->textEdit->toPlainText().trimmed();
    strMat.replace("\n","");
    int nCols = -1;
    bool ok;
    QStringList rows = strMat.split(QRegExp("\\s*;"));
    for(int i = 0; i<rows.size();++i)
    {
        if (rows[i].trimmed().isEmpty())   continue;
        if (nCols<0)
        {
           readOneRow(data, rows[i].trimmed(), nCols, ok);
           if (!ok)
           {
               cout<<"Input is wrong"<<endl;
               return;
           }
        }
        else
        {
            int tmp;
            readOneRow(data, rows[i].trimmed(), tmp, ok);
            if (!ok)
            {
                cout<<"wrong"<<endl;
                return;
            }
            if (tmp != nCols)
            {
                cout<<"matric is wrong"<<endl;
                return;
            }
        }
    }
    int nStyle = 0;
    switch (btnGroup->checkedId())
    {
    case 0:
        nStyle = 0;
        break;
    case 1:
        nStyle = 1;
        break;
    case 2:
        nStyle = 2;
        break;
    }

    emit okClicked(data, nCols, nStyle);
}

void DlgSpacialFilter::readOneRow(vector<double>& data,
                                  QString row, int& nCols, bool& ok)
{
    nCols = 0;
    QStringList cols = row.split(QRegExp("\\s+|\\s*,"));
    for (int i=0; i<cols.size(); ++i)
    {
        if (cols[i].trimmed().isEmpty())   continue;
        double d = cols[i].toDouble(&ok);
        if (!ok)  return;
        data.push_back(d);
        nCols++;
    }
}

void DlgSpacialFilter::button_cancel()
{

}
