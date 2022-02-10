#pragma once

#include <functional>
#include <map>

#include "Utils.h"

class Equation;
typedef std::shared_ptr<Equation> EquationPtr;

class Equation : std::enable_shared_from_this<Equation> {
public:
  typedef std::function<bool(double)>  ConditionFunction;
  enum Type {
    Variable = 0,
    Constant,
    Condition,
    Sum,
    Mul,
  };

  double Evaluate();
  double Derive(const GeomVariablePtr& iVariable) const;

  EquationPtr Clone() const;
  EquationPtr Copy() const;
 
  size_t GetArgsCount() const;
  EquationPtr& GetArg(size_t iInd);
  const EquationPtr& GetArg(size_t iInd) const;

  static EquationPtr MakeVariable(GeomVariablePtr iVariable);
  static EquationPtr MakeConstant(double iValue);
  static EquationPtr MakeCondition(EquationPtr iTrueArg, EquationPtr iFalseArg, EquationPtr iCondArg, ConditionFunction iCondFunc);

  friend EquationPtr operator+(EquationPtr iLsh, EquationPtr iRsh);
  friend EquationPtr operator*(EquationPtr iLsh, EquationPtr iRsh);

private:
  Equation(double iValue);
  Equation(GeomVariablePtr iVariable);
  Equation(EquationPtr iArg, Type iType);
  Equation(EquationPtr iArg1, EquationPtr iArg2, Type iType);
  Equation(EquationPtr iTrueArg, EquationPtr iFalseArg, EquationPtr iCondArg, ConditionFunction iCondFunc);

  void RecursivePrimitiveClone(std::map<EquationPtr, EquationPtr>& ioClonedEqMap, std::map<GeomVariablePtr, GeomVariablePtr>& ioClonedGeomMap);

  GeomVariablePtr _var;
  double _value = 0;
  bool _evaluated = false;
  bool _condValue = false;
  Type _type;
  EquationPtr _arg1, _arg2, _condArg;
  size_t _argCount;
  ConditionFunction _condFunction;
};
