#ifndef DLGCONTRASTSTRETCH_H
#define DLGCONTRASTSTRETCH_H

#include <QDialog>

namespace Ui {
class DlgContrastStretch;
}

class DlgContrastStretch : public QDialog
{
    Q_OBJECT

public:
    explicit DlgContrastStretch(QWidget *parent = 0);
    ~DlgContrastStretch();

private:
    Ui::DlgContrastStretch *ui;

private slots:
    void button_ok();
    void button_cancel();

signals:
    void okClicked(int m,double E);

};

#endif // DLGCONTRASTSTRETCH_H
