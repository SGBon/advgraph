#ifndef VAO_HPP
#define VAO_HPP

/* a struct for VAOs */

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cstdlib>
#include "lsystem.hpp"

#define VERT_WIDTH 4
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

/* create VAO from an lsystem */
void VAO_loadlsystem(struct VAO *vao, const lsystem &lsys);

/* release memory allocated to the VAO */
void VAO_destroy(struct VAO *vao);
#endif
