#include "boid.hpp"
#include <cstdio>

//#define FLIPPING

const float boid::FLOCK_RADIUS = 10.0f;
const float boid::MAX_ACCELERATION = 5.0f;

float boid::bounds[4] = {0.0f,0.0f,0.0f,0.0f};

boid::boid(const glm::vec3 startPos, enum tribes tribe):
  position(startPos),
  velocity(glm::vec3(0.0f,0.0f,0.0f)),
  acceleration(glm::vec3(0.0f,0.0f,0.0f)),
  goal(0.0f),
  tribe(tribe){}

void boid::step(const float timestep){
  const float acceleration_decay = 1.0f;
  if(!atGoal()){
    this->velocity += this->acceleration*timestep;
    this->position += this->velocity*timestep;

    /* bounds checking */
    if(this->position.x < bounds[0]){
      this->position.x = bounds[0];
      this->velocity.x = 0.0f;
    }
    else if(this->position.x > bounds[2]){
      this->position.x = bounds[2];
      this->velocity.x = 0.0f;
    }

    if(this->position.z < bounds[1]){
      this->position.z = bounds[1];
      this->velocity.z = 0.0f;
    }
    else if(this->position.z > bounds[3]){
      this->position.z = bounds[3];
      this->velocity.z = 0.0f;
    }

    /* decay acceleration */
    //if(glm::length(this->acceleration) != 0){
      this->acceleration *= acceleration_decay*timestep;
    //}
  }
  #ifdef FLIPPING
  else{
    flipGoal();
  }
  #endif
}

void boid::addAcceleration(const glm::vec3 accel){
  const glm::vec3 newAccel(this->acceleration+accel);
  const float mag = glm::length(newAccel);
  if(mag < boid::MAX_ACCELERATION){
    this->acceleration = newAccel;
  }else if (!glm::isinf(mag) && !glm::isnan(mag)){
    /* cap the acceleration */
    this->acceleration = glm::normalize(newAccel)*boid::MAX_ACCELERATION;
  }
}

void boid::setAcceleration(const glm::vec3 accel){
  this->acceleration = accel;
}

void boid::setGoal(const glm::vec3 goal){
  this->goal = goal;
}

void boid::setBounds(const float x1, const float y1, const float x2, const float y2){
  if(boid::bounds[0] == 0.0f){
      bounds[0] = x1;
      bounds[1] = y1;
      bounds[2] = x2;
      bounds[3] = y2;
    }
  }

bool boid::atGoal(){
  const float GOAL_RADIUS = 2.0f;
  return glm::distance(this->position,this->goal) < GOAL_RADIUS;
}

glm::vec3 boid::getPosition() const{
  return this->position;
}

glm::vec3 boid::getVelocity() const{
  return this->velocity;
}

glm::vec3 boid::getDirection() const{
  return glm::normalize(this->velocity);
}

glm::vec3 boid::goalDirection() const{
  const glm::vec3 direction = glm::normalize(this->goal - this->position);
  return glm::vec3(direction.x,0,direction.z);
}

enum tribes boid::getTribe() const {
  return this->tribe;
}

void boid::flipGoal(){
  this->goal = -this->goal;
  this->tribe = tribe == RED ? BLUE : RED;
}
