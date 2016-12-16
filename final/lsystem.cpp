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
  file >> state.magnitude;
  file >> this->rotation_angle;
  this->rotation_angle = glm::radians(this->rotation_angle);

  printf("Generations: %d magnitude: %f angle: %f\n",generations,state.magnitude,this->rotation_angle);

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

  file.close();

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
    //printf("current axiom: %s\n",axiom.c_str());
  }

  //printf("final string: %s\n",axiom.c_str());
  /* set system_string to the expanded axiom */
  this->system_string = axiom;

  /*  now that we have the system, evaluate it*/
  evaluate_system();

  /* after system is completely evaluated, create 'tubes' */
  construct_tubes();
}

void lsystem::evaluate_system(){
  /* step through each symbol in system and evaluate it */
  for(size_t i = 0; i < system_string.length();++i){
    evaluate_symbol(system_string.at(i));
  }
  /* set last vertex to leaf */
  segment_spec[segment_spec.size()-1] = LEAF;
}

void lsystem::evaluate_symbol(char symbol){
  switch(symbol){
  /* go forward */
  case 'f':
    move_forward();
    break;

  /* turn right about z-axis */
  case '+':
    rotate(glm::dvec3(0.0,0.0,1.0),true);
    break;

  /* turn left about z-axis */
  case '-':
    rotate(glm::dvec3(0.0,0.0,1.0),false);
    break;

  /* turn right about y-axis */
  case '/':
    rotate(glm::dvec3(0.0,1.0,0.0),true);
    break;

  /*turn left about y-axis */
  case '\\':
    rotate(glm::dvec3(0.0,1.0,0.0),true);
    break;

  /* push stack */
  case '[':
    state_stack.push(state);
    break;

  /* pop stack */
  case ']':
    assert(!state_stack.empty());
    /* on a stack pop, current vertex before pop will be a leaf node */
    segment_spec[state.index] = LEAF;
    leaf_points.push_back(state.position);
    state = state_stack.top();
    state_stack.pop();
    break;

  /* shorten distances */
  case 's':
    state.magnitude *= 0.8;
    break;

  /* silently do nothing */
  default:
    break;
  }
}

/* swaps the largest components and sets the remaining
 * component to negative to construct a non-colinear vector
 */
glm::vec3 swap_largest(glm::vec3 v){
  glm::vec3 ret;
  if(v.x > v.y && v.x > v.z){
    if(v.y > v.z)
      ret = glm::vec3(v.y,v.x,-v.z);
    else
      ret = glm::vec3(v.z,-v.y,v.x);
  }else if (v.y > v.x && v.y > v.z){
    if (v.x > v.z)
      ret = glm::vec3(v.y,v.x,-v.z);
    else
      ret = glm::vec3(-v.x,v.z,v.y);
  }else if (v.z > v.x && v.z > v.y){
    if(v.x > v.y)
      ret = glm::vec3(v.z,-v.y,v.x);
    else
      ret = glm::vec3(-v.x,v.z,v.y);
  }

  return ret;
}

/* swizzle a 4-vector to a 3-vector */
glm::vec3 swiz43(const glm::vec4 v){
  return glm::vec3(v.x,v.y,v.z);
}

void lsystem::construct_tubes(){
  std::vector<glm::vec4> sections;
  std::vector<glm::vec3> s_norms;
  std::vector<unsigned int> s_ind;

  /* construct vertices about each vertex in lsystem to form tubes */
  for(size_t i = 0; i < this->indices.size(); i+=2){
    /* get the base and next position of the section */
    const glm::vec3 curr = swiz43(this->vertices[this->indices[i]]);
    const glm::vec3 next = swiz43(this->vertices[this->indices[i+1]]);

    const glm::vec3 line = next - curr;
    const glm::vec3 nonco = swap_largest(line);

    /* get normal and binormal to line of section */
    const glm::vec3 norm = glm::normalize(glm::cross(line,nonco));
    const glm::vec3 binorm = glm::normalize(glm::cross(line,norm));

    /* new vertices
     * direction meaning is arbitrary, what's important is that it's
     * a square around the point
     */
    const glm::vec4 up(curr+norm,1.0f);
    const glm::vec4 right(curr+binorm,1.0f);
    const glm::vec4 down(curr-norm,1.0f);
    const glm::vec4 left(curr-binorm,1.0f);

    /* add the section with vertex normal */
    sections.push_back(up);
    s_norms.push_back(swiz43(up) - curr);
    s_ind.push_back(sections.size()-1);

    sections.push_back(right);
    s_norms.push_back(swiz43(right) - curr);
    s_ind.push_back(sections.size()-1);

    sections.push_back(down);
    s_norms.push_back(swiz43(down) - curr);
    s_ind.push_back(sections.size()-1);

    sections.push_back(left);
    s_norms.push_back(swiz43(left) - curr);
    s_ind.push_back(sections.size()-1);
  }

  /* link vertices to form tubes */
  std::vector<unsigned int> t_ind;
  for(unsigned int i = 0; i < s_ind.size();i+=4){
    /* go through one segment (4 vertices) at a time and connect them
     * to the next segment; skip leaf segments since they do not
     * connect forward
     */
    if(segment_spec[i/4] == LEAF) continue;

    /* link the: first side */
    t_ind.push_back(i);
    t_ind.push_back(i+1);
    t_ind.push_back(i+4);

    t_ind.push_back(i+4);
    t_ind.push_back(i+1);
    t_ind.push_back(i+5);

    /* second side */
    t_ind.push_back(i+3);
    t_ind.push_back(i);
    t_ind.push_back(i+7);

    t_ind.push_back(i+7);
    t_ind.push_back(i);
    t_ind.push_back(i+4);

    /* third side */
    t_ind.push_back(i+2);
    t_ind.push_back(i+3);
    t_ind.push_back(i+6);

    t_ind.push_back(i+6);
    t_ind.push_back(i+3);
    t_ind.push_back(i+7);

    /* fourth side */
    t_ind.push_back(i+1);
    t_ind.push_back(i+2);
    t_ind.push_back(i+5);

    t_ind.push_back(i+5);
    t_ind.push_back(i+2);
    t_ind.push_back(i+6);
  }

  /* replace vertices, normals, indices */
  this->vertices = sections;
  this->normals = s_norms;
  this->indices = t_ind;
}



void lsystem::move_forward(){
  const unsigned int old_index = this->state.index;

  this->state.position += (this->state.direction * state.magnitude);

  /* make vertices and stuff */
  this->vertices.push_back(this->state.position);
  this->segment_spec.push_back(GEN);
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
}
