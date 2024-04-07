#ifndef MESH_DATA_H
#define MESH_DATA_H
#include <vector>
#include <list>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"


#include "Mesh_info.h"
namespace CeEngine {
typedef unsigned int uint;
class Mesh_data{
    GL_layer::Mesh_info info;

public:
    std::vector<float>  vertex;
    std::vector<uint>   indexes;

    template<GL_layer::TYPE_data type>
    void set_marking_vertices(uint cout){
        info.set<type>(cout);
    }

    template<std::size_t type_texture>
    void set_marking_texture(uint cout){
        info.set_texture<type_texture>(cout);
    }

    const GL_layer::Mesh_info & get_info() const{
        return info;
    }

    std::vector<glm::vec2> normalaize(unsigned int begin_offset, unsigned int end_offset);

    template<GL_layer::TYPE_data type>
    auto normalaize(){
        unsigned int begin_offset=0;
        for (std::size_t i=0;i <static_cast<int>(type) ;i++) {
            begin_offset+=info.get(type);//придумать что-то элегантное
        }

        return normalaize(begin_offset,begin_offset+info.get<type>());
    }

    Mesh_data(std::list<glm::vec3> & point_data,std::vector<std::pair<uint,uint>> edges, std::vector<float> data_outher={},int data_step=3);

    Mesh_data(std::vector<float> &&vertex_,std::vector<uint> &&indexes_):vertex(std::move(vertex_)),indexes(std::move(indexes_)){}
    Mesh_data(std::vector<float> &&vertex_):vertex(std::move(vertex_)){}
    Mesh_data(){}
};
}// namespace CeEngine
#endif // MESH_DATA_H
