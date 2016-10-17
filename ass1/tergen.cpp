#include "gl_helper.hpp"
#include "Shaders.h"
#include "terproc.hpp"

int main(int argc, char** argv){

  gl_init(argc,argv);
  /* compile shaders */
  GLuint program;
  int vs = buildShader(GL_VERTEX_SHADER,"ter.vs");
  int fs = buildShader(GL_FRAGMENT_SHADER, "ter.fs");
  program = buildProgram(vs, fs, 0);
  dumpProgram(program, "Tergen Shader");

  /* read .ter file
   * calculate subdivisions
   * populate buffers */

  struct terrain ter = ter_read("terrain.ter");
  printf("hello main ");

  TerrainVAO vao(program,ter);
  add_vao(vao);

  glutMainLoop();
  ter_destroy(ter);
}
