#include "boid.hpp"

boid::boid(const glm::vec3 startPos, enum tribes tribe):
  position(startPos),
  velocity(glm::vec3(0.0f,0.0f,0.0f)),
  acceleration(glm::vec3(0.0f,0.0f,0.0f)),
  tribe(tribe){}

void boid::step(const float timestep){
  this->position += this->velocity*timestep;
  this->velocity += this->acceleration*timestep;
}

void boid::addAcceleration(const glm::vec3 accel){
  this->acceleration += accel;
}

void boid::setAcceleration(const glm::vec3 accel){
  this->acceleration = accel;
}

glm::vec3 boid::getPosition(){
  return this->position;
}

enum tribes boid::getTribe(){
  return this->tribe;
}
