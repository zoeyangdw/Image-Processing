#ifndef DLGGUASSIANBLUR_H
#define DLGGUASSIANBLUR_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class DlgGuassianBlur;
}

class DlgGuassianBlur : public QDialog
{
    Q_OBJECT

public:
    explicit DlgGuassianBlur(QWidget *parent = 0);
    ~DlgGuassianBlur();

private:
    Ui::DlgGuassianBlur *ui;
    QButtonGroup *btnGroup;

private slots:
    void button_ok();
    void button_cancel();

signals:
    void okClicked(int nStyle);
};

#endif // DLGGUASSIANBLUR_H
