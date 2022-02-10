#include "Function.h"

#include <cmath>

#define _VARS_NAME_ iVars
#define _INDEX_NAME_ iInd
#define _ARGS_NAME_ iArgs

#define MAKE_EVAL_LAMBDA(capture, functionBody) \
  [capture](const DoubleVector& _VARS_NAME_, const FunctionVector& _ARGS_NAME_)->double{functionBody}

#define MAKE_DERIVE_LAMBDA(capture, functionBody) \
  [capture](size_t _INDEX_NAME_, const DoubleVector& _VARS_NAME_, const FunctionVector& _ARGS_NAME_)->double{functionBody}

#define VAL(index) _ARGS_NAME_[(index)].Eval(_VARS_NAME_)
#define DERIV(index) _ARGS_NAME_[(index)].DeriveByIndVar(_INDEX_NAME_, _VARS_NAME_)


Function::Function(EvalFunction iEvalFunc, DeriveFunction iDeriveFunc, Type iType, FunctionVector iArgs)
  : type(iType), _evalFunc(std::move(iEvalFunc)), _deriveFunc(std::move(iDeriveFunc)), _arguments(std::move(iArgs))
{
  for (auto it = _arguments.begin(); it != _arguments.end(); ++it) {
    _dependentIndexes.insert(it->_dependentIndexes.begin(), it->_dependentIndexes.end());
  }
}

Function operator+(Function iLsh, Function iRsh)
{
  FunctionVector args;
  args.push_back(std::move(iLsh));
  args.push_back(std::move(iRsh));

  return Function(
    MAKE_EVAL_LAMBDA(, return VAL(0) + VAL(1);),
    MAKE_DERIVE_LAMBDA(, return DERIV(0) + DERIV(1);),
    Function::Summation,
    std::move(args)
  );
}

Function operator-(Function iLsh, Function iRsh)
{
  FunctionVector args;
  args.push_back(std::move(iLsh));
  args.push_back(std::move(iRsh));

  return Function(
    MAKE_EVAL_LAMBDA(, return VAL(0) - VAL(1);),
    MAKE_DERIVE_LAMBDA(, return DERIV(0) - DERIV(1);),
    Function::Summation,
    std::move(args)
  );
}

Function operator*(Function lsh, Function rsh)
{
  FunctionVector args;
  args.push_back(std::move(lsh));
  args.push_back(std::move(rsh));

  return Function(
    MAKE_EVAL_LAMBDA(, return VAL(1) * VAL(0);),
    MAKE_DERIVE_LAMBDA(, return DERIV(0) * VAL(1) + VAL(0) * DERIV(1);),
    Function::Multiplication,
    std::move(args)
  );
}

Function operator/(Function iLsh, Function iRsh)
{
  FunctionVector args;
  args.push_back(std::move(iLsh));
  args.push_back(std::move(iRsh));

  return Function(
    MAKE_EVAL_LAMBDA(, return VAL(1) / VAL(0);),
    MAKE_DERIVE_LAMBDA(,
      double rVal = VAL(1);
      return DERIV(0) / VAL(1) + VAL(0) * DERIV(1) / (rVal * rVal);
    ),
    Function::Multiplication,
    std::move(args)
  );
}

Function operator-(Function iArg)
{
  FunctionVector args;
  args.push_back(std::move(iArg));

  return Function(
    MAKE_EVAL_LAMBDA(, return -VAL(0);),
    MAKE_DERIVE_LAMBDA(,return -DERIV(0);),
    Function::Negate,
    std::move(args)
  );
}

Function Abs(Function iArg)
{
  FunctionVector args;
  args.push_back(std::move(iArg));

  return Function(
    MAKE_EVAL_LAMBDA(, return -VAL(0);),
    MAKE_DERIVE_LAMBDA(, return -DERIV(0);),
    Function::Absolute,
    std::move(args)
  );
}

Function Sqrt(Function iArg)
{
  FunctionVector args;
  args.push_back(std::move(iArg));

  return Function(
    MAKE_EVAL_LAMBDA(, return std::sqrt(VAL(0));),
    MAKE_DERIVE_LAMBDA(, return DERIV(0) / (2 * std::sqrt(VAL(0)));),
    Function::SquareRoot,
    std::move(args)
  );
}

Function Pow(Function iArg, double iDegree)
{
  FunctionVector args;
  args.push_back(std::move(iArg));

  return Function(
    MAKE_EVAL_LAMBDA(iDegree, return std::pow(VAL(0), iDegree);),
    MAKE_DERIVE_LAMBDA(iDegree, return iDegree * std::pow(VAL(0), iDegree - 1) * DERIV(0);),
    Function::Power,
    std::move(args)
  );
}

Function Sin(Function iArg)
{
  FunctionVector args;
  args.push_back(std::move(iArg));

  return Function(
    MAKE_EVAL_LAMBDA(, return std::sin(VAL(0));),
    MAKE_DERIVE_LAMBDA(, return DERIV(0) * std::cos(VAL(0));),
    Function::Sine,
    std::move(args)
  );
}

Function Cos(Function iArg)
{
  FunctionVector args;
  args.push_back(std::move(iArg));

  return Function(
    MAKE_EVAL_LAMBDA(, return std::cos(VAL(0));),
    MAKE_DERIVE_LAMBDA(, return -DERIV(0) * std::sin(VAL(0));),
    Function::Cosine,
    std::move(args)
  );
}

Function Tan(Function iArg)
{
  FunctionVector args;
  args.push_back(std::move(iArg));

  return Function(
    MAKE_EVAL_LAMBDA(, return std::tan(VAL(0));),
    MAKE_DERIVE_LAMBDA(,
      double cos = std::cos(VAL(0));
      return DERIV(0) / (cos  * cos);
    ),
    Function::Tangent,
    std::move(args)
  );
}

Function Exp(Function iArg)
{
  FunctionVector args;
  args.push_back(std::move(iArg));

  return Function(
    MAKE_EVAL_LAMBDA(, return std::exp(VAL(0));),
    MAKE_DERIVE_LAMBDA( , return DERIV(0) * std::exp(VAL(0));),
    Function::Exponent,
    std::move(args)
  );
}

Function::Function(double iValue)
  : Function(
      MAKE_EVAL_LAMBDA(iValue, return iValue;),
      MAKE_DERIVE_LAMBDA(, return 0.0;), 
      Constant
    )
{}

Function::Function(size_t iIndex)
  : Function(
      MAKE_EVAL_LAMBDA(iIndex, return iVars[iIndex];),
      MAKE_DERIVE_LAMBDA(iIndex, return iInd == iIndex ? 1.0 : 0.0;),
      Variable
    )
{
  _dependentIndexes.insert(iIndex);
}

DoubleVector Function::Derive(const DoubleVector& iVars) const
{
  DoubleVector derivatives(iVars.size(), 0.0);
  for (auto it = _dependentIndexes.begin(); it != _dependentIndexes.end(); ++it) {
    derivatives[*it] = DeriveByIndVar(*it, iVars);
  }
  return derivatives;
}

