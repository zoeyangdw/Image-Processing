#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDir>
#include <QRect>
#include <QImage>
#include <QPointF>
#include "mypixmapitem.h"
#include "imageprocesser.h"
#include "dlgcontraststretch.h"
#include "dlgspatialfilter.h"
#include "dlgmedianfilter.h"
#include "dlgtransform.h"
#include "dlgguassianblur.h"
#include "dlgedgeextraction.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void show_img(QImage img);
    void setcurrent_state();


private:
    QImage ret_img;
    Ui::MainWindow *ui;
    QFileDialog *fd;
    QGraphicsScene *gs;
    MyPixmapItem *gpi;
    QDir myDir;
    QStringList file_list;
    QString currentFilePath;
    QRectF scene_rect;
    ImageProcesser *ip;
    DlgContrastStretch *dlgContrastStretch;
    DlgSpacialFilter *dlgSpatialFilter;
    DlgMedianFilter *dlgMedianFilter;
    DlgTransform *dlgTransform;
    DlgGuassianBlur *dlgGuassianBlur;
    DlgEdgeExtraction *dlgEdgeExtraction;

public slots:
    void open();
    void reset();
    void save();
    void save_as();
    void last();
    void next();
    void rgb2gray();
    void rgb2bw();
    void reverse();
    void log_trans();
    void contrast_open();
    void on_dlgContrastStretch_okClicked(int m, double E);
    void histeq();
    void exact_histeq();
    void spatial_filter();
    void on_dlgSpatialFilter_okClicked(vector<double>data, int nCols, int nStyle);
    void median_filter();
    void on_dlgMedianFilter_okClicked(int nRows, int nCols, int nStyle);
    void fft2();
    void ifft2();

    void transform();
    void on_dlgTransform_okClicked(const QMatrix& mat);
    //形态学处理
    void erosion();
    void dilation();
    void opening();
    void closing();
    //高斯模糊
    void gs_blur();
    void on_dlaGuassianBlur_onClicked(int nStyle);
    //边缘提取
    void edge_extraction();
    void on_dlgEdgeExtraction_onClicked(int nStryle);
    //浮雕
    void emboss();
    //油画
    void oilPainting();
};

#endif // MAINWINDOW_H
