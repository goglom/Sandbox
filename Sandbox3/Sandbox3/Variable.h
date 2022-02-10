#pragma once
#include <vector>
#include <functional>
using DerivVector = std::vector<double>;

class Variable {
public:
  Variable();
  Variable(double iVal, std::vector<double> iDeriv);

  Variable operator-() const;
  friend Variable operator-(Variable&& iVar);

  Variable& operator+=(const Variable& iVar);
  Variable& operator+=(double iNumber);

  Variable& operator-=(const Variable& iVar);
  Variable& operator-=(double iNumber);

  Variable& operator*=(const Variable& iVar);
  Variable& operator*=(double iNumber);

  Variable& operator/=(const Variable& iVar);
  Variable& operator/=(double iNumber);

  Variable operator+(const Variable& iVar) const;
  Variable operator+(Variable&& iVar) const;
  Variable operator+(double iNumber) const;

  friend Variable operator+(double iNumber, const Variable& iVar);
  friend Variable operator+(double iNumber, Variable&& iVar);
  friend Variable operator+(Variable&& iLshVar, const Variable& iRshVal);

  Variable operator-(const Variable& iVar) const;
  Variable operator-(Variable&& iVar) const;
  Variable operator-(double iNumber) const;

  friend Variable operator-(double iNumber, const Variable& iVar);
  friend Variable operator-(double iNumber, Variable&& iVar);
  friend Variable operator-(Variable&& iLshVar, const Variable& iRshVal);

  Variable operator*(const Variable& iVar) const;
  Variable operator*(Variable&& iVar) const;
  Variable operator*(double iNumber) const;

  Variable operator/(const Variable& iVar) const;
  Variable operator/(Variable&& iVar) const;
  Variable operator/(double iNumber) const;

private:
  double _val;
  DerivVector _derivatives;


  typedef std::function<double(double, double)> BinaryFunction;
  static void Apply(const Variable& iLsh, const Variable& iRsh, Variable& oRes, const BinaryFunction& iValFunc, const BinaryFunction& iDerivFunc);

};
