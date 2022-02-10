#include "Equation.h"

#include <stdexcept>


EquationPtr& Equation::GetArg(size_t iInd)
{
  switch (iInd) {
  case 0:
    return _arg1;
  case 1:
    return _arg2;
  case 2:
    return _condArg;
  default:
    throw std::logic_error("Incorrect index of argument");
  }
}

const EquationPtr& Equation::GetArg(size_t iInd) const
{
  switch (iInd) {
  case 0:
    return _arg1;
  case 1:
    return _arg2;
  case 2:
    return _condArg;
  default:
    throw std::logic_error("Incorrect index of argument");
  }
}

size_t Equation::GetArgsCount() const
{
  return _argCount;
}

EquationPtr operator+(EquationPtr iLsh, EquationPtr iRsh)
{
  return EquationPtr(new Equation(move(iLsh), move(iRsh), Equation::Sum));
}

EquationPtr operator*(EquationPtr iLsh, EquationPtr iRsh)
{
  return EquationPtr(new Equation(move(iLsh), move(iRsh), Equation::Mul));
}


EquationPtr Equation::MakeVariable(GeomVariablePtr iVariable)
{
  return EquationPtr(new Equation(move(iVariable)));
}

EquationPtr Equation::MakeConstant(double iValue)
{
  return EquationPtr(new Equation(iValue));
}

EquationPtr Equation::MakeCondition(EquationPtr iTrueArg, EquationPtr iFalseArg, EquationPtr iCondArg, ConditionFunction iCondFunc)
{
  return EquationPtr(new Equation(move(iTrueArg), move(iFalseArg), move(iCondArg), move(iCondFunc)));
}


Equation::Equation(double iValue)
  : _value(iValue), _type(Constant), _argCount(0)
{
}

Equation::Equation(GeomVariablePtr iVariable)
  : _var(move(iVariable)), _type(Variable), _argCount(0)
{
}

Equation::Equation(EquationPtr iArg, Type iType)
  : _type(iType), _arg1(move(iArg)), _argCount(1)
{
}

Equation::Equation(EquationPtr iArg1, EquationPtr iArg2, Type iType)
  : _type(iType), _arg1(move(iArg1)), _arg2(move(iArg2)), _argCount(2)
{
}

Equation::Equation(EquationPtr iTrueArg, EquationPtr iFalseArg, EquationPtr iCondArg, ConditionFunction iCondFunc)
  : _type(Condition), _arg1(move(iTrueArg)), _arg2(move(iFalseArg)), _condArg(move(iCondArg)), _argCount(3), _condFunction(move(iCondFunc))
{
}

void Equation::RecursivePrimitiveClone(std::map<EquationPtr, EquationPtr>& ioClonedEqMap, std::map<GeomVariablePtr, GeomVariablePtr>& ioClonedGeomMap)
{
  auto found = ioClonedEqMap.find(shared_from_this());

  if (found != ioClonedEqMap.end()) {
    ioClonedEqMap[shared_from_this()] = Copy();
    if (_arg1)
      _arg1->RecursivePrimitiveClone(ioClonedEqMap, ioClonedGeomMap);
    if (_arg2)
      _arg2->RecursivePrimitiveClone(ioClonedEqMap, ioClonedGeomMap);
    if (_condArg)
      _condArg->RecursivePrimitiveClone(ioClonedEqMap, ioClonedGeomMap);
    if (_var) {
      auto varFound = ioClonedGeomMap.find(_var);

      if (varFound != ioClonedGeomMap.end())
        

    }

  }
}

double Equation::Evaluate()
{
  if (_evaluated)
    return _value;

  _evaluated = true;

  switch (_type) {
  case Variable:
    _value = _var->value;
    break;
  case Constant:
    break;
  case Condition:
    _condValue = _condFunction(_condArg->Evaluate());
    _value = _condValue ? _arg1->Evaluate() : _arg2->Evaluate();
    break;
  case Sum:
    _value = _arg1->Evaluate() + _arg2->Evaluate();
    break;
  case Mul:
    _value = _arg1->Evaluate() * _arg2->Evaluate();
    break;
  }
  return _value;
}

double Equation::Derive(const GeomVariablePtr& iVariable) const
{
  switch (_type) {
  case Variable:
    return _var == iVariable ? 1.0 : 0.0;
  case Constant:
    return 0.0;
  case Condition:
    return _condValue ? _arg1->Derive(iVariable) : _arg2->Derive(iVariable);
  case Sum:
    return _arg1->Derive(iVariable) + _arg2->Derive(iVariable);
  case Mul:
    return _arg1->Evaluate() * _arg2->Derive(iVariable) + _arg1->Derive(iVariable) * _arg2->Evaluate();
  }
  return 0.0;
}

EquationPtr Equation::Clone() const
{
  throw std::logic_error("Not implemented");
}

EquationPtr Equation::Copy() const
{
  return EquationPtr(new Equation(*this));
}
