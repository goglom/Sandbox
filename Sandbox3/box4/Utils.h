#pragma once
#include <memory>

class GeomVariable;
typedef std::shared_ptr<GeomVariable> GeomVariablePtr;


class GeomVariable
{
public:
  double value;
};
