#include <iostream>
#include <cmath>
#include <functional>
#include <vector>

using DerivVector = std::vector<double>;
using ValueFunc = std::function<double()>;
using DerivFunc = std::function<double(size_t)>;

struct Variable {
  double value;
  DerivVector derivatives;
};

struct VariableFunc {
  ValueFunc val;
  DerivFunc deriv;
  VariableFunc()
  {}
  VariableFunc(double iVal, size_t iIndex) {
    val = [iVal]() -> double {
      return iVal;
    };
    deriv = [iIndex](size_t iInd) -> double {
      return iInd == iIndex ? 1.0 : 0.0;
    };
  }
  VariableFunc(double iVal)
    : val([iVal]() -> double {return iVal;}), deriv([](size_t) -> double {return 0.0;})
  {}
  Variable EvalAndDerive(size_t iDerivSize) {
    Variable result;
    result.value = val();
    result.derivatives.reserve(iDerivSize);
    for (size_t i = 0; i < iDerivSize; ++i) {
      result.derivatives[i] = deriv(i);
    }
  }
};

VariableFunc operator+(const VariableFunc& lsh, const VariableFunc& rsh) {
  VariableFunc result;
  result.val = [lsh, rsh]() -> double {
    return lsh.val() + rsh.val();
  };
  result.deriv = [lsh, rsh](size_t iInd) -> double {
    return lsh.deriv(iInd) + rsh.deriv(iInd);
  };
  return result;
}

VariableFunc operator-(const VariableFunc& lsh, const VariableFunc& rsh) {
  VariableFunc result;
  result.val = [lsh, rsh]() -> double {
    return lsh.val() - rsh.val();
  };
  result.deriv = [lsh, rsh](size_t iInd) -> double {
    return lsh.deriv(iInd) - rsh.deriv(iInd);
  };
  return result;
}

VariableFunc operator*(const VariableFunc& lsh, const VariableFunc& rsh) {
  VariableFunc result;
  result.val = [lsh, rsh]() -> double {
    return lsh.val() * rsh.val();
  };
  result.deriv = [lsh, rsh](size_t iInd) -> double {
    return lsh.deriv(iInd) * rsh.val() + lsh.val() * rsh.deriv(iInd);
  };
  return result;
}

VariableFunc operator/(const VariableFunc& lsh, const VariableFunc& rsh) {
  VariableFunc result;
  result.val = [lsh, rsh]() -> double {
    return lsh.val() / rsh.val();
  };
  result.deriv = [lsh, rsh](size_t iInd) -> double {
    double rVal = rsh.val();
    return lsh.deriv(iInd) / rVal  + lsh.val() * rsh.deriv(iInd) / (rVal * rVal);
  };
  return result;
}

VariableFunc operator-(const VariableFunc& arg) {
  VariableFunc result;
  result.val = [arg]() -> double {
    return -arg.val();
  };
  result.deriv = [arg](size_t iInd) -> double {
    return -arg.deriv(iInd);
  };
  return result;
}

VariableFunc abs(const VariableFunc& arg) {
  VariableFunc result;
  result.val = [arg]() -> double {
    return std::abs(arg.val());
  };
  result.deriv = [arg](size_t iInd) -> double {
    return arg.val() < 0 ? -arg.deriv(iInd) : arg.deriv(iInd);
  };
  return result;
}

int main() {
  auto a = VariableFunc(1, 0);
  auto b = VariableFunc(10, 1);
  
  auto c = a + b;

  auto res = c.EvalAndDerive(2);

  return 0;
}


