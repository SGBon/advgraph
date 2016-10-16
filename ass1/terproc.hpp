#ifndef TERGEN_TERPROC_H
#define TERGEN_TERPROC_H
/* terproc: terrain processing functions */

#include <GL/glut.h>
#include <string>

struct terrain{
  int world_size[2]; /* size in OpenGL space */
  int final_res[2]; /* final resolution */
  int init_res[2]; /* initial resolution */
  GLfloat *heights; /* final heights */
};

/* reads a file specified by filename and parses the data
 * into a terrain struct.
 */
struct terrain ter_read(std::string filename);

/* deallocates any memory used my ter */
void ter_destroy(struct terrain& ter);

#endif
