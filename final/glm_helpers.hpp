#ifndef GLM_HELPERS_HPP
#define GLM_HELPERS_HPP

/* helper functions for GLM vectors */

#include <glm/glm.hpp>

/* swaps the largest components and sets the remaining
 * component to negative to construct a non-colinear vector
 */
glm::vec3 swap_largest(glm::vec3 v);

/* swizzle a 4-vector to a 3-vector */
glm::vec3 swiz43(const glm::vec4 v);

#endif
