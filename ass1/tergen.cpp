#include "gl_helper.hpp"
#include "Shaders.h"
#include "terproc.hpp"

int main(int argc, char** argv){
  /* read .ter file
   * calculate subdivisions
   * populate buffers */

  struct terrain ter = ter_read("terrain.ter");

  ter_gl_init(argc,argv);
  /* compile shaders */
  GLuint program;
  int vs = buildShader(GL_VERTEX_SHADER,"ter.vs");
  int fs = buildShader(GL_FRAGMENT_SHADER, "ter.fs");
  program = buildProgram(vs, fs, 0);
  dumpProgram(program, "Tergen Shader");

  TerrainVAO* vao = new TerrainVAO(program,ter);
  add_vao(vao);
  ter_destroy(ter);

  glutMainLoop();
}
