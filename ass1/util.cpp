#include <stdio.h>
#include "util.hpp"

/* get index for a 1d array from 2d arguments */
size_t one_d_index(size_t x, size_t y, size_t width){
  return x + y*width;
}

float normalize(float value, float min, float max){
  return (value - min)/(max - min);
}

void printVec3(glm::vec3& vec){
  printf("%.2f %.2f %.2f\n",vec.x,vec.y,vec.z);
}

void printVec4(glm::vec4& vec){
  printf("%.2f %.2f %.2f %.2f\n",vec.x,vec.y,vec.z,vec.w);
}
