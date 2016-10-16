#ifndef TERGEN_TERPROC_H
#define TERGEN_TERPROC_H
/* terproc: terrain processing functions */

#include <GL/glut.h>

struct terrain{
  int world_size[2]; /* size in OpenGL space */
  int final_res[2]; /* final resolution */
  int init_res[2]; /* initial resolution */
  GLfloat *init_heights; /* initial heights */
  GLfloat *heights; /* final heights */
};

/* reads a file specified by filename and parses the data
 * into a terrain struct
 */
int ter_read(char *filename, struct terrain *ter);

/* generates the terrain, performs subdivisions */
int ter_generate(struct terrain *ter);
#endif
