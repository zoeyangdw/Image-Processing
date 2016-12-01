#ifndef DLGTRANSFORM_H
#define DLGTRANSFORM_H

#include <QDialog>
#include"matrix_.h"
#include<QMatrix>

namespace Ui {
class DlgTransform;
}

class DlgTransform : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTransform(QWidget *parent = 0);
    ~DlgTransform();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DlgTransform *ui;
signals:
    void okClicked(const QMatrix& mat);
};

#endif // DLGTRANSFORM_H
