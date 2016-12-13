#include "lsystem.hpp"
#include <unordered_map>
#include <fstream>
#include <cstdio>
#include <cmath>

lsystem::lsystem(const glm::vec4 origin,
  const glm::vec4 direction):
  state(origin,direction)
{
  this->vertices.push_back(origin);
}

void lsystem::readlsystem(const std::string &filename){
  std::unordered_map<std::string,std::string> productions;
  std::fstream file(filename);

  /* get parameters and productions from file, format is:
  generations magnitude angle
  axiom
  rule production
  */
  unsigned int generations;
  file >> generations;
  file >> this->move_magnitude;
  file >> this->rotation_angle;
  this->rotation_angle = glm::radians(this->rotation_angle);

  printf("%d %f %f\n",generations,this->move_magnitude,this->rotation_angle);

  std::string axiom;
  file >> axiom;
  printf("axiom -> %s\n",axiom.c_str());
  while(!file.eof()){
    std::string rule;
    std::string prod;
    file >> rule;
    file >> prod;
    /* safety check so we aren't reading nothing */
    if(rule.length() > 0)
      productions[rule] = prod;
  }

  for(auto iter = productions.begin(); iter != productions.end();++iter){
    printf("%s: %s\n",iter->first.c_str(),iter->second.c_str());
  }

  /* expand the axiom using the rules */
  for(unsigned int i = 0; i < generations;++i){
    const std::string current = axiom;
    std::string window;
    std::string next_string("");
    unsigned int last = 0;

    for(unsigned int j = 0; j < current.length();++j){
      window = current.substr(last,(j-last)+1);
      /* check if either rules are in the substring */
      for(auto iter = productions.begin(); iter != productions.end();++iter){
        size_t pos = window.find(iter->first);

        // production found
        if(pos != std::string::npos){
          next_string.append(window.substr(0,pos));
          next_string.append(iter->second);
          last = j+1;
          window = "";
        }
      }
    }

    /* need to add remaining bit of window left */
    if(window.length() > 0){
      next_string.append(window);
    }

    axiom = next_string;
    printf("current axiom: %s\n",axiom.c_str());
  }

  printf("final string: %s\n",axiom.c_str());
  /* set system_string to the expanded axiom */
  this->system_string = axiom;
  evaluate_system();
}

void lsystem::evaluate_system(){
  for(size_t i = 0; i < system_string.length();++i){
    evaluate_symbol(system_string.at(i));
  }
}

void lsystem::evaluate_symbol(char symbol){
  switch(symbol){
  /* go forward */
  case 'f':
    move_forward();
    break;

  /* turn right */
  case '+':
    rotate(glm::dvec3(0.0,0.0,1.0),true);
    break;

  /* turn left */
  case '-':
    rotate(glm::dvec3(0.0,0.0,1.0),false);
    break;

  /* push stack */
  case '[':
    state_stack.push(state);
    break;

  /* pop stack */
  case ']':
    assert(!state_stack.empty());
    state = state_stack.top();
    state_stack.pop();
    break;

  /* silently do nothing */
  default:
    break;
  }
}

void lsystem::move_forward(){
  const unsigned int old_index = this->state.index;
  this->state.position += this->state.direction;

  /* make vertices and stuff */
  this->vertices.push_back(this->state.position);
  this->normals.push_back(glm::vec3(0.0f,1.0f,0.0f));
  this->state.index = this->vertices.size() - 1;
  this->indices.push_back(old_index);
  this->indices.push_back(this->state.index);
}

/* multiplication of 2 quaternions */
glm::dvec4 quat_prod(const glm::dvec4 &Q1, const glm::dvec4 &Q2){
  const double w1 = Q1.w;
  const double w2 = Q2.w;
  const glm::dvec3 v1(Q1.x,Q1.y,Q1.z);
  const glm::dvec3 v2(Q2.x,Q2.y,Q2.z);

  const double wf = w1*w2 - glm::dot(v1,v2);
  const glm::dvec3 vf(glm::cross(v1,v2) + (w1*v2) + (w2*v1));
  return glm::dvec4(vf.x,vf.y,vf.z,wf);
}

/* inversion of a quaternion */
glm::dvec4 quat_inv(const glm::dvec4 &Q){
  const double qnorm = sqrt(Q.x*Q.x +
    Q.y*Q.y +
    Q.z*Q.z +
    Q.w*Q.w);
  const glm::dvec4 Q_con(-Q.x,-Q.y,-Q.z,Q.w);

  return glm::dvec4(Q_con/qnorm);
}

/* solution inspired by http://paulbourke.net/geometry/rotate/ */
void lsystem::rotate(glm::dvec3 axis, bool reverse){
  axis = glm::normalize(axis);

  /* make quaternions */
  const glm::dvec4 Q2(axis.x*sin(rotation_angle/2.0),
    axis.y*sin(rotation_angle/2.0),
    axis.z*sin(rotation_angle/2.0),
    cos(rotation_angle/2.0));

  const glm::dvec4 Q2_inv(quat_inv(Q2));

  glm::dvec4 Q3;

  if(reverse){
    Q3 = quat_prod(quat_prod(Q2,state.direction),Q2_inv);
  }else{
    Q3 = quat_prod(quat_prod(Q2_inv,state.direction),Q2);
  }

  state.direction = glm::dvec4(Q3.x,Q3.y,Q3.z,0.0f);

  //printf("new direction %f %f %f\n",state.direction.x,state.direction.y,state.direction.z);
}
