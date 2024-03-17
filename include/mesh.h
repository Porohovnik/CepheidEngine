#ifndef MESH_H
#define MESH_H
#include <vector>
#include <memory>

#include "Mesh_info.h"

namespace CeEngine {
typedef unsigned int uint;

struct Mesh_data{
    std::vector<float>  vertex;
    std::vector<uint>   indexes;
    Mesh_data(std::vector<float> &&vertex_,std::vector<uint> &&indexes_):vertex(std::move(vertex_)),indexes(std::move(indexes_)){}
    Mesh_data(std::vector<float> &&vertex_):vertex(std::move(vertex_)){}
};

class Mesh{
    GL_layer::Mesh_info info;
    std::shared_ptr<Mesh_data> data=nullptr;
public:
    Mesh(GL_layer::Mesh_info info_, std::shared_ptr<Mesh_data> data_):info(info_),data(data_){}

    const GL_layer::Mesh_info & get_info() const{
        return info;
    }

    //vertex
    float * data_vertex(){
        return data->vertex.data();
    }
    std::size_t size_vertex(){
        return data->vertex.size();
    }

    //indeces
    uint * data_indexes(){
        return data->indexes.data();
    }

    std::size_t size_indexes(){
        return data->indexes.size();
    }

    std::shared_ptr<Mesh_data> get_data () const{
        return data;
    }
};

Mesh   line_mesh();
Mesh  plate_mesh();
Mesh skybox_mesh();
Mesh    box_mesh();
}// namespace CeEngine
#endif // MESH_H
