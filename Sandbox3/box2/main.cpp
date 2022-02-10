#include "Function.h"

#include <vector>
#include <iostream>
#include <chrono>
#include <iomanip>

#include "Equation.h"

#define TIME_MACRO(resultVarMilliSec, statement)                            \
  unsigned long long resultVarMilliSec;                                     \
  {                                                                         \
    using namespace std::chrono;                                            \
    system_clock::time_point _start = system_clock::now();                  \
    statement;                                                              \
    system_clock::time_point _end = system_clock::now();                    \
    resultVarMilliSec = duration_cast<milliseconds>(_end - _start).count(); \
  }

class TrigonometricIdentity : public Equation {
public:
  TrigonometricIdentity(const Indexes& iIndLine);
  virtual Variable EvalAndDerive(const VariableVector& iPoint) const;

private:
  Indexes _indLine;
};

TrigonometricIdentity::TrigonometricIdentity(const Indexes& iIndLine) : _indLine(iIndLine)
{}

Variable TrigonometricIdentity::EvalAndDerive(const VariableVector& iPoint) const
{
  return pow(iPoint[_indLine[0]] * iPoint[_indLine[0]] + iPoint[_indLine[1]] * iPoint[_indLine[1]] + iPoint[_indLine[2]] * iPoint[_indLine[2]], 3) - 1.0;
}

Function MakeTrigonometricIdentityEq(const Indexes& iIndLine)
{
  Function x = Function(iIndLine[0]);
  Function y = Function(iIndLine[1]);
  Function z = Function(iIndLine[2]);

  return Pow(x * x + y * y + z * z, 3) - 1.0;
}

template<class Func>
double TimeMeasurement(Func func, size_t iterCount)
{
  TIME_MACRO(time,
    for (size_t i = 0; i < iterCount; ++i)
    {
      func();
    }
  )
  return static_cast<double>(time) / iterCount;
}

VariableVector Make(const DerivVector& vars)
{
  VariableVector res(vars.size());

  for (size_t i = 0; i < res.size(); ++i)
  {
    res[i].GetValue() = vars[i];
    DoubleVector derivs(vars.size(), 0.0);
    derivs[i] = 1.0;
    res[i].GetDerivatives() = std::move(derivs);
  }
  return res;
}

int main() {

  DoubleVector vars(100);
  vars[0] = 1;
  vars[1] = 2;
  vars[2] = 3;
  VariableVector varVec = Make(vars);
  Indexes ind = { 0, 1, 2 };

  auto eq1 = MakeTrigonometricIdentityEq(ind);
  auto eq2 = EquationPtr(new TrigonometricIdentity(ind));


  auto func1 = [&eq1, &vars]()
  {
    Variable res(eq1.Eval(vars), eq1.Derive(vars));
  };
  auto func2 = [&eq2, &varVec]()
  {
    Variable res = eq2->EvalAndDerive(varVec);
  };
  size_t iters = 500000;

  std::cout << TimeMeasurement(func1, iters) << std::endl;
  std::cout << TimeMeasurement(func2, iters) << std::endl;

  Variable res1(eq1.Eval(vars), eq1.Derive(vars));
  Variable res2 = eq2->EvalAndDerive(varVec);

  std::cout << std::abs(res1.GetValue() - res2.GetValue()) << std::endl;
  double sumSq = 0;
  for (size_t i = 0; i < res1.GetDerivatives().size(); ++i) {
    sumSq += std::pow(res1.GetDerivatives()[i] - res2.GetDerivatives()[i], 2);
  }
  std::cout << std::setprecision(16) << std::sqrt(sumSq);

  return 0;
}