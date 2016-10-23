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

  colours.push_back(glm::vec4(0.31,0.31,0.94,1.0)); /* deep water */
  colours.push_back(glm::vec4(0.31,0.94,0.94,1.0)); /* shallow water */
  colours.push_back(glm::vec4(0.8,0.72,0.439,1.0)); /* beach */
  colours.push_back(glm::vec4(0.15,0.46,0.03,1.0)); /* light grass */
  colours.push_back(glm::vec4(0.16,0.39,0.01,1.0)); /* grass */
  colours.push_back(glm::vec4(0.42,0.37,0.33,1.0)); /* stone */
  colours.push_back(glm::vec4(1.0,1.0,1.0,1.0)); /* snow */
  int ranges[] = {3,5,3,7,14,9,1};
  struct Texture1D texture;
  Texture1D_create(texture,64,colours,ranges);

  /* create VAO and hand over to GL helper functions */
  TerrainVAO* vao = new TerrainVAO(program,texture,ter);
  add_vao(vao);
  setMS(ter.world_size,ter.final_res);
  ter_destroy(ter);

  glutMainLoop();
}
