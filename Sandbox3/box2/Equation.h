#pragma once

#include <memory>
#include "Variable.h"

class Equation;
typedef std::shared_ptr<Equation> EquationPtr;
typedef std::vector<EquationPtr> EquationVector;
typedef std::vector<size_t> Indexes;

class Equation {
public:
  // Calculate value of function and its derivatives using the autodifferentiation 
  // mechanism implemented by operators overloading in Variable class
  virtual Variable EvalAndDerive(const VariableVector& iPoint) const = 0;
  virtual ~Equation();
  bool isDimensional;

protected:
  Equation();
  Equation(const Equation& iEqObj);
};