#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>


vector<complex<double> >fft_res;
#include<iostream>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),fd(NULL),gpi(NULL),gs(NULL),ip(NULL),ret_img(NULL),
    dlgContrastStretch(new DlgContrastStretch(this)),dlgSpatialFilter(new DlgSpacialFilter(this)),
    dlgMedianFilter(new DlgMedianFilter(this)),dlgTransform(new DlgTransform(this)),
    dlgGuassianBlur(new DlgGuassianBlur(this)),dlgEdgeExtraction(new DlgEdgeExtraction(this))
{
    ui->setupUi(this);
    fd = new QFileDialog(this);
    fd->setNameFilter("Image(*.bmp *.jpg *.png)");
    fd->setFileMode(QFileDialog::ExistingFile);//设置模式,文件模式

    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->actionReset,SIGNAL(triggered()),this,SLOT(reset()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->actionSave_as,SIGNAL(triggered()),this,SLOT(save_as()));

    connect(ui->actionLast,SIGNAL(triggered()),this,SLOT(last()));
    connect(ui->actionNext,SIGNAL(triggered()),this,SLOT(next()));
    connect(ui->actionGray,SIGNAL(triggered()),this,SLOT(rgb2gray()));
    connect(ui->btn_bw,SIGNAL(clicked()),this,SLOT(rgb2bw()));
    connect(ui->actionReverse,SIGNAL(triggered()),this,SLOT(reverse()));
    connect(ui->btn_logtrans,SIGNAL(clicked()),this,SLOT(log_trans()));

    connect(ui->actionContrast_stretch,SIGNAL(triggered()),this,SLOT(contrast_open()));
    connect(dlgContrastStretch,SIGNAL(okClicked(int,double)),this,SLOT(on_dlgContrastStretch_okClicked(int,double)));

    connect(ui->actionHisteq,SIGNAL(triggered()),this,SLOT(histeq()));
    connect(ui->actionExact_Histeq,SIGNAL(triggered()),this,SLOT(exact_histeq()));

    connect(ui->actionSpatial_Filter,SIGNAL(triggered()),this,SLOT(spatial_filter()));
    connect(dlgSpatialFilter,SIGNAL(okClicked(vector<double>,int,int)),this,SLOT(on_dlgSpatialFilter_okClicked(vector<double>,int,int)));

    connect(ui->actionMedian_Filter,SIGNAL(triggered()),this,SLOT(median_filter()));
    connect(dlgMedianFilter,SIGNAL(okClicked(int,int,int)),this,SLOT(on_dlgMedianFilter_okClicked(int,int,int)));

    connect(ui->actionFft2,SIGNAL(triggered()),this, SLOT(fft2()));
    connect(ui->actionIfft2,SIGNAL(triggered()),this,SLOT(ifft2()));

    connect(ui->actionTransform,SIGNAL(triggered()),this,SLOT(transform()));
    connect(dlgTransform,SIGNAL(okClicked(QMatrix)),this, SLOT(on_dlgTransform_okClicked(QMatrix)));
    //形态学槽函数
    connect(ui->actionErosion,SIGNAL(triggered()),this,SLOT(erosion()));
    connect(ui->actionDilation,SIGNAL(triggered()),this,SLOT(dilation()));
    connect(ui->actionOpening,SIGNAL(triggered()),this,SLOT(opening()));
    connect(ui->actionClosing,SIGNAL(triggered()),this,SLOT(closing()));
    //高斯模糊槽函数
    connect(ui->actionGaussianBlur,SIGNAL(triggered()),this,SLOT(gs_blur()));
    connect(dlgGuassianBlur,SIGNAL(okClicked(int)),this,SLOT(on_dlaGuassianBlur_onClicked(int)));
    //边缘提取
    connect(ui->actionEdgeExtraction,SIGNAL(triggered()),this,SLOT(edge_extraction()));
    connect(dlgEdgeExtraction,SIGNAL(okClicked(int)),this,SLOT(on_dlgEdgeExtraction_onClicked(int)));
    //浮雕
    connect(ui->actionEmboss,SIGNAL(triggered()),this,SLOT(emboss()));
    //油画
    connect(ui->actionOilPainting,SIGNAL(triggered()),this,SLOT(oilPainting()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete fd;
    delete dlgContrastStretch;
    delete dlgSpatialFilter;
    delete dlgMedianFilter;
    delete dlgEdgeExtraction;
    delete dlgGuassianBlur;
    delete dlgTransform;
}

void MainWindow::open()
{
    if(fd->exec())
    {
        QStringList files = fd->selectedFiles();
        currentFilePath = files[0];
        ret_img = QImage(currentFilePath);
        show_img(ret_img);
        myDir = fd->directory();
        QStringList filters;
        filters << QString("*.bmp") << QString("*.jpg") << QString("*.png");
        myDir.setNameFilters(filters);
        QFileInfoList list = myDir.entryInfoList();
        for(int i = 0;i<list.size();i++)
        {
            QFileInfo fileInfo = list.at(i);
            file_list.append(fileInfo.filePath());
        }
    }   
}

void MainWindow::show_img(QImage img)
{
    if(gpi)
    {
        delete gpi;
        gpi = NULL;
    }
    gpi = new MyPixmapItem();
    gpi->setPixmap(QPixmap::fromImage(img));
    if(gs)
    {
        delete gs;
        gs = NULL;
    }
    gs = new QGraphicsScene(this);
    if(ip)
    {
        delete ip;
        ip = NULL;
    }
    ip = new ImageProcesser(img);
    gs->addItem(gpi);
    ui->graphicsView->setScene(gs);
    reset();
}

void MainWindow::reset()
{
    QRectF item_rect = gpi->boundingRect();
    gpi->reset();
    gs->setSceneRect(0,0,item_rect.width(),item_rect.height());
    ui->graphicsView->fitInView(item_rect,Qt::KeepAspectRatio);
    ui->graphicsView->centerOn(gpi);
}

void MainWindow::save()
{
    if (ip->getImage().isNull())
    {
        ip->getImage().save(currentFilePath);
    }
}

void MainWindow::save_as()
{
    QString saveFile = QFileDialog::getSaveFileName(this,currentFilePath,"","Image(*.bmp *.jpg *.png)");
    ip->getImage().save(saveFile);
}

void MainWindow::last()
{
    gpi->reset();
    int index = file_list.indexOf(currentFilePath);
    if (index == 0)
    {
        currentFilePath = file_list.last();
    }
    else
    {
        currentFilePath = file_list.at(index-1);
    }
    show_img(QImage(currentFilePath));
}

void MainWindow::next()
{
    gpi->reset();
    int index = file_list.indexOf(currentFilePath);
    if (index == (file_list.size()-1))
    {
        currentFilePath = file_list.first();
    }
    else
    {
        currentFilePath = file_list.at(index+1);
    }
    show_img(QImage(currentFilePath));
}

void MainWindow::rgb2gray()
{ 
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->rgb2gray());
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::rgb2bw()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    QString t = ui->bw_t->text();
    bool ok;
    int threshold = t.toInt(&ok);
    if(ok)show_img(ip->rgb2bw(threshold));
    else
    {
        threshold = 128;
        show_img(ip->rgb2bw(threshold));
    }
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::reverse()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->reverse());
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}


