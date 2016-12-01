#ifndef DLGSPACIALFILTER_H
#define DLGSPACIALFILTER_H

#include <QDialog>
#include <QButtonGroup>
#include <vector>
using std::vector;

namespace Ui {
class DlgSpacialFilter;
}

class DlgSpacialFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSpacialFilter(QWidget *parent = 0);
    ~DlgSpacialFilter();

private:
    Ui::DlgSpacialFilter *ui;
    QButtonGroup *btnGroup;
    void readOneRow(vector<double>& data, QString row, int& nCols, bool& ok);

private slots:
    void button_ok();
    void button_cancel();

signals:
    void okClicked(vector<double>data, int nCols, int nStyle);
};

#endif // DLGSPACIALFILTER_H
