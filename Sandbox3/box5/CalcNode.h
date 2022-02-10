#pragma once

#include <functional>
#include <memory>
#include <cmath>

class CalcNode;
typedef std::shared_ptr<CalcNode> CalcNodePtr;


class CalcNode {
public:
  typedef std::function<bool(double)> CondtitionFunction;

  enum Type {
    Variable,
    Condition,
    Summation,
    Multiplication,
    Sin,

    //...
  };
  double GetDerivative() const;
  double CalculateValue();
  void CalculateDerivatives();
  void Clear();

  double value;
  CalcNodePtr arg1, arg2;
  CalcNodePtr conditionArg;

  static CalcNodePtr MakeVariable(double iValue);
  static CalcNodePtr Construct(Type iType, CalcNodePtr iArg1, CalcNodePtr iArg2 = nullptr);
  static CalcNodePtr MakeConditionalFuncton(CalcNodePtr iTrueBranch, CalcNodePtr iFalseBranch, CalcNodePtr iConditionArg, CondtitionFunction iConditionFuction);

private:
  explicit CalcNode(double iValue);
  CalcNode(Type iType, CalcNodePtr iArg1, CalcNodePtr iArg2);
  CalcNode(CalcNodePtr iTrueBranch, CalcNodePtr iFalseBranch, CalcNodePtr iConditionArg, CondtitionFunction iConditionFuction);


  void PropagateDerivatives(double iInputDerivative);

  std::function<bool(double)> _condition;
  Type _type;
  bool _evaluated;
  double _derivative;
};


void CalcNode::Clear() {
  _evaluated = false;
  
  if (arg1)
    arg1->Clear();
  if (arg2)
    arg2->Clear();
  if (conditionArg)
    conditionArg->Clear();
}

CalcNodePtr sin(CalcNodePtr iArg) {
  return CalcNode::Construct(CalcNode::Sin, std::move(iArg));
}

CalcNodePtr operator+(CalcNodePtr iArg1, CalcNodePtr iArg2) {
  return CalcNode::Construct(CalcNode::Summation, std::move(iArg1), std::move(iArg2));
}

CalcNodePtr operator*(CalcNodePtr iArg1, CalcNodePtr iArg2) {
  return CalcNode::Construct(CalcNode::Multiplication, std::move(iArg1), std::move(iArg2));
}


CalcNodePtr CalcNode::MakeVariable(double iValue) {
  return CalcNodePtr(new CalcNode(iValue));
}

CalcNodePtr CalcNode::Construct(Type iType, CalcNodePtr iArg1, CalcNodePtr iArg2) {
  return CalcNodePtr(new CalcNode(iType, std::move(iArg1), std::move(iArg2)));
}

CalcNodePtr CalcNode::MakeConditionalFuncton(CalcNodePtr iTrueBranch, CalcNodePtr iFalseBranch, CalcNodePtr iConditionArg, CondtitionFunction iConditionFuction) {
  return CalcNodePtr(new CalcNode(std::move(iTrueBranch), std::move(iFalseBranch), std::move(iConditionArg), std::move(iConditionFuction)));
}

CalcNode::CalcNode(double iValue)
  : value(iValue), _derivative(0), _evaluated(false), _type(Variable)
{}

CalcNode::CalcNode(Type iType, CalcNodePtr iArg1, CalcNodePtr iArg2)
  : value(0), _derivative(0), _evaluated(false), arg1(std::move(iArg1)), arg2(std::move(iArg2)), _type(iType)
{
}

CalcNode::CalcNode(CalcNodePtr iTrueBranch, CalcNodePtr iFalseBranch, CalcNodePtr iConditionArg, CondtitionFunction iConditionFuction)
  : value(0), _derivative(0), _evaluated(false), arg1(std::move(iTrueBranch)), arg2(std::move(iFalseBranch)), conditionArg(std::move(iConditionArg)),
    _type(Condition)
{

}

inline double CalcNode::GetDerivative() const
{
  return _derivative;
}

inline double CalcNode::CalculateValue()
{
  if (!_evaluated) {
    switch (_type) {
    case Variable: {
      break;
    }
    case Condition: {
      if (_condition(conditionArg->CalculateValue())) {
        value = arg1->CalculateValue();
      }
      else {
        value = arg2->CalculateValue();
      }
      break;
    }
    case Summation: {
      value = arg1->CalculateValue() + arg2->CalculateValue();
      break;
    }
    case Multiplication: {
      value = arg1->CalculateValue() * arg2->CalculateValue();
      break;
    }
    case Sin: {
      value = std::sin(arg1->CalculateValue());
      break;
    }
    }
  }

  _evaluated = true;

  return value;
}

inline void CalcNode::CalculateDerivatives()
{
  PropagateDerivatives(1);
}

inline void CalcNode::PropagateDerivatives(double iInputDerivative)
{
  _derivative += iInputDerivative;

  switch (_type) {
  case Variable: {
    break;
  }
  case Condition: {
    if (_condition(conditionArg->value)) {
      arg1->PropagateDerivatives(iInputDerivative);
    }
    else {
      arg2->PropagateDerivatives(iInputDerivative);
    }
    break;
  case Summation: {
    arg1->PropagateDerivatives(iInputDerivative);
    arg2->PropagateDerivatives(iInputDerivative);
    break;
  }
  case Multiplication: {
    arg1->PropagateDerivatives(iInputDerivative * arg2->value);
    arg2->PropagateDerivatives(iInputDerivative * arg1->value);
    break;
  }
  case Sin: {
    arg1->PropagateDerivatives(iInputDerivative * std::cos(arg1->value));
    break;
  }
  }
  }
}
