#include "helper.hpp"
#include <cmath>

unsigned int getGridCell(const float pos,const unsigned int offset){
  return (floor(pos) + offset) - 1;
}
