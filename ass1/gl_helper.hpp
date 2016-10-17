#ifndef TERGEN_GL_HELPER_H
#define TERGEN_GL_HELPER_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TerrainVAO.hpp"

void gl_init(int argc, char** argv);

void add_vao(TerrainVAO vao);

#endif
