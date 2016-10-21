#ifndef TERGEN_UTIL_H
#define TERGEN_UTIL_H
#include <stddef.h>
#include <glm/glm.hpp>

/* get index for a 1d array from 2d arguments */
size_t one_d_index(size_t x, size_t y,size_t width);

/* normalize input value between min and max and return result */
float normalize(float value,float min, float max);

void printVec3(glm::vec3 vec);

#endif