void MainWindow::log_trans()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    QString m = ui->log_t->text();
    bool ok;
    int log_c = m.toInt(&ok);
    if(ok)
    {
        show_img(ip->log_trans(log_c));
    }
    else
    {
        log_c = 2;
        show_img(ip->log_trans(log_c));
    }
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::contrast_open()
{
    dlgContrastStretch->exec();
}

void MainWindow::on_dlgContrastStretch_okClicked(int m, double E)
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->contrast(m,int(E)));
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::histeq()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->histeq());
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::exact_histeq()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->exact_histeq());
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::spatial_filter()
{
    dlgSpatialFilter->exec();
}

void MainWindow::on_dlgSpatialFilter_okClicked(vector<double> data, int nCols, int nStyle)
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->spatial_filter(data, nCols, nStyle));
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::median_filter()
{
    dlgMedianFilter->exec();
}

void MainWindow::on_dlgMedianFilter_okClicked(int nRows, int nCols, int nStyle)
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->median_filter(nRows, nCols, nStyle));
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::fft2()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->fft2());
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::ifft2()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->ifft2());
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::on_dlgTransform_okClicked(const QMatrix &mat)
{
    QPointF pos=gpi->pos();
    double scale=gpi->get_scale();
    show_img(ip->transform(mat));
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::transform()
{
    dlgTransform->exec();
}

void MainWindow::erosion()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->erosion());
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::dilation()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->dilation());
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::opening()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->opening());
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::closing()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->closing());
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::gs_blur()
{
    dlgGuassianBlur->exec();
}

void MainWindow::on_dlaGuassianBlur_onClicked(int nStyle)
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->gs_blur(nStyle));
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::edge_extraction()
{
    dlgEdgeExtraction->exec();
}

void MainWindow::on_dlgEdgeExtraction_onClicked(int nStyle)
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->edge_extraction(nStyle));
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::emboss()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->emboss());
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

void MainWindow::oilPainting()
{
    QPointF pos = gpi->pos();
    double scale = gpi->get_scale();
    show_img(ip->oilPainting());
    gpi->setPos(pos);
    gpi->setTransformOriginPoint(
                gpi->boundingRect().width()/2,
                gpi->boundingRect().height()/2);
    gpi->setScale(scale);
}

