#ifndef MESH_H
#define MESH_H
#include <memory>
#include "Mesh_data.h"

namespace CeEngine {
std::shared_ptr<Mesh_data>   line_mesh();
std::shared_ptr<Mesh_data>    sss_mesh();
std::shared_ptr<Mesh_data>  plate_mesh();
std::shared_ptr<Mesh_data> skybox_mesh();
std::shared_ptr<Mesh_data>    box_mesh();
}// namespace CeEngine
#endif // MESH_H
