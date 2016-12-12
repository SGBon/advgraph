#ifndef LSYSTEM_HPP
#define LSYSTEM_HPP
#include <stack>
#include <string>
#include <glm/glm.hpp>

/* struct for holding states of the turtle so that it can be pushed onto and off stack */
struct turtle_state{
  glm::vec4 position;
  glm::vec4 direction;
  unsigned int index = 0; /* vertice index for linking meshes */

  turtle_state(glm::vec4 position,glm::vec4 direction):
  position(position),
  direction(direction){}
};

class lsystem{
public:
  lsystem(const glm::vec4 origin = glm::vec4(0.0,0.0,0.0,1.0),
    const glm::vec4 direction = glm::vec4(0.0,1.0,0.0,0.0));

  /* read and expand the lsystem specifier in the file at filename */
  void readlsystem(const std::string &filename);

private:
  /* evaluate a symbol in the lsystem, changing the turtle's state */
  void evaluate_symbol(const char symbol);

  /* state and stack */
  turtle_state state;
  std::stack<turtle_state> state_stack;

  /* turtle parameters */
  float move_magnitude; // magnitude of translations
  float rotation_angle; // magnitude of rotations

  std::string system_string;

  /* vertices, indices, and normals of the evaluated lsystem */
};

#endif
