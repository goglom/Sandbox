#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <set>

class EvaluableFunction;
class VariableFunction;
class UnaryOperator;
class BinaryOperator;

using EvaluableFunctionPtr = std::shared_ptr<EvaluableFunction>;
using VariableFunctionPtr = std::shared_ptr<VariableFunction>;
using UnaryOperatorPtr = std::shared_ptr<UnaryOperator>;
using BinaryOperatorPtr = std::shared_ptr<BinaryOperator>;

using DoubleVector = std::vector<double>;
using IndexesSet = std::set<size_t>;

template<class Derived>
class SharedConstructable
{
public:
  template<class... Args>
  static std::shared_ptr<Derived> Construct(Args&&... args)
  {
    return std::shared_ptr<Derived>(new Derived(std::forward<Args>(args)...));
  }
};

class EvaluableFunction {
public:
  virtual ~EvaluableFunction() = default;

  virtual double Eval(const DoubleVector& iVars) const = 0;
  virtual double Derive(size_t iInd, const DoubleVector& iVars) const = 0;
  DoubleVector Derive(const DoubleVector& iVars) const;
  const IndexesSet& GetDependentIndexes() const;

protected:
  IndexesSet _dependentIndexes;
};

class VariableFunction : public EvaluableFunction, public SharedConstructable<VariableFunction> {
public:
  virtual double Eval(const DoubleVector& iVars) const;
  virtual double Derive(size_t iInd, const DoubleVector& iVars) const;

private:
  friend  SharedConstructable;
  VariableFunction(size_t iInd);

  size_t _ind;
};

class ConstantFunction : public EvaluableFunction, public SharedConstructable<ConstantFunction>
{
public:
  virtual double Eval(const DoubleVector& iVars) const;
  virtual double Derive(size_t iInd, const DoubleVector& iVars) const;

private:
  friend  SharedConstructable;
  ConstantFunction(double iValue);

  double _value;
};

class UnaryOperator : public EvaluableFunction, public SharedConstructable<UnaryOperator>
{
public:
  using EvalFunction = std::function<double(double)>;
  using DeriveFunction = std::function<double(double, double)>;

  virtual double Eval(const DoubleVector& iVars) const;
  virtual double Derive(size_t iInd, const DoubleVector& iVars) const;

private:
  friend SharedConstructable;
  UnaryOperator(EvaluableFunctionPtr iArg, EvalFunction iEvalFunc, DeriveFunction iDeriveFunc);

  EvaluableFunctionPtr _arg;
  EvalFunction _evalFunc;
  DeriveFunction _deriveFunc;
};

class BinaryOperator : public EvaluableFunction, public SharedConstructable<BinaryOperator> {
public:
  using EvalFunction = std::function<double(double, double)>;
  using DeriveFunction = std::function<double(double, double, double, double)>;

  virtual double Eval(const DoubleVector& iVars) const;
  virtual double Derive(size_t iInd, const DoubleVector& iVars) const;

private:
  friend SharedConstructable;
  BinaryOperator(EvaluableFunctionPtr iLsh, EvaluableFunctionPtr iRsh, EvalFunction iEvalFunc, DeriveFunction iDeriveFunc);

  EvaluableFunctionPtr _lsh, _rsh;
  EvalFunction _evalFunc;
  DeriveFunction _deriveFunc;
};

EvaluableFunctionPtr operator+(EvaluableFunctionPtr iLsh, EvaluableFunctionPtr iRsh);
EvaluableFunctionPtr operator-(EvaluableFunctionPtr iLsh, EvaluableFunctionPtr iRsh);
EvaluableFunctionPtr operator*(EvaluableFunctionPtr iLsh, EvaluableFunctionPtr iRsh);
EvaluableFunctionPtr operator/(EvaluableFunctionPtr iLsh, EvaluableFunctionPtr iRsh);

EvaluableFunctionPtr operator+(EvaluableFunctionPtr iLsh, double iRsh);
EvaluableFunctionPtr operator-(EvaluableFunctionPtr iLsh, double iRsh);
EvaluableFunctionPtr operator*(EvaluableFunctionPtr iLsh, double iRsh);
EvaluableFunctionPtr operator/(EvaluableFunctionPtr iLsh, double iRsh);


EvaluableFunctionPtr Abs(EvaluableFunctionPtr iArg);
EvaluableFunctionPtr Sqrt(EvaluableFunctionPtr iArg);
EvaluableFunctionPtr Pow(EvaluableFunctionPtr iArg, double iDegree);
EvaluableFunctionPtr Sin(EvaluableFunctionPtr iArg);
EvaluableFunctionPtr Cos(EvaluableFunctionPtr iArg);

