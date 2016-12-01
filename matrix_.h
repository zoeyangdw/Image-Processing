#ifndef MATRIX_H
#define MATRIX_H

#include <complex>
#include <iostream>
#include <stdexcept>
#include <vector>
using std::complex;
using std::ostream;
using std::vector;

class matrix
{
private:
    complex<double>** data_;
    size_t rows_, cols_;
    int t_, startRow_, startCol_;
    // check if r and c are out of range
	bool checkRange(size_t r, size_t c) const;
    // used by operator=
    void swap(matrix& mat);
    // get element in position (r,c), used by operator()
    complex<double>& getElem(int r, int c) const;
	matrix();

public:
    // constructors
    matrix(size_t r, size_t c);
    matrix(size_t r, size_t c, const complex<double>& init);
    matrix(const matrix& mat);
    // copy assignment
    // Copy-and-swap idiom
    matrix& operator=(matrix mat);
    // deconstructor
    ~matrix();
    // operator(), it is used to get and set element
    complex<double>& operator()(size_t r, size_t c);
    const complex<double>& operator()(size_t r, size_t c) const;
    // operator<<, it is used to out put
    friend ostream& operator<<(ostream& out, const matrix& rhs);
    // transpose
    matrix& T();
    // get the number of rows or columns in matrix
    size_t Rows() const;
    size_t Cols() const;
    // get the specific row or column in matrix
    vector<complex<double> > getRow(int r) const;
    vector<complex<double> > getCol(int c) const;
    // replace the specific row or column with given data
    bool setRow(int r, vector<complex<double> > row);
    bool setCol(int c, vector<complex<double> > col);
    // circular shift in row or column direction
    void rowCyclicShift(int r);
    void colCyclicShift(int c);
    // fftshift
    matrix fftshift();
};


#endif //MATRIX_H
