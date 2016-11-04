/* a struct for VAOs */

#include <GL/gl.h>
#include <stdlib.h>

struct VAO{
  GLuint id; /* identifier for the VAO */

  /* buffer identifiers */
  GLuint vbuffer;
  GLuint ibuffer;
  GLuint tbuffer;

  /* buffers */
  GLfloat *vertices;
  GLfloat *normals;
  GLuint *indices;

  /* buffer sizes */
  size_t num_vertices;
  size_t num_normals;
  size_t num_indices;

  unsigned char vertex_width;
  unsigned char normal_width;

  GLuint program; /* shader program used with the VAO */
};

void VAO_loadObj(char *filename,struct VAO *vao);
