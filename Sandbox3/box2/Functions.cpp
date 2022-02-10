#include "Functions.h"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <utility>

DoubleVector EvaluableFunction::Derive(const DoubleVector& iVars) const
{
  DoubleVector derivatives(iVars.size(), 0.0);
  for (auto it = _dependentIndexes.begin(); it != _dependentIndexes.end(); ++it)
  {
    derivatives[*it] = Derive(*it, iVars);
  }
  return derivatives;
}

const IndexesSet& EvaluableFunction::GetDependentIndexes() const
{
  return _dependentIndexes;
}

double VariableFunction::Eval(const DoubleVector& iVars) const
{
  return iVars[_ind];
}

double VariableFunction::Derive(size_t iInd, const DoubleVector& iVars) const
{
  return _ind == iInd ? 1.0 : 0.0;
}

VariableFunction::VariableFunction(size_t iInd)
  : _ind(iInd)
{
  _dependentIndexes.insert(iInd);
}


double ConstantFunction::Eval(const DoubleVector& iVars) const
{
  return _value;
}

double ConstantFunction::Derive(size_t iInd, const DoubleVector& iVars) const
{
  return 0.0;
}

ConstantFunction::ConstantFunction(double iValue)
  : _value(iValue)
{}


double UnaryOperator::Eval(const DoubleVector& iVars) const
{
  return _evalFunc(_arg->Eval(iVars));
}

double UnaryOperator::Derive(size_t iInd, const DoubleVector& iVars) const
{
  return _deriveFunc(_arg->Eval(iVars), _arg->Derive(iInd, iVars));
}

UnaryOperator::UnaryOperator(EvaluableFunctionPtr iArg, EvalFunction iEvalFunc, DeriveFunction iDeriveFunc)
  : _arg(std::move(iArg)), _evalFunc(std::move(iEvalFunc)), _deriveFunc(std::move(iDeriveFunc))
{
  _dependentIndexes = _arg->GetDependentIndexes();
}


double BinaryOperator::Eval(const DoubleVector& iVars) const
{
  return _evalFunc(_lsh->Eval(iVars), _rsh->Eval(iVars));
}

double BinaryOperator::Derive(size_t iInd, const DoubleVector& iVars) const
{
  return _deriveFunc(_lsh->Eval(iVars), _lsh->Derive(iInd, iVars), _rsh->Eval(iVars), _rsh->Derive(iInd, iVars));
}

BinaryOperator::BinaryOperator(EvaluableFunctionPtr iLsh, EvaluableFunctionPtr iRsh, EvalFunction iEvalFunc, DeriveFunction iDeriveFunc)
  :  _lsh(std::move(iLsh)), _rsh(std::move(iRsh)), _evalFunc(std::move(iEvalFunc)), _deriveFunc(std::move(iDeriveFunc))
{
  const IndexesSet& lDep = _lsh->GetDependentIndexes();
  const IndexesSet& rDep = _rsh->GetDependentIndexes();

  std::merge(lDep.begin(), lDep.end(), rDep.begin(), rDep.end(),
   std::inserter(_dependentIndexes, _dependentIndexes.begin()));
}


EvaluableFunctionPtr operator+(EvaluableFunctionPtr iLsh, EvaluableFunctionPtr iRsh)
{
  return BinaryOperator::Construct(std::move(iLsh), std::move(iRsh),
   [](double lVal, double rVal) -> double {
     return lVal + rVal;
   },
   [](double, double lDeriv, double, double rDeriv) {
     return lDeriv + rDeriv;
   }
);
}

EvaluableFunctionPtr operator-(EvaluableFunctionPtr iLsh, EvaluableFunctionPtr iRsh)
{
  return BinaryOperator::Construct(std::move(iLsh), std::move(iRsh),
   [](double lVal, double rVal) -> double {
     return lVal - rVal;
   },
   [](double, double lDeriv, double, double rDeriv) {
     return lDeriv - rDeriv;
   }
);
}

EvaluableFunctionPtr operator*(EvaluableFunctionPtr iLsh, EvaluableFunctionPtr iRsh)
{
  return BinaryOperator::Construct(std::move(iLsh), std::move(iRsh),
    [](double lVal, double rVal) -> double {
     return lVal * rVal;
    },
    [](double lVal, double lDeriv, double rVal, double rDeriv) {
     return lDeriv * rVal + lVal * rDeriv;
    }
  );
}

EvaluableFunctionPtr operator/(EvaluableFunctionPtr iLsh, EvaluableFunctionPtr iRsh)
{
  return BinaryOperator::Construct(std::move(iLsh), std::move(iRsh),
   [](double lVal, double rVal) -> double {
     return lVal / rVal;
   },
   [](double lVal, double lDeriv, double rVal, double rDeriv) {
     return lDeriv / rVal - lVal * rDeriv / (rVal * rVal);
   }
  );
}

EvaluableFunctionPtr operator+(EvaluableFunctionPtr lsh, double rsh)
{
  return std::move(lsh) + ConstantFunction::Construct(rsh);
}

EvaluableFunctionPtr operator-(EvaluableFunctionPtr lsh, double rsh)
{
  return std::move(lsh) - ConstantFunction::Construct(rsh);
}

EvaluableFunctionPtr operator*(EvaluableFunctionPtr lsh, double rsh)
{
  return std::move(lsh) * ConstantFunction::Construct(rsh);
}

EvaluableFunctionPtr operator/(EvaluableFunctionPtr lsh, double rsh)
{
  return std::move(lsh) / ConstantFunction::Construct(rsh);
}

EvaluableFunctionPtr Abs(EvaluableFunctionPtr iArg)
{
  return UnaryOperator::Construct(std::move(iArg),
    [](double val) -> double
    {
      return std::abs(val);
    },
    [](double val, double deriv) -> double
    {
      return  val > 0 ? deriv : -deriv;
    }
  );
}

EvaluableFunctionPtr Sqrt(EvaluableFunctionPtr iArg)
{
  return UnaryOperator::Construct(std::move(iArg),
    [](double val) -> double
    {
      return std::sqrt(val);
    },
    [](double val, double deriv) -> double
    {
      return  deriv / (2 * std::sqrt(val));
    }
  );
}

EvaluableFunctionPtr Pow(EvaluableFunctionPtr iArg, double iDegree)
{
  return UnaryOperator::Construct(std::move(iArg),
    [iDegree](double val) -> double
    {
      return std::pow(val, iDegree);
    },
    [iDegree](double val, double deriv) -> double
    {
      return  iDegree * std::pow(val, iDegree - 1.0) * deriv;
    }
  );
}

EvaluableFunctionPtr Sin(EvaluableFunctionPtr iArg)
{
  return UnaryOperator::Construct(std::move(iArg),
    [](double val) -> double
    {
      return std::sin(val);
    },
    [](double val, double deriv) -> double
    {
      return std::cos(val) * deriv;
    }
  );
}

EvaluableFunctionPtr Cos(EvaluableFunctionPtr iArg)
{
  return UnaryOperator::Construct(std::move(iArg),
    [](double val) -> double
    {
      return std::cos(val);
    },
    [](double val, double deriv) -> double
    {
      return -std::sin(val) * deriv;
    }
  );
}
