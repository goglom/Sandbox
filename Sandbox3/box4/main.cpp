
#include "Equation.h"


int main() {
  GeomVariablePtr a(new GeomVariable{3});
  GeomVariablePtr b(new GeomVariable{ 21 });

  auto eqA = Equation::MakeVariable(a);
  auto eqB = Equation::MakeVariable(b);

  auto sum = Equation::MakeCondition(
    eqA * eqA + eqB * eqB, 
    Equation::MakeConstant(0), 
    eqA, 
    [](double x) -> bool{ return x > 2.0;}
  );

  auto resSum = sum->Evaluate();
  auto sumDerivA = sum->Derive(a);
  auto sumDerivB = sum->Derive(b);



  return 0;
}
