#ifndef MESH_H
#define MESH_H
#include <memory>
#include "Mesh_data.h"

namespace CeEngine {
std::shared_ptr<Mesh_data>   line_mesh();

enum TYPE_JOINT{TENSION=0,OBLIQUE,STRAIGHT,ROUNDED};
std::shared_ptr<Mesh_data>   line_mesh(std::vector<float> &vertices,float width,TYPE_JOINT type=TENSION);
std::shared_ptr<Mesh_data>  plate_mesh();
std::shared_ptr<Mesh_data> skybox_mesh();
std::shared_ptr<Mesh_data>    box_mesh();
}// namespace CeEngine
#endif // MESH_H
