#ifndef TERGEN_TERRAIN_VAO_H
#define TERGEN_TERRAIN_VAO_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include "tertype.h"
class TerrainVAO{
private:
  GLuint id;
  GLuint vbuffer;
  GLuint ibuffer;

  GLfloat* vertices;
  GLuint* indices;

  GLuint num_vertices;
  GLuint num_indices;
  unsigned int width;

  GLuint shader_program;

  /* set indices to link up vertices into triangles*/
  void linkVertices();

  /* put vertices and indices into buffers */
  void populateBuffers();

public:
  /* constructor from terrain struct and shader program id */
  TerrainVAO(GLuint shader_program, struct terrain& ter);
  /* default constructor */
  TerrainVAO();

  /* load a terrain struct into the VAO */
  void loadTer(struct terrain& ter);

  /* set shader program for this VAO */
  void setShader(GLuint shader_program);

  GLuint getShader();

  void drawVAO();
};

#endif
