#include "VAO.h"
#include "tiny_obj_loader.h"

void VAO_loadObj(char *filename, struct VAO *vao){
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err = tinyobj::LoadObj(shapes, materials, filename, 0);
}
