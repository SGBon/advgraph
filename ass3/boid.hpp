#ifndef TRIBES_BOID_H
#define TRIBES_BOID_H

#include <glm/glm.hpp>

enum tribes{
  RED,
  BLUE
};

class boid{
public:
  /* detection radius around boid */
  static const float FLOCK_RADIUS;

  /* constructor, argument is starting position and member tribe */
  boid(const glm::vec3 startPos, enum tribes tribe);

  /* update the position, velocity, and acceleration based on timestep */
  void step(const float timestep);

  /* add acceleration to the boid's acceleration */
  void addAcceleration(const glm::vec3 acceleration);

  /* set the acceleration of the boid */
  void setAcceleration(const glm::vec3 acceleration);

  /* set the x coordinate that the boid has to reach */
  void setGoal(const float goal);

  /* return position */
  glm::vec3 getPosition();

  /* return velocity */
  glm::vec3 getVelocity();

  /* return forward direction of boid */
  glm::vec3 getDirection();

  /* return tribe */
  enum tribes getTribe();

private:
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;

  float goal;

  const enum tribes tribe;

  bool atGoal();
};

#endif
