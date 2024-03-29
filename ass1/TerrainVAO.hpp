#ifndef TERGEN_TERRAIN_VAO_H
#define TERGEN_TERRAIN_VAO_H
/* VAO for terrain class */

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include "tertype.h"
#include "Texture1D.hpp"

class TerrainVAO{
private:
  GLuint id;
  GLuint vbuffer;
  GLuint ibuffer;
  GLuint tbuffer;

  /* pointers to vertices, indices */
  GLfloat* vertices;
  GLfloat* normals;
  GLuint* indices;
  GLfloat* texels;

  /* total number of vertices,indices */
  GLuint num_vertices;
  GLuint num_indices;
  GLuint num_texels;

  static const unsigned char vertex_width = 4; /* size of single vertex in OpenGL */
  static const unsigned char normal_width = 3; /* size of single vertex normal */
  static const unsigned char texel_width = 4; /* size of single texel */

  unsigned int width; /* size of grid sides */
  const float ratio; /* ratio between grid space and world space */

  GLuint shader_program; /* shader program id to use for this VAO */


  /* default constructor, private so can't be used */
  TerrainVAO();

  /* set indices to link up vertices into triangles*/
  void linkVertices();

  /* create vertices from height values */
  void createVertices(GLfloat* heights);

  /* create normals from vertices */
  void createNormals();

  /* put vertices and indices into buffers */
  void populateBuffers();

public:
  float min,max; /* minimum and maximum heights */

  /* constructor from terrain struct, shader program id, and texture */
  TerrainVAO(GLuint shader_program, struct Texture1D& texture, struct terrain& ter);

  /* deconstructor */
  ~TerrainVAO();

  /* load a terrain struct into the VAO */
  void loadTer(struct terrain& ter);

  /* load a Texture1D struct into the VAO */
  void loadTexture(struct Texture1D& texture);

  /* set shader program for this VAO */
  void setShader(GLuint shader_program);

  /* retrieves the shader */
  GLuint getShader();

  /* draws the VAO to OpenGL */
  void drawVAO();

  void printVerts();
  void printNormals();
};

#endif
