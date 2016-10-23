#ifndef TERGEN_TEXTURE_1D_H
#define TERGEN_TEXTURE_1D_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>
#include <vector>

#define COLOUR_BYTE 4
#define COLOUR_RANGE 255.0f

struct Texture1D{
  unsigned char* colour; /* colours in texture */
  unsigned int size; /* size in bytes of texture */
};

/* generate a 1 dimensional texture
 * colours are vectors of colours for texture
 * ranges are heights for each colour to be applied
 */
void Texture1D_create(struct Texture1D& texture,unsigned int resolution,std::vector<glm::vec4> colours,int ranges[]);

void Texture1D_destroy(struct Texture1D& texture);
#endif
