#include "Function.h"

#define _VARS_NAME_ iVars
#define _INDEX_NAME_ iInd
#define _ARGS_NAME_ iArgs

#define MAKE_EVAL_LAMBDA(capture, functionBody) \
  [capture](const DoubleVector& _VARS_NAME_, const FunctionVector& _ARGS_NAME_)->double{functionBody}

#define MAKE_DERIVE_LAMBDA(capture, functionBody) \
  [capture](size_t _INDEX_NAME_, const DoubleVector& _VARS_NAME_, const FunctionVector& _ARGS_NAME_)->double{functionBody}

#define VAL(index) _ARGS_NAME_[(index)].Eval(_VARS_NAME_)
#define DERIV(index) _ARGS_NAME_[(index)].DeriveByIndVar(_INDEX_NAME_, _VARS_NAME_)

Function::Function(EvalFunction iEvalFunc, DeriveFunction iDeriveFunc, FunctionVector iArgs)
  : _evalFunc(std::move(iEvalFunc)), _deriveFunc(std::move(iDeriveFunc)), _arguments(std::move(iArgs))
{
  for (auto it = _arguments.begin(); it != _arguments.end(); ++it) {
    _dependentIndexes.insert(it->_dependentIndexes.begin(), it->_dependentIndexes.end());
  }
}

Function Function::MakeVariableFunction( size_t iIndex)
{
  Function res(
    MAKE_EVAL_LAMBDA(iValue, return iValue;),
    MAKE_DERIVE_LAMBDA(iIndex, return iInd == iIndex ? 1.0 : 0.0;)
  );
  res._dependentIndexes.insert(iIndex);
  return res;
}

Function Function::MakeConstantFunction(double iValue)
{
  return Function(
    MAKE_EVAL_LAMBDA(iValue, return iValue;),
    MAKE_DERIVE_LAMBDA(, return 0.0;)
  );
}

Function operator+(Function iLsh, Function iRsh)
{
  FunctionVector args;
  args.push_back(std::move(iLsh));
  args.push_back(std::move(iRsh));

  return Function(
    MAKE_EVAL_LAMBDA(, return VAL(0) + VAL(1);),
    MAKE_DERIVE_LAMBDA(, return DERIV(0) + DERIV(1);),
    std::move(args)
  );
}

Function operator+(Function lsh, double rsh)
{
  FunctionVector args;
  args.push_back(std::move(lsh));
  return Function(
    MAKE_EVAL_LAMBDA(rsh, return VAL(0) + rsh;),
    MAKE_DERIVE_LAMBDA(, return DERIV(0);),
    std::move(args)
  );
}

Function operator-(Function lsh, double rsh)
{
  FunctionVector args;
  args.push_back(std::move(lsh));
  return Function(
    MAKE_EVAL_LAMBDA(rsh, return VAL(0) - rsh;),
    MAKE_DERIVE_LAMBDA(, return DERIV(0);),
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
    std::move(args)
  );
}

double Function::Eval(const DoubleVector& iVars) const
{
  return _evalFunc(iVars, _arguments);
}

DoubleVector Function::Derive(const DoubleVector& iVars) const
{
  DoubleVector derivatives(iVars.size(), 0.0);
  for (auto it = _dependentIndexes.begin(); it != _dependentIndexes.end(); ++it) {
    derivatives[*it] = DeriveByIndVar(*it, iVars);
  }
  return derivatives;
}

double Function::DeriveByIndVar(size_t iInd, const DoubleVector& iVars) const
{
  return _deriveFunc(iInd, iVars, _arguments);
}
