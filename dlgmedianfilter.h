#ifndef DLGMEDIANFILTER_H
#define DLGMEDIANFILTER_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class DlgMedianFilter;
}

class DlgMedianFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMedianFilter(QWidget *parent = 0);
    ~DlgMedianFilter();

private:
    Ui::DlgMedianFilter *ui;
    QButtonGroup *btnGroup;

private slots:
    void button_ok();
    void button_cancel();

signals:
    void okClicked(int nRows, int nCols, int nStyle);
};

#endif // DLGMEDIANFILTER_H
