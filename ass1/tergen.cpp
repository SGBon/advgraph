#include "gl_helper.hpp"
#include "Shaders.h"
#include "terproc.hpp"
#include "Texture1D.hpp"

int main(int argc, char** argv){
  /* initialize opengl */
  ter_gl_init(argc,argv);

  /* read ter file */
  struct terrain ter = ter_read("terrain.ter");

  /* compile shaders */
  GLuint program;
  int vs = buildShader(GL_VERTEX_SHADER,"ter.vs");
  int fs = buildShader(GL_FRAGMENT_SHADER, "ter.fs");
  program = buildProgram(vs, fs, 0);
  dumpProgram(program, "Tergen Shader");

  /* define colour ranges for terrain */
  std::vector<glm::vec4> colours;
  colours.push_back(glm::vec4(1.0,0.0,0.0,1.0));
  colours.push_back(glm::vec4(0.0,1.0,0.0,1.0));
  colours.push_back(glm::vec4(0.0,0.0,1.0,1.0));
  int ranges[] = {10,20,30};
  GLuint texture = genTexture1D(colours,ranges);

  /* create VAO and hand over to GL helper functions */
  TerrainVAO* vao = new TerrainVAO(program,texture,ter);
  add_vao(vao);
  setMS(ter.world_size,ter.final_res);
  ter_destroy(ter);

  glutMainLoop();
}
