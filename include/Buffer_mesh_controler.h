#ifndef BUFFER_MESH_CONTROLER_H
#define BUFFER_MESH_CONTROLER_H
#include "draw_program.h"
#include <functional>

#include <iostream>
#include <interlayer_gl.h>
namespace CeEngine {
class Buffer_mesh_controller{

    std::function<void()> deleter=nullptr;
public:
    template<typename VAO_, typename D_VBO, typename D_EBO,typename Mesh_data>
    Buffer_mesh_controller(VAO_ VAO__, D_VBO VBO,std::size_t VBO_mark_first ,D_EBO EBO,std::size_t EBO_mark_first ,int id_bd, Mesh_data data){
        //std::cout<<"Buffer_mesh_controller"<<std::endl;

        VBO->add_element(id_bd,VBO_mark_first,std::move(data->vertex));//тут всё равно нужно добавлять данные, в любом случае
        if constexpr(!std::is_same_v<D_EBO,std::nullptr_t>){
            EBO->add_element(id_bd,EBO_mark_first,std::move(data->indexes));
        }

        deleter=[VBO,EBO,id_bd](){
            VBO!=nullptr?VBO->delete_element(id_bd):0;
            if constexpr(!std::is_same_v<D_EBO,std::nullptr_t>){
                EBO!=nullptr?EBO->delete_element(id_bd):0;
            }
        };

        (*VAO__)->Get_VRAM().attach(VBO,EBO);

    }

    ~Buffer_mesh_controller(){
        if(deleter!=nullptr){
            //std::cout<<"||||||||||||||||||||||Buffer_mesh_controller|||||||||||||:"<<std::endl;
            deleter();
        }
    }
};
}// namespace CeEngine
#endif // BUFFER_MESH_CONTROLER_H
