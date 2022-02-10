#include "Variable.h"

DerivVector operator+(const DerivVector& iVector1, const DerivVector& iVector2)
{
  size_t size = iVector1.size();
  DerivVector outVector(size);
  for (size_t i = 0; i < size; ++i) {
    outVector[i] = (iVector1[i] + iVector2[i]);
  }

  return outVector;
}

DerivVector& operator+=(DerivVector& iVector1, const DerivVector& iVector2)
{
  for (size_t i = 0; i < iVector1.size(); ++i) {
    iVector1[i] += iVector2[i];
  }
  return iVector1;
}

DerivVector& operator-=(DerivVector& iVector1, const DerivVector& iVector2)
{
  for (size_t i = 0; i < iVector1.size(); ++i) {
    iVector1[i] -= iVector2[i];
  }
  return iVector1;
}

DerivVector operator-(const DerivVector& iVector1, const DerivVector& iVector2)
{
  size_t size = iVector1.size();
  DerivVector outVector(size);
  for (size_t i = 0; i < size; ++i) {
    outVector[i] = (iVector1[i] - iVector2[i]);
  }

  return outVector;
}

DerivVector operator*(double iNumber, const DerivVector& iVector)
{
  size_t size = iVector.size();
  DerivVector outVector(size);
  for (size_t i = 0; i < size; ++i) {
    outVector[i] = (iNumber * iVector[i]);
  }

  return outVector;
}

DerivVector& operator*=(DerivVector& iVector, double iNumber)
{
  for (size_t i = 0; i < iVector.size(); ++i) {
    iVector[i] *= iNumber;
  }
  return iVector;
}

DerivVector& operator/=(DerivVector& iVector, double iNumber)
{
  for (size_t i = 0; i < iVector.size(); ++i) {
    iVector[i] /= iNumber;
  }
  return iVector;
}

Variable::Variable(double iVal) : _val(iVal)
{
}

Variable::Variable(double iVal, std::vector<double> iDeriv) : _val(iVal), _derivatives(std::move(iDeriv))
{
}

//Variable::~Variable()
//{
//}

double Variable::GetValue() const
{
  return _val;
}

double& Variable::GetValue()
{
  return _val;
}

const DerivVector& Variable::GetDerivatives() const
{
  return _derivatives;
}

std::vector<double>& Variable::GetDerivatives()
{
  return _derivatives;
}

Variable Variable::operator+(const Variable& iVar) const
{
  return Variable(_val + iVar.GetValue(), _derivatives + iVar.GetDerivatives());
}

Variable Variable::operator+(double iNumber) const
{
  return Variable(_val + iNumber, _derivatives);
}

Variable& Variable::operator+=(const Variable& iVar)
{
  _val += iVar._val;
  _derivatives += iVar._derivatives;
  return *this;
}

Variable& Variable::operator-=(const Variable& iVar)
{
  _val -= iVar._val;
  _derivatives -= iVar._derivatives;
  return *this;
}

Variable& Variable::operator*=(double iNumber)
{
  _val *= iNumber;
  _derivatives *= iNumber;
  return  *this;
}

Variable& Variable::operator/=(double iNumber)
{
  _val /= iNumber;
  _derivatives /= iNumber;
  return  *this;
}

Variable operator+(Variable&& iVar1, Variable&& iVar2)
{
  iVar1 += iVar2;
  return std::move(iVar1);
}

Variable operator-(Variable&& iVar1, Variable&& iVar2)
{
  iVar1 -= iVar2;
  return std::move(iVar1);
}

Variable operator+(Variable&& iVar, double iNumber)
{
  iVar._val += iNumber;
  return std::move(iVar);
}

Variable operator+(double iNumber, Variable&& iVar)
{
  return iVar + iNumber;
}

Variable operator-(Variable&& iVar, double iNumber)
{
  iVar._val -= iNumber;
  return  std::move(iVar);
}

Variable operator-(double iNumber, Variable&& iVar)
{
  iVar._val = iNumber - iVar._val;
  iVar.GetDerivatives() *= -1;
  return std::move(iVar);
}

