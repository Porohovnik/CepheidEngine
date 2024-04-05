#ifndef BUFFER_MESH_H
#define BUFFER_MESH_H
#include "VAO.h"
#include "buffer_gl.h"
namespace CeEngine {
class Buffer_mesh{
    Buffer_GL<float,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::ARRAY_BUFFER> VBO;
    Buffer_GL<uint, GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::ELEMENT_ARRAY_BUFFER> EBO;

    VAO VAO_;//Может вызывать падения рендера
public:
    template<typename Mesh>
    Buffer_mesh(Mesh mesh):VBO(mesh->vertex),EBO(mesh->indexes),VAO_(mesh->get_info(),&VBO,&EBO){}

    inline void Bind(int base,int id=-1){
        VAO_.Bind(base,id);
    }
};
}// namespace CeEngine

#endif // BUFFER_MESH_H
