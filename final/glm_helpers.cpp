#include "glm_helpers.hpp"

/* swaps the largest components and sets the remaining
 * component to negative to construct a non-colinear vector
 */
glm::vec3 swap_largest(glm::vec3 v){
  glm::vec3 ret;
  if(v.x > v.y && v.x > v.z){
    if(v.y > v.z)
      ret = glm::vec3(v.y,v.x,-v.z);
    else
      ret = glm::vec3(v.z,-v.y,v.x);
  }else if (v.y > v.x && v.y > v.z){
    if (v.x > v.z)
      ret = glm::vec3(v.y,v.x,-v.z);
    else
      ret = glm::vec3(-v.x,v.z,v.y);
  }else if (v.z > v.x && v.z > v.y){
    if(v.x > v.y)
      ret = glm::vec3(v.z,-v.y,v.x);
    else
      ret = glm::vec3(-v.x,v.z,v.y);
  }

  return ret;
}

/* swizzle a 4-vector to a 3-vector */
glm::vec3 swiz43(const glm::vec4 v){
  return glm::vec3(v.x,v.y,v.z);
}