Variable operator*(Variable&& iVar, double iNumber)
{
  iVar *= iNumber;
  return std::move(iVar);
}

Variable operator*(double iNumber, Variable&& iVar)
{
  return std::move(iVar *= iNumber);
}

Variable operator/(Variable&& iVar, double iNumber)
{
  iVar /= iNumber;
  return std::move(iVar);
}

Variable operator/(double iNumber, Variable&& iVar)
{
  double factor = -(iNumber / (iVar._val * iVar._val));
  iVar._val = iNumber / iVar._val;
  iVar._derivatives *= factor;
  return std::move(iVar);
}

Variable operator+(double iNumber, const Variable& iVar)
{
  return iVar + iNumber;
}

Variable Variable::operator-(const Variable& iVar) const
{
  return Variable(_val - iVar.GetValue(), _derivatives - iVar.GetDerivatives());
}

Variable Variable::operator-(double iNumber) const
{
  return Variable(_val - iNumber, _derivatives);
}

Variable Variable::operator-() const
{
  return Variable(-_val, (-1) * _derivatives);
}

Variable operator-(double iNumber, const Variable& iVar)
{
  return Variable(iVar._val - iNumber, (-1) * iVar._derivatives);
}

Variable Variable::operator*(const Variable& iVar) const
{
  return Variable(_val * iVar.GetValue(), iVar.GetValue() * _derivatives + _val * iVar.GetDerivatives());
}

Variable Variable::operator*(double iNumber) const
{
  return Variable(_val * iNumber, iNumber * _derivatives);
}

Variable operator*(double iNumber, const Variable& iVar)
{
  return iVar * iNumber;
}

Variable Variable::operator/(const Variable& iVar) const
{
  double factor = _val / (iVar.GetValue() * iVar.GetValue());
  return Variable(_val / iVar.GetValue(), 1 / iVar.GetValue() * _derivatives - factor * iVar.GetDerivatives());
}

Variable Variable::operator/(double iNumber) const
{
  return Variable(_val / iNumber, (1 / iNumber) * _derivatives);
}

Variable operator/(double iNumber, const Variable& iVar)
{
  double factor = -(iNumber / (iVar._val * iVar._val));
  return Variable(iNumber / iVar._val, factor * iVar._derivatives);
}

Variable abs(const Variable& iVar)
{
  return Variable(std::abs(iVar._val), iVar._val < 0 ? (-1) * iVar._derivatives : iVar._derivatives);
}

Variable sqrt(const Variable& iVar)
{
  double sqrtVal = std::sqrt(iVar._val);
  return Variable(sqrtVal, 1 / (2.0 * sqrtVal) * iVar._derivatives);
}

Variable exp(const Variable& iVar)
{
  double expVal = std::exp(iVar._val);
  return Variable(expVal, expVal * iVar._derivatives);
}

Variable log(const Variable& iVar)
{
  return Variable(std::log(iVar._val), (1 / iVar._val) * iVar._derivatives);
}

Variable pow(const Variable& iVar, double iDegree)
{
  double powVal1 = std::pow(iVar.GetValue(), iDegree);
  double powVal2 = std::pow(iVar.GetValue(), iDegree - 1.0);
  return Variable(powVal1, iDegree * powVal2 * iVar._derivatives);
}

Variable cos(const Variable& iVar)
{
  return Variable(std::cos(iVar._val), -std::sin(iVar._val) * iVar._derivatives);
}

Variable sin(const Variable& iVar)
{
  return Variable(std::sin(iVar._val), std::cos(iVar._val) * iVar._derivatives);
}

Variable tan(const Variable& iVar)
{
  double cosVal = std::cos(iVar._val);
  return Variable(std::tan(iVar._val), 1 / (cosVal * cosVal) * iVar._derivatives);
}

Variable arccos(const Variable& iVar)
{
  double arccosVal = std::acos(iVar._val);
  double factor = 1 / std::sqrt(1 - iVar._val * iVar._val);
  return Variable(arccosVal, -factor * iVar._derivatives);
}
