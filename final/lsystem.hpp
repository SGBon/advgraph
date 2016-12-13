#ifndef LSYSTEM_HPP
#define LSYSTEM_HPP
#include <stack>
#include <vector>
#include <string>
#include <glm/glm.hpp>

/* struct for holding states of the turtle so that it can be pushed onto and off stack */
struct turtle_state{
  glm::dvec4 position;
  glm::dvec4 direction;
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
  /* evaluate the expanded lsystem */
  void evaluate_system();

  /* evaluate a symbol in the lsystem, changing the turtle's state */
  void evaluate_symbol(const char symbol);

  /* --- evaluation functions --- */

  void move_forward();

  void rotate(glm::dvec3 axis, bool reverse);

  /* --- state and stack --- */
  turtle_state state;
  std::stack<turtle_state> state_stack;

  /* vertices, indices, and normals of the evaluated lsystem */
  std::vector<glm::dvec4> vertices;
  std::vector<unsigned int> indices;

  /* --- turtle parameters --- */
  float move_magnitude; // magnitude of translations
  double rotation_angle; // magnitude of rotations

  std::string system_string;

};

#endif
