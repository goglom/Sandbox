#include <iostream>
#include <vector>

#include "A.h"

template<class T>
class Matrix {
public:

  Matrix() = default;

  Matrix(size_t iRows, size_t iCols, const T& iFillValue = {})
    : _data(iRows * iCols, iFillValue), _rows(iRows), _cols(iCols)
  {
  }

  T& operator()(size_t iRow, size_t iCol)
  {
    return _data[iRow * _cols + iCol];
  }

  const T& operator()(size_t iRow, size_t iCol) const
  {
    return _data[iRow * _cols + iCol];
  }

  size_t Size() const
  {
    return _rows * _cols;
  }

  size_t Rows() const
  {
    return _rows;
  }

  size_t Cols() const
  {
    return _cols;
  }

private:
  std::vector<T> _data = {};
  size_t         _rows = 0;
  size_t         _cols = 0;
};


int main()
{
  Matrix<int> a(3, 4);

  int counter = 0;
  for (size_t i = 0; i < a.Rows(); ++i) {
    for (size_t j = 0; j < a.Cols(); ++j) {
      a(i, j) = counter;
      ++counter;
    }
  }

  for (size_t i = 0; i < a.Rows(); ++i) {
    for (size_t j = 0; j < a.Cols(); ++j) {

      std::cout << a(i, j) << '\t';
    }
    std::cout << '\n';
  }

  A aaa;

  aaa.foo();

  return 0;
}