#pragma once

#include <vector>
#include <cmath>

class Variable;
typedef std::vector<Variable> VariableVector;
typedef std::vector<double> DerivVector;

class Variable {
public:
  Variable(double iVal = 0.0);
  Variable(double iVal, std::vector<double> iDeriv);

  double GetValue() const;
  double& GetValue();
  const DerivVector& GetDerivatives() const;
  DerivVector& GetDerivatives();
  Variable operator+(const Variable& iVar) const;
  Variable operator+(double iNumber) const;
  Variable operator-(const Variable& iVar) const;
  Variable operator-(double iNumber) const;
  Variable operator-() const;
  Variable operator*(const Variable& iVar) const;
  Variable operator*(double iNumber) const;
  Variable operator/(const Variable& iVar) const;
  Variable operator/(double iNumber) const;
  Variable& operator+=(const Variable& iVar);
  Variable& operator-=(const Variable& iVar);
  Variable& operator*=(double iNumber);
  Variable& operator/=(double iNumber);

  friend Variable operator+(Variable&& iVar1, Variable&& iVar2);
  friend Variable operator-(Variable&& iVar1, Variable&& iVar2);
  friend Variable operator+(Variable&& iVar, double iNumber);
  friend Variable operator+(double iNumber, Variable&& iVar);
  friend Variable operator-(Variable&& iVar, double iNumber);
  friend Variable operator-(double iNumber, Variable&& iVar);
  friend Variable operator*(Variable&& iVar, double iNumber);
  friend Variable operator*(double iNumber, Variable&& iVar);
  friend Variable operator/(Variable&& iVar, double iNumber);
  friend Variable operator/(double iNumber, Variable&& iVar);

  friend Variable operator+(double iNumber, const Variable& iVar);
  friend Variable operator-(double iNumber, const Variable& iVar);
  friend Variable operator*(double iNumber, const Variable& iVar);
  friend Variable operator/(double iNumber, const Variable& iVar);
  friend Variable abs(const Variable& iVar);
  friend Variable sqrt(const Variable& iVar);
  friend Variable exp(const Variable& iVar);
  friend Variable log(const Variable& iVar);
  friend Variable pow(const Variable& iVar, double iDegree);
  friend Variable cos(const Variable& iVar);
  friend Variable sin(const Variable& iVar);
  friend Variable tan(const Variable& iVar);
  friend Variable arccos(const Variable& iVar);
  //virtual ~Variable();

private:
  double _val;
  DerivVector _derivatives;
};
