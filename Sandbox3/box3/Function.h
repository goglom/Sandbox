#pragma once

#include <functional>
#include <vector>
#include <set>

class Function;
using FunctionVector = std::vector<Function>;
using DoubleVector = std::vector<double>;
using IndexesSet = std::set<size_t>;

class Function
{
public:
  using EvalFunction = std::function<double(const DoubleVector&, const FunctionVector&)>;
  using DeriveFunction = std::function<double(size_t, const DoubleVector&, const FunctionVector&)>;

  static Function MakeVariableFunction(size_t iInd);
  static Function MakeConstantFunction(double iValue);

  friend Function operator+(Function iLsh, Function iRsh);
  friend Function operator+(Function iLsh, double iRsh);
  friend Function operator-(Function iLsh, double iRsh);
  friend Function operator*(Function iLsh, Function iRsh);

  double Eval(const DoubleVector& iVars) const;
  DoubleVector Derive(const DoubleVector& iVars) const;

private:
  Function(EvalFunction iEvalFunc, DeriveFunction iDeriveFunc, FunctionVector iArgs = FunctionVector());
  double DeriveByIndVar(size_t iIndex, const DoubleVector& iVars) const;

  EvalFunction _evalFunc;
  DeriveFunction _deriveFunc;
  FunctionVector _arguments;
  IndexesSet _dependentIndexes;
};