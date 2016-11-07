/* a struct for VAOs */

#include <GL/glew.h>
#include <stdlib.h>
#include "texture.h"

struct VAO{
  /* pointers to texture in union */
  union text_p{
    Texture *t;
    Cube *c;
  }texture;

  /* texture state */
  enum vao_state{
    undefined, /* memory not yet allocated */
    is_texture,
    is_cubemap,
    post_buffer /* after buffers filled */
  } t_state,b_state; /* texture state, buffer state */

  /* buffer sizes */
  size_t num_vertices;
  size_t num_normals;
  size_t num_texels;
  size_t num_indices;

  GLuint id; /* identifier for the VAO */
  GLuint program; /* shader program used with the VAO */

  /* buffer identifiers */
  GLuint vbuffer;
  GLuint ibuffer;
  GLuint tbuffer;

  /* buffers */
  GLfloat *vertices;
  GLfloat *normals;
  GLfloat *texels;
  GLuint *indices;
};

/* clears some data in struct */
void VAO_init(struct VAO *vao);

/* create a VAO from a .obj file stored in filename */
void VAO_loadObj(struct VAO *vao, char *filename, unsigned char is_sphere);

/* release memory allocated to the VAO */
void VAO_destroy(struct VAO *vao);

/* load cubemap into VAO */
void VAO_loadCubeMap(struct VAO *vao, char *filename);
