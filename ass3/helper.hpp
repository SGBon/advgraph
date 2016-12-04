#ifndef TRIBES_HELPER_H
#define TRIBES_HELPER_H

#include "boid.hpp"

/* get a grid cell based on pos in the boids grid */
unsigned int getGridCell(const float pos, const unsigned int offset);

/* do the inverse */
float getReverseGrid(const int cell, const int offset);

/* determine if a sphere is in front of a boid */
bool infront(const boid B, const glm::vec3 C, const float R);
#endif
