#ifndef TERGEN_TEXTURE_1D_H
#define TERGEN_TEXTURE_1D_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>
#include <vector>

/* generate a 1 dimensional texture
 * colours are vectors of colours for texture
 * ranges are heights for each colour to be applied
 */
GLuint genTexture1D(std::vector<glm::vec4> colours,int ranges[]);

#endif
