/* a struct for VAOs */

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stdlib.h>

#define VERT_WIDTH 3
#define NORM_WIDTH 3

struct VAO{
  /* buffers */
  GLfloat *vertices;
  GLfloat *normals;
  GLuint *indices;

  /* buffer sizes */
  size_t num_vertices;
  size_t num_normals;
  size_t num_indices;

  GLuint id; /* identifier for the VAO */
  GLuint program; /* shader program used with the VAO */

  /* buffer identifiers */
  GLuint vbuffer;
  GLuint ibuffer;

  /* texture/buffer state */
  enum vao_state{
    undefined, /* memory not yet allocated */
    post_buffer, /* after buffers filled */
  } b_state; /* texture state, buffer state */
};

/* clears some data in struct */
void VAO_init(struct VAO *vao);

/* create a VAO from a .obj file stored in filename */
void VAO_loadObj(struct VAO *vao, char *filename);

/* release memory allocated to the VAO */
void VAO_destroy(struct VAO *vao);
