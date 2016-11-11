/*
 *  Simple fragment sharder for Lab 2
 */

#version 330 core

in vec3 normal;
in vec3 position;
uniform samplerCube tex;
uniform vec3 Eye;
out vec4 fragcolor;

/* compute R0 coefficient in schlick approximation */
float schlick_R0(const float eta){
  return pow((1.0f-eta)/(1.0f+eta),2.0f);
}

/* schlick aproximation */
float schlick_approx(const vec3 incident,const vec3 normal, const float R0){
  vec3 ninc = normalize(incident);
  vec3 nnorm = normalize(normal);
  float idotn = max(0.0f,dot(ninc,nnorm));

  return R0 + (1.0f-R0)*pow((1.0f-idotn),5.0f);
}

void main() {
  const float glass_eta = 1.6f;
  float coef = schlick_approx(Eye,normal,schlick_R0(glass_eta));

  vec3 tc = reflect(-position,normal);
  vec4 fleccomp = texture(tex,tc); /* reFLECtion component */
  tc = refract(position - Eye,normal,glass_eta);
  vec4 fraccomp = texture(tex,tc); /* reFRACtion component */

  fragcolor = mix(fraccomp,fleccomp,coef);
}
