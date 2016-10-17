#include "gl_helper.hpp"
#include <cmath>
#include <cstdio>
#include "Shaders.h"
#include "terproc.hpp"

int main(int argc, char** argv){
  gl_init(argc,argv);
  /* read .ter file
   * calculate subdivisions
   * populate buffers */

  struct terrain ter = ter_read("terrain.ter");

  ter_destroy(ter);

  return 0;
}
