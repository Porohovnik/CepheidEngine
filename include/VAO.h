#ifndef VAO_H
#define VAO_H
#include <type_traits>

#include "Buffer_id.h"
#include "Mesh_info.h"

#include "GL_type_to_cpp.h"
namespace CeEngine {
class VAO{
    GL_layer::Buffer_id<GL_layer::TYPE_BUFFER::VERTEX_ARRAY> buffer;
    const GL_layer::Mesh_info info;
public:
    VAO(const GL_layer::Mesh_info info_):buffer(),info(info_){}

    template<typename VBO, typename EBO>
    VAO(const GL_layer::Mesh_info info_,VBO vbo, EBO ebo):buffer(),info(info_){
        attach(vbo,ebo);
    }

    template<typename VBO, typename EBO>
    void attach(VBO vbo, EBO ebo){
        buffer.Bind_();
        vbo->Bind();
        info.set_atribute_Mesh_buffer();
        if constexpr(!std::is_same_v<EBO,std::nullptr_t>){
            ebo->Bind();
        }
        GL_layer::Bind<GL_layer::TYPE_BUFFER::VERTEX_ARRAY>(0);
        GL_layer::Bind<GL_layer::TYPE_BUFFER::ARRAY_BUFFER> (0);

        GL_layer::Bind<GL_layer::TYPE_BUFFER::ELEMENT_ARRAY_BUFFER>   (0);
    }

    inline void Bind([[maybe_unused]] int base, [[maybe_unused]] int id=-1){
        buffer.Bind_();
    }
};
}// namespace CeEngine
#endif // VAO_H
