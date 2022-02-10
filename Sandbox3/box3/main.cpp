#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <set>
#include <array>
#include <tuple>

using std::move;

class CalcNode;

typedef std::shared_ptr<CalcNode> CalcNodePtr;
typedef std::vector<CalcNodePtr> CalcNodeVector;
typedef std::set<CalcNodePtr> CalcNodeSet;



class CalcNode
{
public:
  virtual ~CalcNode() = default;

  static CalcNodePtr Construct(double iValue);
  virtual void CalcValue();
  virtual void CalcDerivatives(double iPrevFactor = 1.0);

  double value;
  double derivative;
  CalcNodeVector dependencies;
protected:
  bool _evaluated;

  CalcNode();
  CalcNode(double iValue);
};

class Condition;
using ConditionPtr = std::shared_ptr<Condition>;

class Condition : public CalcNode
{
public:
  typedef bool (*CondFunction)(double);

  static ConditionPtr Construct(CalcNodePtr iTrueBranchNode, CalcNodePtr iFalseBranchNode, CalcNodePtr iConditionNode, CondFunction iCondition) {
      return ConditionPtr(new Condition(std::move(iTrueBranchNode), std::move(iFalseBranchNode), std::move(iConditionNode), iCondition));
  }
  

  
  CalcNodePtr trueBranchNode;
  CalcNodePtr falseBranchNode;
  CalcNodePtr condNode;
  CondFunction condFunc;
  bool condValue = false;

  void CalcValue() override
  {
    CalcNode::CalcValue();

    condNode->CalcValue();
    condValue = condFunc(condNode->value);
    if (condValue) {      
      trueBranchNode->CalcValue();
      value = trueBranchNode->value;
    } 
    else {
      falseBranchNode->CalcValue();
      value = falseBranchNode->value;
    }
  }
  void CalcDerivatives(double iPrevFactor = 1.0) override
  {
    CalcNode::CalcDerivatives(iPrevFactor);

    if (condValue) {
      trueBranchNode->CalcDerivatives(iPrevFactor);
    }
    else
      falseBranchNode->CalcDerivatives(iPrevFactor);
  }
private:
  Condition(CalcNodePtr iTrueBranchNode, CalcNodePtr iFalseBranchNode, CalcNodePtr iConditionNode, CondFunction iCondition)
    : trueBranchNode(std::move(iTrueBranchNode)), falseBranchNode(std::move(iFalseBranchNode)), condNode(std::move(iConditionNode)), condFunc(iCondition)
  {}
};


template<size_t ArgsCount>
struct GetMultiArgFunction;

template<>
struct GetMultiArgFunction<1>
{
  typedef double (*Type)(double);
};

template<>
struct GetMultiArgFunction<2>
{
  typedef double (*Type)(double, double);
};


template<size_t Size>
double Apply(typename GetMultiArgFunction<Size>::Type iFunction, const std::array<CalcNodePtr, Size>& iArray);

template<>
double Apply<1>(GetMultiArgFunction<1>::Type iFunction, const std::array<CalcNodePtr, 1>& iArray)
{
  return iFunction(iArray[0]->value);
}

template<>
double Apply<2>(GetMultiArgFunction<2>::Type iFunction, const std::array<CalcNodePtr, 2>& iArray)
{
  return iFunction(iArray[0]->value, iArray[1]->value);
}


template<size_t ArgsCount>
class FunctionNode : public CalcNode {
public:
  typedef typename GetMultiArgFunction<ArgsCount>::Type Function;
  typedef std::array<Function, ArgsCount> FunctionArray;
  typedef std::array<CalcNodePtr, ArgsCount> CalcNodeArray;
  typedef std::shared_ptr<FunctionNode> Ptr;

  struct DerivableFunction
  {
    Function value;
    FunctionArray derivatives;
  };
  
  CalcNodeArray args;
  DerivableFunction function;



  static Ptr Construct(CalcNodeArray iArgs, DerivableFunction iFunction)
  {
    return Ptr(new FunctionNode(std::move(iArgs), std::move(iFunction)));
  }

