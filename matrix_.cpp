#include <iostream>
#include <vector>
#include "matrix_.h"

using namespace std;


matrix::matrix(size_t r, size_t c) : 
	rows_(r), cols_(c),t_(0),
	startRow_(0), startCol_(0)
{
	data_ = new complex<double>*[r];
	for(size_t i=0;i<r;++i){
		data_[i] = new complex<double>[c];
	}
}

matrix::matrix(size_t r, size_t c, const complex<double>& init) :
	rows_(r), cols_(c),t_(0),
	startRow_(0), startCol_(0)
{
	data_ = new complex<double>*[r];
	for(size_t i=0; i<r; ++i){
		data_[i] = new complex<double>[c];
		for(size_t j=0; j<c; ++j) {
			data_[i][j] = init;
		}
	}
}

matrix::matrix(const matrix& mat) :
	rows_(mat.rows_), cols_(mat.cols_),t_(mat.t_),
	startRow_(mat.startRow_), startCol_(mat.startCol_)
{
	data_ = new complex<double>*[rows_];
	for(size_t i=0; i<rows_; ++i){
		data_[i] = new complex<double>[cols_];
		for(size_t j=0; j<cols_; ++j) {
			data_[i][j] = mat.data_[i][j];
		}
	}
}

matrix& matrix::operator=(matrix mat) {
	swap(mat);
	return *this;
}

matrix::~matrix() {
	if(0==t_)
	{
	for(size_t i=0; i<rows_; ++i) {
		delete [] data_[i];
	}
	}
	else
	{for(size_t i=0; i<cols_; ++i) {
		delete [] data_[i];
	}
	}

	delete [] data_;
}

complex<double>& matrix::operator()(size_t r, size_t c) {
	return getElem(r, c);
}

const complex<double>& matrix::operator()(size_t r, size_t c) const{
	return const_cast<const complex<double>&>(getElem(r, c));
}

ostream& operator<<(ostream& out, const matrix& rhs) {
	for(size_t i=0; i<rhs.rows_; ++i){
		for(size_t j=0; j<rhs.cols_; ++j){
			out << rhs(i,j) << " ";
		}
		out << endl;
	}
	return out;
}

matrix& matrix::T() {
	t_ =!t_;
	size_t temp = rows_;
	rows_ = cols_;
	cols_ = temp;
	return *this;
}

size_t matrix::Rows() const {
	return rows_;
}

size_t matrix::Cols() const {
	return cols_;
}

vector<complex<double> > matrix::getRow(int r) const {
	vector<complex<double> > ret;
	if(!checkRange(r,0)){
		throw range_error("in getRow, out of range!");
	}
	for(size_t i=0;i<cols_;++i)
	{
		ret.push_back(operator()(r,i));
	}
	return ret;
}

vector<complex<double> > matrix::getCol(int c) const {
		vector<complex<double> > ret;
	if(!checkRange(0,c)){
		throw range_error("in getCol, out of range!");
	}
	for(size_t i=0;i<rows_;++i)
	{
		ret.push_back(operator()(i,c));
	}
	return ret;
}


bool matrix::setRow(int r, vector<complex<double> > row) {
	if(!checkRange(r,0) || row.size() !=cols_){
		throw range_error("in getRow, out of range!");
	}
	for(size_t i=0;i<cols_;++i)
	{
		operator()(r,i) =row[i];
	}
	return true;
}

bool matrix::setCol(int c, vector<complex<double> > col){
		if(!checkRange(0,c) || col.size() !=rows_){
		throw range_error("in getCol, out of range!");
	}
	for(size_t i=0;i<rows_;++i)
	{
		operator()(i,c) =col[i];
	}
	return true;
}

void matrix::rowCyclicShift(int r) {
	startRow_ -=(r%rows_);
	if(startRow_ <0)  startRow_ +=rows_;
}

void matrix::colCyclicShift(int c) {
	startCol_ -=(c%cols_);
	if(startCol_ <0)   startCol_ +=cols_;
}

matrix matrix::fftshift() {
	rowCyclicShift(rows_/2);
	colCyclicShift(cols_/2);
	return *this;
}

bool matrix::checkRange(size_t r, size_t c) const {
	if(r<0 || r>=rows_ || c<0 || c>=cols_) {
		return false;
	}
	return true;
}

complex<double>& matrix::getElem(int r, int c) const{
	if (!checkRange(r,c)) {
		throw range_error("in getElem, row or col are out of range!");
	}
	if (0==t_) return data_[(r+startRow_)%rows_][(c+startCol_)%cols_];
	else return data_[(c+startCol_)%cols_][(r+startRow_)%rows_];
	
}

void matrix::swap(matrix &mat) {
    std::swap(data_, mat.data_);
    std::swap(rows_, mat.rows_);
    std::swap(cols_, mat.cols_);
    std::swap(t_, mat.t_);
    std::swap(startRow_, mat.startRow_);
    std::swap(startCol_, mat.startCol_);
}
