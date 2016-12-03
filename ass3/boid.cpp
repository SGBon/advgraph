#include "boid.hpp"

const float boid::FLOCK_RADIUS = 5.0f;
const float boid::MAX_ACCELERATION = 5.0f;

boid::boid(const glm::vec3 startPos, enum tribes tribe):
  position(startPos),
  velocity(glm::vec3(0.0f,0.0f,0.0f)),
  acceleration(glm::vec3(0.0f,0.0f,0.0f)),
  goal(0.0f),
  tribe(tribe){}

void boid::step(const float timestep){
  const float acceleration_decay = 0.3f;
  if(!atGoal()){
    this->velocity += this->acceleration*timestep;
    this->position += this->velocity*timestep;

    /* bounds checking */
    if(this->position.x < bounds[0])
      this->position.x = bounds[0];
    else if(this->position.x > bounds[2])
      this->position.x = bounds[2];

    if(this->position.z < bounds[1])
      this->position.z = bounds[1];
    else if(this->position.z > bounds[3])
      this->position.z = bounds[3];

    /* decay acceleration */
    if(glm::length(this->acceleration) > 0){
      this->acceleration *= acceleration_decay*timestep;
    }else if(glm::length(this->acceleration) < 0){
      this->acceleration *= acceleration_decay*timestep;
    }
  }
}

void boid::addAcceleration(const glm::vec3 accel){
  const glm::vec3 newAccel(this->acceleration+accel);
  if(glm::length(newAccel) < boid::MAX_ACCELERATION)
    this->acceleration = newAccel;
}

void boid::setAcceleration(const glm::vec3 accel){
  this->acceleration = accel;
}

void boid::setGoal(const float goal){
  this->goal = goal;
}

void boid::setBounds(const float x1, const float y1, const float x2, const float y2){
      this->bounds[0] = x1;
      this->bounds[1] = y1;
      this->bounds[2] = x2;
      this->bounds[3] = y2;
  }

bool boid::atGoal(){
  if(this->tribe == RED){
    return position.x < goal;
  }else{
    return position.x > goal;
  }
}

glm::vec3 boid::getPosition(){
  return this->position;
}

glm::vec3 boid::getVelocity(){
  return this->velocity;
}

glm::vec3 boid::getDirection(){
  return glm::normalize(this->velocity);
}

float boid::goalDirection(){
  return abs(goal)/goal;
}

enum tribes boid::getTribe(){
  return this->tribe;
}
