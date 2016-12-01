#ifndef DLGEDGEEXTRACTION_H
#define DLGEDGEEXTRACTION_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class DlgEdgeExtraction;
}

class DlgEdgeExtraction : public QDialog
{
    Q_OBJECT

public:
    explicit DlgEdgeExtraction(QWidget *parent = 0);
    ~DlgEdgeExtraction();

private:
    Ui::DlgEdgeExtraction *ui;
    QButtonGroup *btnGroup;

private slots:
    void button_ok();
    void button_cancel();

signals:
    void okClicked(int nStyle);
};

#endif // DLGEDGEEXTRACTION_H
