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
    productions[rule] = prod;
  }

  for(auto iter = productions.begin(); iter != productions.end();++iter){
    printf("%s: %s\n",iter->first.c_str(),iter->second.c_str());
  }

  /* expand the axiom using the rules */
  for(unsigned int i = 0; i < generations;++i){
    std::string current(axiom);
    for(unsigned int j = 0; j < current.length();++j){
      
    }
    printf("%s\n",current.c_str());
  }

  printf("final string: %s\n",axiom.c_str());
  /* set system_string to the expanded axiom */
  this->system_string = axiom;
}

void evaluate_symbol(char symbol){
  switch(symbol){
    case 'f':
      // go forward
      break;
    default:
      /* silently nothing */
      break;
  }
}
