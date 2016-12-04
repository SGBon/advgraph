#include "helper.hpp"
#include <cmath>

unsigned int getGridCell(const float pos,const unsigned int offset){
  return (floor(pos) + offset) - 1;
}

float getReverseGrid(const int cell, const int offset){
  return cell + 1 - offset;
}

bool infront(const boid B, const glm::vec3 C, const float R){
  const glm::vec3 L = C - B.getPosition();

  const float tca = glm::dot(L,B.getDirection());
  /* if the dot product is negative, ray is pointing wrong way */
  if(tca < 0)
    return false;

  const float inside = glm::dot(L,L) - (tca*tca);
  if(inside < 0)
    return false;

  const float d = sqrt(inside);
  /* if d is greater than radius, then ray goes past sphere */
  if(d > R)
    return false;

  const float thc = sqrt((R*R) - (d*d));

  if(tca - thc >= 0 || tca + thc >= 0){
    return true;
  }else{
    return false;
  }
}
