#include "imageprocesser.h"
#include <map>
#include <cmath>
#include <iostream>
using namespace std;
#define PI 3.1415926
extern vector<complex<double> >fft_res;

ImageProcesser::ImageProcesser(QImage img):
    _img(img)
{
}

QImage ImageProcesser::getImage()
{
    return _img;
}

QImage ImageProcesser::rgb2gray()
{
    int width = _img.width();
    int height = _img.height();
    for(int i = 0;i<width;++i)
    {
        for(int j=0;j<height;++j)
        {
            QRgb rgb =_img.pixel(i,j);
            int r = qRed(rgb);
            int g = qGreen(rgb);
            int b = qBlue(rgb);
            int gray = (r+g+b)/3;
            _img.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return _img;
}

QImage ImageProcesser::rgb2bw(int threshold)
{
    int width = _img.width();
    int height = _img.height();
    for(int i = 0; i<width; ++i)
    {
        for(int j=0; j<height; ++j)
        {
            QRgb rgb =_img.pixel(i,j);
            int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
            if( gray > threshold )
            {
                gray = 255;
            }
            else
            {
                gray = 0;
            }
            _img.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return _img;
}

QImage ImageProcesser::reverse()
{
    int width = _img.width();
    int height = _img.height();
    for(int i = 0; i<width; ++i)
    {
        for(int j=0; j<height; ++j)
        {
            QRgb rgb =_img.pixel(i,j);
            int r = qRed(rgb);
            int g = qGreen(rgb);
            int b = qBlue(rgb);
            _img.setPixel(i,j,qRgb(255-r,255-g,255-b));
        }
    }
    return _img;
}

QImage ImageProcesser::contrast(int contr_m, int contr_e)
{
    int width = _img.width();
    int height = _img.height();
    for(int i = 0; i<width; ++i)
    {
        for(int j=0; j<height; ++j)
        {
            QRgb rgb =_img.pixel(i,j);
            int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
            double d_gray = (double)((contr_m/(gray+1))^contr_e);
            double _gray = 1/(1+d_gray);
            int new_gray = (int)(255*_gray);
            _img.setPixel(i,j,qRgb(new_gray,new_gray,new_gray));
        }
    }
    return _img;
}

QImage ImageProcesser::log_trans(int log_c)
{
    int width = _img.width();
    int height = _img.height();
    for(int i = 0; i<width; ++i)
    {
        for(int j=0; j<height; ++j)
        {
            QRgb rgb =_img.pixel(i,j);
            int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
            double d_gray = (double)(1+gray);
            double d_c = (double)log_c;
            double _gray = d_c*(qLn(d_gray)/qLn(10));
            int new_gray = (int)(255*_gray);
            _img.setPixel(i,j,qRgb(new_gray,new_gray,new_gray));
        }
    }
    return _img;
}

QImage ImageProcesser::histeq()
{
    int width = _img.width();
    int height = _img.height();
    int hist[256] = {0};
    int sum = 0;
    int N = width*height;
    map<int,int> histMAP;
    for(int i = 0; i<width; ++i)
    {
        for(int j=0; j<height; ++j)
        {
            QRgb rgb =_img.pixel(i,j);
            int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
            hist[gray]++;
        }
    }
    for(int i = 0; i<256; ++i)
    {
        sum += hist[i];
        histMAP[i] = round(255*((double)sum/N));

    }
    for(int i = 0; i<width; ++i)
    {
        for(int j=0; j<height; ++j)
        {
            QRgb rgb =_img.pixel(i,j);
            int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
            int result = histMAP[gray];
            _img.setPixel(i,j,qRgb(result,result,result));
        }
    }
    return _img;
}

QImage ImageProcesser::exact_histeq()
{
    int width = _img.width();
    int height = _img.height();
    vector< vector<SixDim> >pixels(256);
    for(int i = 0; i<width; ++i)
    {
        for(int j=0; j<height; ++j)
        {
            int gray = getGray(i,j);
            int a1 = gray;
            int a2 = a1 + calcA2(i,j);
            int a3 = a2 + calcA3(i,j);
            int a4 = a3 + calcA4(i,j);
            int a5 = a4 + calcA5(i,j);
            int a6 = a5 + calcA6(i,j);
            pixels[gray].push_back(
                        SixDim(i,j,a1,a2/5.0,a3/9.0,a4/13.0,a5/21.0,a6/25.0));
        }
    }
    for (int i=0; i<256; ++i)
        myqsort<SixDim>(pixels[i].begin(),pixels[i].end());

    int N = width * height;
    int numPerGray = N/256;
    int current = 0;
    for(int i=0; i<256; ++i)
    {
        for(int j=0; j<pixels[i].size(); ++j)
        {
            int w = pixels[i][j].getX();
            int h = pixels[i][j].getY();
            int gray = current / numPerGray;
            if (gray>255) gray = 255;
            _img.setPixel(w,h,qRgb(gray,gray,gray));
            current ++;
        }
    }
    return _img;
}

int ImageProcesser::getGray(int i, int j)
{
    QRgb rgb = _img.pixel(i,j);
    return (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
}

int ImageProcesser::calcA2(int i,int j)
{
    int sum = 0;
    if (_img.valid(i,j-1))
    {
        sum += getGray(i,j-1);
    }
    if (_img.valid(i,j+1))
    {
        sum += getGray(i,j+1);
    }
    if (_img.valid(i-1,j))
    {
        sum += getGray(i-1,j);
    }
    if (_img.valid(i+1,j))
    {
        sum += getGray(i+1,j);
    }
    return sum;
}

int ImageProcesser::calcA3(int i, int j)
{
    int sum = 0;
    if(_img.valid(i-1,j-1))
    {
        sum += getGray(i-1,j-1);
    }
    if(_img.valid(i+1,j-1))
    {
        sum += getGray(i+1,j-1);
    }
    if(_img.valid(i-1,j+1))
    {
        sum += getGray(i-+1,j+1);
    }
    if(_img.valid(i+1,j+1))
    {
        sum += getGray(i+1,j+1);
    }
    return sum;
}

int ImageProcesser::calcA4(int i, int j)
{
    int sum = 0;
    if(_img.valid(i,j-2))
    {
        sum += getGray(i,j-2);
    }
    if(_img.valid(i-2,j))
    {
        sum += getGray(i-2,j);
    }
    if(_img.valid(i,j+2))
    {
        sum += getGray(i,j+2);
    }
    if(_img.valid(i+2,j))
    {
        sum += getGray(i+2,j);
    }
    return sum;
}

int ImageProcesser::calcA5(int i, int j)
{
    int sum = 0;
    if(_img.valid(i-2,j-1))
    {
        sum += getGray(i-2,j-1);
    }
    if(_img.valid(i-2,j+1))
    {
        sum += getGray(i-2,j+1);
    }
    if(_img.valid(i-1,j-2))
    {
        sum += getGray(i-1,j-2);
    }
    if(_img.valid(i-1,j+2))
    {
        sum += getGray(i-1,j+2);
    }
    if(_img.valid(i+1,j-2))
    {
        sum += getGray(i+1,j-2);
    }
    if(_img.valid(i+1,j+2))
    {
        sum += getGray(i+1,j+2);
    }
    if(_img.valid(i+2,j-1))
    {
        sum += getGray(i+2,j-1);
    }
    if(_img.valid(i+2,j+1))
    {
        sum += getGray(i+2,j+1);
    }
}

int ImageProcesser::calcA6(int i, int j)
{
    int sum = 0;
    if(_img.valid(i+2,j+2))
    {
        sum += getGray(i+2,j+2);
    }
    if(_img.valid(i+2,j-2))
    {
        sum += getGray(i+2,j-2);
    }
    if(_img.valid(i-2,j+2))
    {
        sum += getGray(i-2,j+2);
    }
    if(_img.valid(i-2,j-2))
    {
        sum += getGray(i-2,j-2);
    }
}

template<typename T>
void ImageProcesser::myqsort(typename vector<T>::iterator begin,
                             typename vector<T>::iterator end) const
{

    if (begin == end) return;
    typename vector<T>::iterator iter1 = begin;//指针A
    typename vector<T>::iterator iter2 = end-1;//指针B
    while(iter1 != iter2)
    {
        int d = distance(iter1,iter2);
        if(d>0)
        {
            if(*iter1<*iter2)
            {
                iter2--;
                continue;
            }
            else
            {
                T tempt1 = *iter1;
                *iter1 = *iter2;
                *iter2 = tempt1;
                typename vector<T>::iterator tmp2 = iter1;
                iter1 = iter2;
                iter2 = tmp2;
                iter2++;
            }
        }
        else
        {
            if(*iter2<*iter1)
            {
                iter2++;
            }
            else
            {
                T tmp1 = *iter1;
                *iter1 = *iter2;
                *iter2 = tmp1;
                typename vector<T>::iterator tmp2 = iter2;
                iter2 = iter1;
                iter1 = tmp2;
                iter2--;
            }
        }
    }
    myqsort<T>(begin,iter1);
    myqsort<T>(iter1+1,end);
}

QImage ImageProcesser::spatial_filter(vector<double> data, int nCols, int nStyle)
{
    int nRows = data.size()/nCols;
    if (nRows % 2 != 1 || nCols % 2 != 1)
    {
        cout<< "nCols and nRows should be odd"<<endl;
        return _img;
    }
    switch (nStyle)
    {
    case 0:
        zeroPadding(nCols, nRows);//补零边界扩展
        break;
    case 1:
        mirrorPadding(nCols, nRows);//镜像扩展
        break;
    case 2:
        dupliPadding(nCols, nRows);//复制扩展
        break;
    }
    filter(data, nCols);//滤波
    crop(nCols, nRows);//裁边
    return _img;
}

void ImageProcesser::zeroPadding(int nCols, int nRows)//补零扩展
{
    QImage image = QImage(_img.width()+nCols-1, _img.height()+nRows-1, _img.format());
    int width = image.width();
    int height = image.height();
    for (int i=0; i<width; ++i)
    {
        for (int j=0; j<height; ++j)
        {
            if (i<nCols/2 || i>=(nCols/2+_img.width()) ||
                    j<nRows/2 || j>=(nRows/2+_img.height()))
            {
                image.setPixel(i,j,qRgb(0,0,0));
            }
            else
            {
                QRgb rgb =_img.pixel(i-nCols/2, j-nRows/2);
                int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
        }
    }
    _img = image;
}

void ImageProcesser::mirrorPadding(int nCols, int nRows)//镜像 边界扩展
{
    QImage image = QImage(_img.width()+nCols-1, _img.height()+nRows-1, _img.format());
    int width = image.width();
    int height = image.height();
    int ii = 0, jj = 0;
    for (int i=0; i<width; ++i)
    {
        for (int j=0; j<height; ++j)
        {
            if (i<nCols/2 && j<nRows/2)//左上角
            {
                QRgb rgb =_img.pixel(nCols/2-i-1,nRows/2-j-1);
                int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            else if (i>=nCols/2 && i<(nCols/2+_img.width()) && j<nRows/2)//上边
            {
                QRgb rgb =_img.pixel(i-nCols/2,nRows/2-j-1);
                int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            else if (i>=(nCols/2+_img.width()) && j<nRows/2)//右上角
            {
                QRgb rgb =_img.pixel(2*_img.width()+nCols/2-i-1,nRows/2-j-1);
                int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            else if (i<nCols/2 && j>=nRows/2 && j<(nRows/2+_img.height()))//左边
            {
                QRgb rgb =_img.pixel(nCols/2-i-1,j-nRows/2);
                int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            else if (i>=(nCols/2+_img.width()) && j>=nRows/2 && j<(nRows/2+_img.height()))//右边
            {
                QRgb rgb =_img.pixel(2*_img.width()+nCols/2-i-1,j-nRows/2);
                int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            else if (i<nCols/2 && j>=(nRows/2+_img.height()))//左下角
            {
                QRgb rgb =_img.pixel(nCols/2-i-1,2*_img.height()+nRows/2-j-1);
                int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            else if (i>=nCols/2 && i<(nCols/2+_img.width()) && j>=(nRows/2+_img.height()))//下边
            {
                QRgb rgb =_img.pixel(i-nCols/2,2*_img.height()+nRows/2-j-1);
                int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            else if (i>=(nCols/2+_img.width()) && j>=(nRows/2+_img.height()))//右下角
            {
                QRgb rgb =_img.pixel(2*_img.width()+nCols/2-i-1,2*_img.height()+nRows/2-j-1);
                int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            else
            {
                QRgb rgb =_img.pixel(i-nCols/2, j-nRows/2);
                int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
        }
    }
    _img = image;
}

void ImageProcesser::dupliPadding(int nCols, int nRows)//复制 边界扩展
{
    QImage image = QImage(_img.width()+nCols-1, _img.height()+nRows-1, _img.format());
    int width = image.width();
    int height = image.height();
    int ii = 0, jj = 0;
    for (int i=0; i<width; ++i)
    {
        for (int j=0; j<height; ++j)
        {
            if (i<nCols/2 || i>=(nCols/2+_img.width()) ||
                    j<nRows/2 || j>=(nRows/2+_img.height()))
            {
                ii = i+_img.width();
                jj = j+_img.height();
                if (ii > (nCols/2+_img.width()-1))    ii = ii % _img.width();
                if (ii == 0) ii = 1;
                if (jj > (nRows/2+_img.height()-1))    jj = jj % _img.height();
                if (jj == 0) jj = 1;
                QRgb rgb =_img.pixel(ii-nCols/2, jj-nRows/2);
                int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            else
            {
                QRgb rgb =_img.pixel(i-nCols/2, j-nRows/2);
                int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
        }
    }
    _img = image;
}

void ImageProcesser::filter(vector<double> data, int nCols)
{
    QImage image(_img);
    int nRows = data.size()/nCols;
    int width = _img.width();
    int height = _img.height();
    for (int i=nCols/2; i<width-nCols/2; ++i)
    {
        for (int j=nRows/2; j<height-nRows/2; ++j)
        {
            double sum =0;
            vector<int> patch(data.size());
            patch = getPatch( i, j, nRows, nCols);
            for (int k=0; k<data.size(); ++k)
            {
                sum+= patch[k]*data[k];
            }
            image.setPixel(i,j,qRgb(sum,sum,sum));
        }
    }
    _img = image;
}

vector<int> ImageProcesser::getPatch(int i, int j, int nRows, int nCols)
{
    vector<int> patch;
    for (int jj=j-nRows/2; jj<=j+nRows/2; ++jj)
    {
        for(int ii=i-nCols/2; ii<=i+nCols/2; ++ii)
        {
            QRgb rgb =_img.pixel(ii, jj);
            int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
            patch.push_back(gray);

        }
    }
    return patch;
}

void ImageProcesser::crop(int nCols, int nRows)
{
    QImage image(_img.width()-nCols+1, _img.height()-nRows+1, _img.format());
    int width = _img.width();
    int height = _img.height();
    for (int i=nCols/2; i<width-nCols/2; ++i)
    {
        for (int j=nRows/2; j<height-nRows/2; ++j)
        {
            QRgb rgb = _img.pixel(i,j);
            int gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
            image.setPixel(i-nCols/2, j-nRows/2,qRgb(gray,gray,gray));
        }
    }
    _img = image;
}


QImage ImageProcesser::median_filter(int nRows, int nCols, int nStyle)//中值滤波器
{
    if (nRows % 2 != 1 || nCols % 2 != 1)
    {
        cout<< "nCols and nRows should be odd"<<endl;
        return _img;
    }
    switch (nStyle)
    {
    case 0:
        zeroPadding(nCols, nRows);//补零边界扩展
        break;
    case 1:
        mirrorPadding(nCols, nRows);//镜像扩展
        break;
    case 2:
        dupliPadding(nCols, nRows);//复制扩展
        break;
    }
    medianfilter(nRows, nCols);//滤波
    crop(nCols, nRows);//裁边
    return _img;
}

void ImageProcesser::medianfilter(int nRows, int nCols)
{
    QImage image(_img);
    int width = _img.width();
    int height = _img.height();
    for (int i=nCols/2; i<width-nCols/2; ++i)
    {
        for (int j=nRows/2; j<height-nRows/2; ++j)
        {
            vector<int>patch = getPatch(i, j, nRows, nCols);
            myqsort<int>(patch.begin(),patch.end());
            int gray = patch.at(patch.size()/2+1);
            image.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    _img = image;
}

QImage ImageProcesser::fft2()
{
    fft_res.clear();
    int width = _img.width();
    int height = _img.height();
    int N = calcN(width);
    int M = calcN(height);
    matrix temp(width,height,1);
    for(int i = 0; i<width; ++i)
    {
        for(int j=0; j<height; ++j)
        {
            QRgb rgb =_img.pixel(i,j);
            double gray = (qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3.0;
            temp(i,j) = complex<double>(gray,0); 
        }
    }
    matrix ret = fft2d(temp,0,0);
    double max = log(sqrt(ret(0,0).real()*ret(0,0).real()+ret(0,0).imag()*ret(0,0).imag()));
    double min = log(sqrt(ret(0,0).real()*ret(0,0).real()+ret(0,0).imag()*ret(0,0).imag()));
    for(int i=0; i<M; ++i)
    {
        for(int j=0; j<N; ++j)
        {
            fft_res.push_back(ret(i,j));
            double value = log(1+sqrt(ret(i,j).real()*ret(i,j).real()+ret(i,j).imag()*ret(i,j).imag()));
            if(value>max) max = value;
            if(value<min) min = value;
        }
    }
    double c = max-min;
    ret.colCyclicShift(height/2);
    ret.rowCyclicShift(width/2);
    for(int i=0; i<width; ++i)
    {
        for(int j=0; j<height; ++j)
        {
            int gray = int((log(1+sqrt(ret(i,j).real()*ret(i,j).real()+ret(i,j).imag()*ret(i,j).imag()))/c)*255.0);
            _img.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return _img;
}

QImage ImageProcesser::ifft2()
{
    int width = _img.width();
    int height = _img.height();
    int N = calcN(width);
    int M = calcN(height);
    matrix temp(N,M,1);
    for(int i=0; i<N; ++i)
    {
        for(int j=0; j<M; ++j)
        {
            temp(i,j)=fft_res[i*N+j];
        }
    }
    matrix ret = ifft2d(temp,0,0);
    for(int i=0; i<width; ++i)
    {
        for(int j=0; j<height; ++j)
        {
            int gray = ret(i,j).real();
            _img.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return _img;
}

size_t ImageProcesser::calcN(size_t length)
{
    if (0==(length & (length-1))) {
        return length;
    }
    size_t temp = length;
    while (temp >>= 1) {
        length |= temp;
    }
    return length+1;
}

complex<double> ImageProcesser::pow(complex<double> base, int exponent)
{
    complex<double> ret = 1;
    for (int i=0; i<exponent; ++i) {
        ret *= base;
    }
    return ret;
}

vector<complex<double> >
ImageProcesser::fft(vector<complex<double> > data, size_t N)
{
    // change length to make it the power of 2
    if (0==N) {
        N = calcN(data.size());
    }
    if (0!=(N & (N-1))) {
        cout << "error N" << endl;
        return data;
    }
    // append 0 if necessary
    size_t delta = N - data.size();
    while(delta--) {
        data.push_back(complex<double>(0,0));
    }

    vector<complex<double> > ret;
    if (0 == N) {
        return ret;
    } else if(1 == N) {
        return data;
    } else if (2 == N) {
        // calculate if only two elements in data_
        ret.push_back(data[0]+data[1]);
        ret.push_back(data[0]-data[1]);
        return ret;
    } else {
        // digui
        vector<complex<double> > odd, even, oddRet, evenRet;
        for (size_t i = 0; i<data.size(); i += 2) {
            even.push_back(data[i]);
            odd.push_back(data[i+1]);
        }
        evenRet = fft(even);
        oddRet = fft(odd);

        // order the output
        complex<double> w_k1(cos(-2*PI/N), sin(-2*PI/N));
        for(size_t i=0; i<N/2; ++i) {
            ret.push_back(evenRet[i]+oddRet[i]*pow(w_k1, i));
        }
        for(size_t i=N/2; i<N; ++i) {
            size_t ii = i-N/2;
            ret.push_back(evenRet[ii]-oddRet[ii]*pow(w_k1, ii));
        }
        return ret;
    }
}

vector<complex<double> >
ImageProcesser::ifft(vector<complex<double> > res, size_t N)
{
    // determin N
    if (0 == N){
        N = calcN(res.size());
    }

    // append 0 if necessary
    size_t delta = N-res.size();
    while(delta--){
        res.push_back(complex<double>(0,0));
    }

    // do the calculation
    vector<complex<double> >ret;
    // when N=0
    if ( 0==N ){
        return res;
    }
    //when N=1
    if (1 == N ){
        return res;
    }
    // when N=2
    if (2 == N ){
        ret.push_back((res[0] + res[1])/2.0);
        ret.push_back((res[0] - res[1])/2.0);
        return ret;
    }
    // when N>2
    if (N > 2){
        vector<complex<double> >odd, even, oddRet, evenRet;
        for (size_t i=0; i<N; i+=2) {
            even.push_back(res[i]/2.0);
            odd.push_back(res[i+1]/2.0);
        }
        evenRet = ifft (even);
        oddRet = ifft (odd);
        complex<double> w_N(cos(2*PI/N), sin(2*PI/N));
        for (size_t i =0; i<N/2; ++i) {
            ret.push_back(evenRet[i] + oddRet[i]*pow(w_N,i));
        }
        for (size_t i =0; i<N/2; ++i) {
            ret.push_back(evenRet[i] - oddRet[i]*pow(w_N,i));
        }
        return ret;
    }
}

matrix ImageProcesser::fftRow(const matrix &data)
{
    matrix ret(data);
    for (size_t i=0; i<ret.Rows(); ++i) {
        ret.setRow(i, fft(data.getRow(i)) );
    }
    return ret;
}

matrix ImageProcesser::ifftRow(const matrix &data)
{
    matrix ret(data);
    for (size_t i=0; i<ret.Rows(); ++i) {
        ret.setRow(i, ifft(data.getRow(i)) );
    }
    return ret;
}

matrix ImageProcesser::fft2d(const matrix &data, size_t M, size_t N)
{
    // dertermin M and N
    if(0==M) M = calcN(data.Rows());
    if(0==N) N = calcN(data.Cols());
    if(0!=(M&(M-1)) || 0!=(N&(N-1)))
    {
        cout<<"error"<<endl;
        return data;
    }
    // append 0
    matrix temp(M,N,0);
    for(size_t i = 0;i<M;++i)
    {
        for(size_t j =0;j<N;++j)
        {
            try{
                temp(i,j) = data(i,j);
            } catch(range_error e) {
                continue;
            }
        }
    }
    return fftRow(fftRow(temp).T()).T();
}

matrix ImageProcesser::ifft2d(const matrix &data, size_t M, size_t N)
{
    // dertermin M and N
    if(0==M) M = calcN(data.Rows());
    if(0==N) N = calcN(data.Cols());
    if(0!=(M&(M-1)) || 0!=(N&(N-1)))
    {
        cout<<"error"<<endl;
        return data;
    }
    // append 0
    matrix temp(M,N,0);
    for(size_t i = 0;i<M;++i)
    {
        for(size_t j =0;j<N;++j)
        {
            try{
                temp(i,j) = data(i,j);
            } catch(range_error e) {
                continue;
            }
        }
    }
    return ifftRow(ifftRow(temp).T()).T();
}

QImage& ImageProcesser::transform(const QMatrix &tform)
{
    // check if tform is invertible
    bool invertible = true;
    QMatrix inv_tform = tform.inverted(&invertible);
    if (!invertible)    return _img;

    // determine the size of new image
    double minX, maxX, minY, maxY;
    getRange(tform, minX, maxX, minY, maxY);

    // and create new QImage
    int newWidth = ceil(maxX-minX);
    int newHeight = ceil(maxY-minY);
    QImage newImg( newWidth, newHeight, _img.format());

    // backward map per pixel
    for(int i=0;i<newWidth;++i)
    {
        for(int j=0;j<newHeight;++j)
        {
            QRgb newRgb = getNewRgb(inv_tform, i, j, minX, minY);
            newImg.setPixel(i,j, newRgb);
        }
    }
    _img = newImg;
    return _img;
}

void ImageProcesser::getRange(const QMatrix &tform, double& minX,
                              double &maxX, double& minY, double& maxY) const
{
    // get four points in transformed image
    vector <qreal> vx(4),vy(4);
    tform.map(0,0,&vx[0],&vy[0]);
    tform.map(0,_img.height()-1,&vx[1],&vy[1]);
    tform.map(_img.width()-1,0,&vx[2],&vy[2]);
    tform.map(_img.width()-1,_img.height()-1,&vx[3],&vy[3]);

    // sort x and y
    myqsort<qreal>(vx.begin(),vx.end());
    myqsort<qreal>(vy.begin(),vy.end());

    // get the min and max of x and y
    minX=vx[0]; maxX=vx[3];
    minY=vy[0]; maxY=vy[3];
}

QRgb ImageProcesser::getNewRgb(const QMatrix &tform, int i, int j
                               ,double minX, double minY) const
{
    // get the position in origal image
    double srcX,srcY;
    tform.map(1+minX,j+minY,&srcX,&srcY);

    // if it is out of the original image
    if(srcX<0 ||srcX>_img.width()-1 || srcY<0 || srcY>_img.height()-1)
    {
        return qRgb(0,0,0);
    }

    // get RGB by bilinear interpolation
    else
    {
        return bilinear(srcX,srcY);
    }
}

QRgb ImageProcesser::bilinear(double x, double y) const
{
    // get four pixels
    int x1 = ceil(x);
    int x2 = floor(x);
    int y1 = ceil(y);
    int y2 = floor(y);
    QRgb rgb1 = _img.pixel(x1,y1);
    QRgb rgb2 = _img.pixel(x1,y2);
    QRgb rgb3 = _img.pixel(x2,y1);
    QRgb rgb4 = _img.pixel(x2,y1);

    // bilinear on red
    double tmp1,tmp2;
    tmp1 = linear_inter(qRed(rgb1),qRed(rgb2),y1,y2,y);
    tmp2 = linear_inter(qRed(rgb3),qRed(rgb4),y1,y2,y);
    double r = linear_inter(tmp1,tmp2,x1,x2,x);

    //bilinear on greed
    tmp1 = linear_inter(qGreen(rgb1),qGreen(rgb2),y1,y2,y);
    tmp2 = linear_inter(qGreen(rgb3),qGreen(rgb4),y1,y2,y);
    double g = linear_inter(tmp1,tmp2,x1,x2,x);

    //bilinear on blue
    tmp1 = linear_inter(qBlue(rgb1),qBlue(rgb2),y1,y2,y);
    tmp2 = linear_inter(qBlue(rgb3),qBlue(rgb4),y1,y2,y);
    double b = linear_inter(tmp1,tmp2,x1,x2,x);
    return qRgb(r,g,b);
}

double ImageProcesser::linear_inter(double v1, double v2, double c1,
                                    double c2, double c3) const
{
    // follow the formula of linear interpolation
    if(c1==c2) return v1;
    return (c2-c3)/(c2-c1)*v1+(c3-c1)/(c2-c1)*v2;
}

QImage ImageProcesser::erosion()
{
    //先二值化再做膨胀或腐蚀
    //初始化结构元素
    B[0][0] = B[0][2] = B[2][0] = B[2][2] = 0;
    B[0][1] = B[1][0] = B[1][1] = B[1][2] = B[2][1] = 1;
    QImage image(_img);
    int width = _img.width();
    int height = _img.height();
    for(int y = 1; y<height-1; y++)
    {
        for(int x = 1; x < width-1; x++)
        {
            if (hit(x, y) == -1)
            {
                image.setPixel(x,y, qRgb(0, 0, 0));
            }
            else
            {
                image.setPixel(x,y, qRgb(255, 255, 255));
            }
        }
    }
    _img = image;
    return _img;
}

QImage ImageProcesser::dilation()
{
    //初始化结构元素
    B[0][0] = B[0][2] = B[2][0] = B[2][2] = 0;
    B[0][1] = B[1][0] = B[1][1] = B[1][2] = B[2][1] = 1;
    QImage image(_img);
    int width = _img.width();
    int height = _img.height();
    for(int y = 1; y<height-1; y++)
    {
        for(int x = 1; x < width-1; x++)
        {
            if (hit(x, y) == 1)
            {
                image.setPixel(x,y, qRgb(255, 255, 255));
            }
            else
            {
                image.setPixel(x,y, qRgb(0, 0, 0));
            }
        }
    }
    _img = image;
    return _img;
}

int ImageProcesser::hit(int w, int h)
{
    int count = 0;
    for (int i = -1; i <= 1; i ++)
    {
        for (int j = -1; j <= 1; j ++)
        {
            if (qRed(_img.pixel(w+i,h+j))/255 && B[i+1][j+1])
            {
                count++;
            }
        }
    }
    if (count == 0)
    {
        return -1;
    }
    if (count == 5)
    {
        return 1;
    }
    return 0;
}

QImage ImageProcesser::opening()
{
    //开运算即为先腐蚀再膨胀
    QImage image = erosion();
    image = dilation();
    _img = image;
    return _img;
}

QImage ImageProcesser::closing()
{
    //闭运算为先膨胀再腐蚀
    QImage image = dilation();
    image = erosion();
    _img = image;
    return _img;
}

QImage ImageProcesser::gs_blur(int nStyle)
{
    int nCols = 3;
    int nRows = 3;
    switch (nStyle)
    {
    case 0:
        zeroPadding(nCols, nRows);//补零边界扩展
        break;
    case 1:
        mirrorPadding(nCols, nRows);//镜像扩展
        break;
    case 2:
        dupliPadding(nCols, nRows);//复制扩展
        break;
    }
    QImage image(_img);
    int width = _img.width();
    int height = _img.height();
    int a[9] = {1,2,1,2,4,2,1,2,1};
    vector<int> gs(a,a+9);
    for (int i=nCols/2; i<width-nCols/2; ++i)
    {
        for (int j=nRows/2; j<height-nRows/2; ++j)
        {
            double sum =0;
            vector<int> patch(gs.size());
            patch = getPatch( i, j, nRows, nCols);
            for (int k=0; k<gs.size(); ++k)
            {
                sum+= patch[k]*gs[k];
            }
            sum = sum/16;

            image.setPixel(i,j,qRgb(sum,sum,sum));
        }
    }
    _img = image;
    crop(3,3);
    return _img;
}

QImage ImageProcesser::edge_extraction(int nStyle)
{
    QImage image(_img);
    switch (nStyle)
    {
    case 0:
        image = soberts();//sorberts算子
        break;
    case 1:
        image = sobel();//sobel算子
        break;
    case 2:
        image = laplacian();//laplacian算子
        break;
    }
    _img = image;
    return _img;
}

QImage ImageProcesser:: soberts()
{
    QImage image(_img);
    int width = _img.width();
    int height = _img.height();
    double x[4] = {-1,0,0,1};
    vector<double> cx(x,x+4);
    vector<double> dx = filter1(cx);//水平梯度
    double y[4] = {0,-1,1,0};
    vector<double> cy(y,y+4);
    vector<double> dy = filter1(cy);//垂直梯度
    vector<double> b = sharpen(dx,dy);
    double mean = b[width*height];
    int thresh = 4*(int)mean;//二值化阈值

    for(int i=0; i<width;++i)
    {
        for(int j=0; j<height;++j)
        {
            int gray = (int)b[i*height+j];
            if(gray >= thresh)
            {
                gray = 255;
            }
            else
            {
                gray = 0;
            }
            image.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return image;
}

QImage ImageProcesser::sobel()
{
    QImage image(_img);
    int width = _img.width();
    int height = _img.height();
    double x[9] = {-1,0,1,-2,0,2,-1,0,1};
    vector<double> cx(x,x+9);
    vector<double> dx = filter2(cx);//水平梯度
    double y[9] = {-1,-2,-1,0,0,0,1,2,1};
    vector<double> cy(y,y+9);
    vector<double> dy = filter2(cy);//垂直梯度
    vector<double> b = sharpen(dx,dy);

    double mean = b[width*height];
    int thresh = (int)mean;//二值化阈值

    for(int i=0; i<width;++i)
    {
        for(int j=0; j<height;++j)
        {
            int gray = (int)b[i*height+j];
            /*if(gray >= thresh)
            {
                gray = 255;
            }
            else
            {
                gray = 0;
            }*/
            image.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return image;
}

QImage ImageProcesser::laplacian()
{
    QImage image(_img);
    int width = _img.width();
    int height = _img.height();
    double x[9] = {0,-1,0,-1,4,-1,0,-1,0};
    vector<double> cx(x,x+9);
    vector<double> dx = filter2(cx);//水平梯度
    double y[9] = {0,-1,0,-1,4,-1,0,-1,0};
    vector<double> cy(y,y+9);
    vector<double> dy = filter2(cy);//垂直梯度
    vector<double> b = sharpen(dx,dy);

    double mean = b[width*height];
    int thresh = (int)mean;//二值化阈值

    for(int i=0; i<width;++i)
    {
        for(int j=0; j<height;++j)
        {
            int gray = (int)b[i*height+j];

            /*if(gray >= thresh)
            {
                gray = 255;
            }
            else
            {
                gray = 0;
            }*/
            image.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return image;
}

vector<double> ImageProcesser::filter1(vector<double> data)
{
    vector<double> ret;
    int sum;
    //梯度图像的最下面一行和最右边一行设为0
    for(int i=0; i<_img.width()-1; ++i)
    {
        for(int j=0; j<_img.height()-1; ++j)
        {
            sum = getGray(i,j)*data[0]+getGray(i,j+1)*data[1]
                    +getGray(i+1,j)*data[2]+getGray(i+1,j+1)*data[3];
            ret.push_back(sum);
        }
        ret.push_back(0);
    }
    for(int j=0; j<_img.height(); ++j)
    {
        ret.push_back(0);
    }
    return ret;
}

vector<double> ImageProcesser::filter2(vector<double> data)
{
    int sum;
    vector<double> ret;
    //梯度图像的四周像素点设为0
    for(int j=0; j<_img.height();++j)
    {
        ret.push_back(0);
    }
    for(int i=1; i<_img.width()-1; ++i)
    {
        ret.push_back(0);
        for(int j=1; j<_img.height()-1; ++j)
        {
            sum = 0;
            for(int ii=-1; ii<2; ++ii)
            {
                for(int jj=-1; jj<2; ++jj)
                {
                    sum += getGray(i+ii,j+jj)*data[(ii+1)*3+jj+1];
                }
            }
            ret.push_back(sum);
        }
        ret.push_back(0);
    }
    for(int j=0; j<_img.height();++j)
    {
        ret.push_back(0);
    }
    return ret;
}

vector<double> ImageProcesser::sharpen(vector<double> dx, vector<double> dy)
{
    double sum = 0;
    //将水平梯度图和垂直梯度图结合起来
    vector<double> ret;
    for(int i=0; i<_img.width()*_img.height();++i)
    {
        double d = sqrt(dx[i]*dx[i]+dy[i]*dy[i]);
        sum += d;
        ret.push_back(d);
    }
    double mean = sum/(_img.width()*_img.height());
    ret.push_back(mean);//将梯度图像的均值放在vector的最后一个
    return ret;
}

QImage ImageProcesser::emboss()
{
    int x[9] = {1,0,0,0,0,0,0,0,-1};
    vector<int> data(x,x+9);
    QImage image(_img);
    int width = _img.width();
    int height = _img.height();
    for (int i=1; i<width-1; ++i)
    {
        for (int j=1; j<height-1; ++j)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            for(int ii=-1; ii<2; ++ii)
            {
                for(int jj=-1; jj<2; ++jj)
                {
                    //分别对三个通道进行滤波
                    r += qRed(_img.pixel(i+ii,j+jj))*data[(ii+1)*3+jj+1];
                    g += qGreen(_img.pixel(i+ii,j+jj))*data[(ii+1)*3+jj+1];
                    b += qBlue(_img.pixel(i+ii,j+jj))*data[(ii+1)*3+jj+1];
                }
            }
            r += 128;
            b += 128;
            g += 128;
            if(r > 255) r = 255;
            if(r < 0) r = 0;
            if(g > 255) g = 255;
            if(g < 0) g = 0;
            if(b > 255) b = 255;
            if(b < 0) b = 0;
            image.setPixel(i,j,qRgb(r,g,b));
        }
    }
    _img = image;
    return _img;
}

QImage ImageProcesser::oilPainting()
{
    QImage image(_img);
    int width = _img.width();
    int height = _img.height();
    int Radius = 2;
    for(int i=Radius; i<width-Radius; ++i)
    {
        for(int j=Radius; j<height-Radius; ++j)
        {
            int IntensityCount[20] = {0};
            int sumR[20] = {0};
            int sumG[20] = {0};
            int sumB[20] = {0};
            for(int ii=-Radius; ii<=Radius; ++ii)
            {
                for(int jj=-Radius; jj<=Radius; ++jj)
                {
                    //计算强度
                    int currentIntensity = (getGray(i+ii,j+jj)*20)/255;
                    if(currentIntensity > 255) currentIntensity = 255;
                    int index = currentIntensity;
                    IntensityCount[index]++;
                    sumR[index] += qRed(_img.pixel(i+ii,j+jj));
                    sumG[index] += qGreen(_img.pixel(i+ii,j+jj));
                    sumB[index] += qBlue(_img.pixel(i+ii,j+jj));
                }
            }
            int CurMax = 0;
            int MaxIndex = 0;
            //获得权重最大的强度值
            for(int i=0; i<20; i++)
            {
                if(IntensityCount[i] > CurMax)
                {
                    CurMax = IntensityCount[i];
                    MaxIndex = i;
                }
            }
            //将最大权重强度值对应的RGB分量的均值赋给对应像素
            image.setPixel(i,j,qRgb(sumR[MaxIndex]/CurMax,sumG[MaxIndex]/CurMax,sumB[MaxIndex]/CurMax));
        }
    }
    _img = image;
    return _img;
}
