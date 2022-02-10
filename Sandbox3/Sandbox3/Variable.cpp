#include "Variable.h"

#include <algorithm>

#include <stdexcept>

namespace {
  void Transform(const DerivVector& iVec, DerivVector& oVec, const std::function<double(double)>& iFunc) {
    std::transform(iVec.begin(), iVec.end(), oVec.begin(), iFunc);
  }

  void Transform(const DerivVector& iVec1, const DerivVector& iVec2, DerivVector& oVec, const std::function<double(double, double)>& iFunc) {
    std::transform(iVec1.begin(), iVec1.end(), iVec2.begin(), oVec.begin(), iFunc);
  }

  struct MultiplyDerivative {
    MultiplyDerivative(double iLeftVal, double iRightVal)
      : leftVal(iLeftVal), rightVal(iRightVal)
    {}
    double leftVal;
    double rightVal;

    double operator()(double iLeftDeriv, double iRightDeriv) {
      return leftVal * iRightDeriv + iLeftDeriv * rightVal;
    }
  };

  struct DivisionDerivative {
    DivisionDerivative(double iLeftVal, double iRightVal)
      : factor(-iLeftVal / (iRightVal * iRightVal)), rightVal(iRightVal)
    {}
    double factor;
    double rightVal;

    double operator()(double iLeftDeriv, double iRightDeriv) {
      return iLeftDeriv / rightVal + factor * iRightDeriv;
    }
  };
}

void Variable::Apply(const Variable& iLsh, const Variable& iRsh, Variable& oRes, const BinaryFunction& iValFunc, const BinaryFunction& iDerivFunc)
{
  oRes._val = iValFunc(iLsh._val, iRsh._val);
  Transform(iLsh._derivatives, iRsh._derivatives, oRes._derivatives, iDerivFunc);
}

Variable::Variable()
  : _val(0)
{
}

Variable::Variable(double iVal, std::vector<double> iDeriv)
  : _val(iVal), _derivatives(std::move(iDeriv))
{
}

Variable Variable::operator-() const
{
  Variable result(*this);
  Transform(_derivatives, result._derivatives, [](double val) { return -val; });
  return result;
}

Variable& Variable::operator+=(const Variable& iVar)
{
  auto plus = std::plus<>();
  Apply(*this, iVar, *this, plus, plus);
  return *this;
}

Variable& Variable::operator+=(double iNumber)
{
  _val += iNumber;
  return *this;
}

Variable& Variable::operator-=(const Variable& iVar)
{
  auto plus = std::plus<>();
  Apply(*this, iVar, *this, plus, plus);
  return *this;
}

Variable& Variable::operator-=(double iNumber)
{
  _val -= iNumber;
  return *this;
}

Variable& Variable::operator*=(const Variable& iVar)
{
  Apply(*this, iVar, *this, std::multiplies<>(), MultiplyDerivative(_val, iVar._val));
  return *this;
}

Variable& Variable::operator*=(double iNumber)
{
  _val *= iNumber;
  Transform(_derivatives, _derivatives, [&iNumber](double val) { return val * iNumber; });
  return *this;
}

Variable& Variable::operator/=(const Variable& iVar)
{
  Apply(*this, iVar, *this, std::divides<>(), DivisionDerivative(_val, iVar._val));
  return *this;
}

Variable& Variable::operator/=(double iNumber)
{
  _val /= iNumber;
  Transform(_derivatives, _derivatives, [&iNumber](double val) { return val / iNumber; });
  return *this;
}

Variable Variable::operator+(const Variable& iVar) const
{
  Variable result(*this);
  auto plus = std::plus<>();
  Apply(result, iVar, result, plus, plus);
  return result;
}

Variable Variable::operator+(Variable&& iVar) const
{
  iVar += *this;
  return std::move(iVar);
}

Variable Variable::operator+(double iNumber) const
{
  Variable result(*this);
  result += iNumber;
  return result;
}

Variable Variable::operator-(const Variable& iVar) const
{
  Variable result(*this);
  result -= iVar;
  return result;
}

Variable Variable::operator-(Variable&& iVar) const
{
  iVar._val = _val - iVar._val;
  Transform(_derivatives, iVar._derivatives, iVar._derivatives, std::minus<>());
  return std::move(iVar);
}

Variable Variable::operator-(double iNumber) const
{
  Variable result(*this);
  result -= iNumber;
  return result;
}

Variable Variable::operator*(const Variable& iVar) const
{
  Variable result(*this);
  result *= iVar;
  return result;
}

Variable Variable::operator*(Variable&& iVar) const
{
  iVar *= *this;
  return std::move(iVar);
}

Variable Variable::operator*(double iNumber) const
{
  Variable result(*this);
  result *= iNumber;
  return result;
}

Variable Variable::operator/(const Variable& iVar) const
{
  Variable result(*this);
  result /= iVar;
  return result;
}

Variable Variable::operator/(Variable&& iVar) const
{
  Transform(_derivatives, iVar._derivatives, iVar._derivatives, DivisionDerivative(_val, iVar._val));
  iVar._val = _val / iVar._val;
  return std::move(iVar);
}

Variable Variable::operator/(double iNumber) const
{
  Variable result(*this);
  result /= iNumber;
  return result;
}





Variable operator-(Variable&& iVar)
{
  iVar._val = -iVar._val;
  Transform(iVar._derivatives, iVar._derivatives, [](double val) { return -val; });
  return std::move(iVar);
}

Variable operator+(double iNumber, const Variable& iVar)
{
  return iVar + iNumber;
}

Variable operator+(double iNumber, Variable&& iVar)
{
  iVar += iNumber;
  return std::move(iVar);
}

Variable operator+(Variable&& iLshVar, const Variable& iRshVal)
{
  return iRshVal.operator+(std::move(iLshVar));
}

Variable operator-(double iNumber, const Variable& iVar)
{
  Variable result(iNumber - iVar._val, iVar._derivatives);
  Transform(iVar._derivatives, result._derivatives, [](double val){ return -val; });
  return result;
}

Variable operator-(double iNumber, Variable&& iVar)
{
  iVar._val = iNumber - iVar._val;
  Transform(iVar._derivatives, iVar._derivatives, [](double val) { return -val; });
  return std::move(iVar);
}

Variable operator-(Variable&& iLshVar, const Variable& iRshVal)
{
  iLshVar -= iRshVal;
  return std::move(iLshVar);
}
