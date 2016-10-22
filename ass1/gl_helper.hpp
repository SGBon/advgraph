#ifndef TERGEN_GL_HELPER_H
#define TERGEN_GL_HELPER_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include "TerrainVAO.hpp"

void ter_gl_init(int argc, char** argv);

/* sets movespeed for camera based on size of grid in OpenGL space */
void setMS(const float world_size,const float grid_res);

void add_vao(TerrainVAO* vao);

#endif
