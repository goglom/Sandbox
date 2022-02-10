#pragma once

#include <functional>
#include <vector>
#include <set>

class Function;
using FunctionVector = std::vector<Function>;
using DoubleVector = std::vector<double>;
using IndexesSet = std::set<size_t>;

class Function {
public:
  enum Type {
    Variable = 0,
    Constant,
    Summation,
    Multiplication,
    Negate,
    Absolute,
    SquareRoot,
    Power,
    Sine,
    Cosine,
    Tangent,
    Exponent,
  };
  using EvalFunction = std::function<double(const DoubleVector&, const FunctionVector&)>;
  using DeriveFunction = std::function<double(size_t, const DoubleVector&, const FunctionVector&)>;

  Function(double iValue);
  Function(size_t iInd);
  inline double Eval(const DoubleVector& iVars) const;
  DoubleVector Derive(const DoubleVector& iVars) const;
  inline double DeriveByIndVar(size_t iIndex, const DoubleVector& iVars) const;

  friend Function operator+(Function iLsh, Function iRsh);
  friend Function operator-(Function iLsh, Function iRsh);
  friend Function operator*(Function iLsh, Function iRsh);
  friend Function operator/(Function iLsh, Function iRsh);

  friend Function operator-(Function iArg);

  friend Function Abs(Function iArg);
  friend Function Sqrt(Function iArg);
  friend Function Pow(Function iArg, double iDegree);
  friend Function Sin(Function iArg);
  friend Function Cos(Function iArg);
  friend Function Tan(Function iArg);
  friend Function Exp(Function iArg);

  const Type type;

private:
  Function(EvalFunction iEvalFunc, DeriveFunction iDeriveFunc, Type iType, FunctionVector iArgs = FunctionVector());

  EvalFunction _evalFunc;
  DeriveFunction _deriveFunc;
  FunctionVector _arguments;
  IndexesSet _dependentIndexes;
};

inline double Function::DeriveByIndVar(size_t iInd, const DoubleVector& iVars) const
{
  return _deriveFunc(iInd, iVars, _arguments);
}

inline double Function::Eval(const DoubleVector& iVars) const
{
  return _evalFunc(iVars, _arguments);
}

