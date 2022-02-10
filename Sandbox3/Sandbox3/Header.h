#pragma once

#include <vector>
#include <functional>


using DerivativeVector = std::vector<double>;

using CalcFunction = std::function<double(double, double)>;


struct VariableFunc {
  double value;
  DerivativeVector derivatives;

  inline double operator()()  const {
    return value;
  }
  inline double operator[](size_t i) const {
    return derivatives[i];
  }
};

template<class EvaluableL, class EvaluableR>
struct Multiply {
  EvaluableL left;
  EvaluableR right;

  Multiply(EvaluableL iLeft, EvaluableR iRight) : left(std::move(iLeft)), right(std::move(iRight))
  {}

  inline double operator()()  const {
    return left() * right();
  }
  inline double operator[](size_t i) const {
    return left() * right[i] + left[i] * right();
  }
};


template<class EvaluableL, class EvaluableR>
struct Division {
  EvaluableL left;
  EvaluableR right;

  Division(EvaluableL iLeft, EvaluableR iRight) : left(std::move(iLeft)), right(std::move(iRight))
  {}

  inline double operator()()  const {
    return left() / right();
  }
  inline double operator[](size_t i)  const {
    double rightVal = right();
    return left[i] / rightVal - left() * right[i] / (rightVal * rightVal);
  }
};

template<class EvaluableL, class EvaluableR>
struct Summation {
  EvaluableL left;
  EvaluableR right;

  Summation(EvaluableL iLeft, EvaluableR iRight) : left(std::move(iLeft)), right(std::move(iRight))
  {}

  inline double operator()() const {
    return left() + right();
  }
  inline double operator[](size_t i) const {
    return left[i] + right[i];
  }
};

template<class Evaluable>
struct Abs {
  Evaluable var;

  Abs(Evaluable iVar) : var(iVar)
  {}
  inline double operator()() const {
    return std::abs(var());
  }
  inline double operator[](size_t i) const {
    return var() < 0 ? -var[i] : var[i];
  }
};


template<class EvaluableL, class EvaluableR>
auto operator*(EvaluableL iLsh, EvaluableR iRsh) {
  return Multiply(std::move(iLsh), std::move(iRsh));
}

template<class EvaluableL, class EvaluableR>
auto operator+(EvaluableL iLsh, EvaluableR iRsh) {
  return Summation(std::move(iLsh), std::move(iRsh));
}

template<class EvaluableL, class EvaluableR>
auto operator-(EvaluableL iLsh, EvaluableR iRsh) {
  return Division(std::move(iLsh), std::move(iRsh));
}

template<class Evaluable>
void EvalAndDerive(const Evaluable& iEvaluable, VariableFunc& oResult) {
  oResult.value = iEvaluable();

  for (size_t i = 0; i < oResult.derivatives.size(); ++i)
    oResult.derivatives[i] = iEvaluable[i];
}
