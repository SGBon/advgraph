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
  double magnitude;
  double width;
  unsigned int index = 0; /* vertice index for linking meshes */

  turtle_state(glm::vec4 position,glm::vec4 direction):
  position(position),
  direction(direction),
  magnitude(1),
  width(4)
  {}
};

class lsystem{
  /* specifies if vertex/segment is a general/leaf vertex/segment */
  enum seg_spec{
    GEN,
    LEAF
  };

public:
  lsystem(const glm::vec4 origin = glm::vec4(0.0,0.0,0.0,1.0),
    const glm::vec4 direction = glm::vec4(0.0,1.0,0.0,0.0));

  /* read and expand the lsystem specifier in the file at filename */
  void readlsystem(const std::string &filename);

  /* vertices, indices, and normals of the evaluated lsystem */
  std::vector<glm::vec4> vertices;
  std::vector<seg_spec> segment_spec;
  std::vector<glm::vec3> normals;
  std::vector<unsigned int> indices;

  /* points on lsystem where there are leaves */
  std::vector<glm::vec4> leaf_points;

private:
  /* evaluate the expanded lsystem */
  void evaluate_system();

  /* evaluate a symbol in the lsystem, changing the turtle's state */
  void evaluate_symbol(const char symbol);

  /* construct tubes */
  void construct_tubes();

  /* --- evaluation functions --- */

  void move_forward();

  void rotate(glm::dvec3 axis, bool reverse);

  /* --- state and stack --- */
  turtle_state state;
  std::stack<turtle_state> state_stack;

  /* --- turtle parameters --- */
  double rotation_angle; // magnitude of rotations

  std::string system_string;

};

#endif
