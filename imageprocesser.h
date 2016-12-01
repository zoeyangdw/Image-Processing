#ifndef IMAGEPROCESSER_H
#define IMAGEPROCESSER_H

#include <QImage>
#include <QRgb>
#include <qmath.h>
#include <vector>
#include "sixdim.h"
#include "matrix_.h"
#include <iterator>
using std::vector;

class ImageProcesser
{
public:
    ImageProcesser(QImage img);
    QImage getImage();
    QImage rgb2gray();
    QImage rgb2bw(int threshold);
    QImage reverse();
    QImage contrast(int contr_m,int contr_e);
    QImage log_trans(int log_c);
    QImage histeq();
    QImage exact_histeq();
    QImage spatial_filter(vector<double>data, int nCols, int nStyle);
    QImage median_filter(int nRows, int nCols, int nStyle);
    QImage fft2();
    QImage ifft2();
    QImage& transform(const QMatrix& tform);
    QImage erosion();
    QImage dilation();
    QImage opening();
    QImage closing();
    QImage gs_blur(int nStyle);
    QImage edge_extraction(int nStyle);
    QImage emboss();
    QImage oilPainting();

private:
    QImage _img;
    int getGray(int i, int j);
    int calcA2(int i,int j);
    int calcA3(int i,int j);
    int calcA4(int i,int j);
    int calcA5(int i,int j);
    int calcA6(int i,int j);
    template<typename T>
    void myqsort(typename std::vector<T>::iterator begin,
                 typename std::vector<T>::iterator end) const;
    void zeroPadding(int nCols, int nRows);
    void mirrorPadding(int nCols, int nRows);
    void dupliPadding(int nCols, int nRows);
    void filter(vector<double>data, int nCols);
    void crop(int nCols, int nRows);
    vector<int> getPatch(int i, int j, int nRows, int nCols);
    void medianfilter(int nRows, int nCols);
    size_t calcN(size_t length);
    complex<double> pow(complex<double> base, int exponent);
    vector<complex<double> > fft(vector<complex<double> > data, size_t N=0);
    vector<complex<double> > ifft(vector<complex<double> > data, size_t N=0);
    matrix fft2d(const matrix& data, size_t M=0, size_t N=0);
    matrix ifft2d(const matrix& res, size_t M=0, size_t N=0);
    matrix fftRow(const matrix& data);
    matrix ifftRow(const matrix& data);

    void getRange(const QMatrix& tform, double &minX, double& maxX,
                  double& minY, double& maxY) const;
    QRgb bilinear(double x, double y) const;
    double linear_inter(double v1, double v2, double c1, double c2, double c3) const;
    QRgb getNewRgb(const QMatrix& tform, int i, int j, double minX, double minY) const;

    bool B[3][3];//结构元素B
    int hit(int w, int h);      //全中返回1，全不中返回-1，否则返回0

    QImage soberts();
    QImage sobel();
    QImage laplacian();
    vector<double> filter1(vector<double> data);//一阶
    vector<double> filter2(vector<double> data);//二阶
    vector<double> sharpen(vector<double>dx, vector<double>dy);

};

#endif // IMAGEPROCESSER_H
