#ifndef TERGEN_TERRAIN_TYPE_H
#define TERGEN_TERRAIN_TYPE_H
#include <GL/gl.h>
struct terrain{
  GLfloat world_size; /* size in OpenGL space */
  unsigned int final_res; /* final resolution */
  unsigned int init_res; /* initial resolution */
  GLfloat *heights; /* final heights */
  GLfloat min,max; /* minimum, maximum heights */
};
#endif
