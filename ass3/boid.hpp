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

  static const float MAX_ACCELERATION;

  /* constructor, argument is starting position and member tribe */
  boid(const glm::vec3 startPos, enum tribes tribe);

  /* update the position, velocity, and acceleration based on timestep */
  void step(const float timestep);

  /* add acceleration to the boid's acceleration */
  void addAcceleration(const glm::vec3 acceleration);

  /* set the acceleration of the boid */
  void setAcceleration(const glm::vec3 acceleration);

  /* set the x coordinate that the boid has to reach */
  void setGoal(const glm::vec3 goal);

  /* set the bounds that the boid should not cross over */
  void setBounds(const float x1, const float y1,
    const float x2, const float y2);

  /* return position */
  glm::vec3 getPosition();

  /* return velocity */
  glm::vec3 getVelocity();

  /* return forward direction of boid */
  glm::vec3 getDirection();

  /* return goal direction */
  glm::vec3 goalDirection();

  /* return tribe */
  enum tribes getTribe();

private:
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;

  glm::vec3 goal;

  float bounds[4];

  const enum tribes tribe;

  bool atGoal();
};

#endif
