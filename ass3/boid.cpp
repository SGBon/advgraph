#include "boid.hpp"


boid::boid(const glm::vec3 startPos, enum tribes tribe):
  position(startPos),
  velocity(glm::vec3(0.0f,0.0f,0.0f)),
  acceleration(glm::vec3(0.0f,0.0f,0.0f)),
  goal(0.0f),
  tribe(tribe){}

void boid::step(const float timestep){
  const float acceleration_decay = -0.1f;
  if(!atGoal()){
    this->position += this->velocity*timestep;
    this->velocity += this->acceleration*timestep;

    /* decay acceleration */
    if(this->acceleration.x > 0){
      this->acceleration.x += acceleration_decay*timestep;
    }else if(this->acceleration.x < 0){
      this->acceleration.x -= acceleration_decay*timestep;
    }
  }
}

void boid::addAcceleration(const glm::vec3 accel){
  this->acceleration += accel;
}

void boid::setAcceleration(const glm::vec3 accel){
  this->acceleration = accel;
}

void boid::setGoal(const float goal){
  this->goal = goal;
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

enum tribes boid::getTribe(){
  return this->tribe;
}
