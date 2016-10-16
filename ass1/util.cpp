#include "util.hpp"

/* get index for a 1d array from 2d arguments */
size_t one_d_index(size_t x, size_t y, size_t width){
  return x + y*width;
}