  void CalcValue() override
  {
    CalcNode::CalcValue();

    for (size_t i = 0; i < ArgsCount; ++i) {
      args[i]->CalcValue();
    }
    value = Apply(function.value, args);
  }
  void CalcDerivatives(double iPrevFactor = 1.0) override
  {
    CalcNode::CalcDerivatives(iPrevFactor);
    for (size_t i = 0; i < ArgsCount; ++i) {
      double partialDerivative = Apply(function.derivatives[i], args);
      args[i]->CalcDerivatives(partialDerivative * iPrevFactor);
    }
  }
private:
  FunctionNode(CalcNodeArray iArgs, DerivableFunction iFunction)
    :  args(std::move(iArgs)), function(std::move(iFunction))
  {}
};

typedef FunctionNode<1> UnFunctionNode;
typedef FunctionNode<2> BinFunctionNode;

UnFunctionNode::Ptr MakeUnaryFunctionNode(CalcNodePtr iArg, UnFunctionNode::DerivableFunction iFunction)
{
  UnFunctionNode::CalcNodeArray args = { iArg };
  auto result = UnFunctionNode::Construct(std::move(args), iFunction);
  result->dependencies.push_back(std::move(iArg));
  return result;
}

BinFunctionNode::Ptr MakeBinaryFunctionNode(CalcNodePtr iLsh, CalcNodePtr iRsh, BinFunctionNode::DerivableFunction iFunction)
{
  BinFunctionNode::CalcNodeArray args = { iLsh, iRsh };
  auto result = BinFunctionNode::Construct(std::move(args), iFunction);
  result->dependencies.push_back(std::move(iLsh));
  result->dependencies.push_back(std::move(iRsh));
  return result;
}

BinFunctionNode::Ptr operator+(CalcNodePtr iLsh, CalcNodePtr iRsh)
{
  BinFunctionNode::DerivableFunction function{
    [](double iLsh, double iRsh)->double {return iLsh + iRsh; },
    BinFunctionNode::FunctionArray{
      [](double, double)->double {return 1.0; },
      [](double, double)->double {return 1.0; }
    }
  };
  return MakeBinaryFunctionNode(std::move(iLsh), std::move(iRsh), function);
}

BinFunctionNode::Ptr operator*(CalcNodePtr iLsh, CalcNodePtr iRsh)
{
  BinFunctionNode::DerivableFunction function{
    [](double iLsh, double iRsh)->double {return iLsh * iRsh; },
    BinFunctionNode::FunctionArray{
      [](double, double iRsh)->double {return iRsh; },
      [](double iLsh, double)->double {return iLsh; }
    }
  };
  return MakeBinaryFunctionNode(std::move(iLsh), std::move(iRsh), function);
}

UnFunctionNode::Ptr Sin(CalcNodePtr iArg)
{
  UnFunctionNode::DerivableFunction function{
    std::sin,
    UnFunctionNode::FunctionArray{std::cos}
  };
  return MakeUnaryFunctionNode(std::move(iArg), function);
}


int main() {
  using namespace std;

  auto a = CalcNode::Construct(3);
  auto b = CalcNode::Construct(3);

  auto f = CalcNode::Construct(0);
  auto t = a * b;
  
  auto res = Condition::Construct(t, f, a, [](double x){ return x > 2;});

  res->CalcValue();
  res->CalcDerivatives();

  cout << res->value << endl;
  cout << a->derivative;

  return 0;
}

inline CalcNodePtr CalcNode::Construct(double iValue)
{
  return CalcNodePtr(new CalcNode(iValue));
}

inline void CalcNode::CalcValue()
{
  if (_evaluated)
    return;

  derivative = 0;
  _evaluated = true;
}


inline CalcNode::CalcNode() : CalcNode(0.0)
{}

inline CalcNode::CalcNode(double iValue)
  : value(iValue), derivative(0.0), _evaluated(false)
{}
inline void CalcNode::CalcDerivatives(double iPrevFactor)
{
  derivative += iPrevFactor;
  _evaluated = false;
}
