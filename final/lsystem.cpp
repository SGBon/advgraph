#include "lsystem.hpp"
#include <unordered_map>
#include <fstream>
#include <cstdio>

lsystem::lsystem(const glm::vec4 origin,
  const glm::vec4 direction):
  state(origin,direction)
{}

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
      //printf("window: %d %d %s\n",last,j,window.c_str());
      /* check if either rules are in the substring */
      for(auto iter = productions.begin(); iter != productions.end();++iter){
        size_t pos = window.find(iter->first);

        // production found
        if(pos != std::string::npos){
          // append everything before the production
          next_string.append(window.substr(0,pos));
          next_string.append(iter->second);
          last = j+1;
        }
      }
    }

    axiom = next_string;
    printf("current axiom: %s\n",axiom.c_str());
  }

  printf("final string: %s\n",axiom.c_str());
  /* set system_string to the expanded axiom */
  this->system_string = axiom;
}

void lsystem::evaluate_symbol(char symbol){
  switch(symbol){
    /* go forward */
    case 'f':
      
      break;

    /* turn right */
    case '+':
      break;

    /* turn left */
    case '-':
      break;

    /* push stack */
    case '[':
      this->state_stack.push(state);
      break;

    /* pop stack */
    case ']':
      this->state = state_stack.top();
      state_stack.pop();
      break;

    /* silently do nothing */
    default:
      break;
  }
}
