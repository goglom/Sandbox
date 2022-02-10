#include "CalcNode.h"

#include <iostream>
#include <vector>

using namespace std;


class EquationSystem {
public:
  

private:
  std::vector<CalcNodePtr> _equationNodes;
  std::vector<CalcNodePtr> _variableNodes;
};





int main() {
  CalcNodePtr x = CalcNode::MakeVariable(5);
  CalcNodePtr y = CalcNode::MakeVariable(10);
  
  CalcNodePtr equation = (x + sin(y * x)) * x + y;
  
  std::cout << equation->CalculateValue() << endl;

  equation->CalculateDerivatives();
  
  std::cout << x->GetDerivative() << endl;
  std::cout << y->GetDerivative() << endl;
}