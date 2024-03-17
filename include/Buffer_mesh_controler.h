#ifndef BUFFER_MESH_CONTROLER_H
#define BUFFER_MESH_CONTROLER_H
#include "draw_program.h"
#include <functional>

namespace CeEngine {
class Buffer_mesh_controller{
    std::size_t first=0;
    std::size_t size_data=0;
    std::function<void()> deleter=nullptr;
public:
    template<typename VAO_, typename D_VBO, typename D_EBO, typename D_cmd, typename D_cmd_m, typename D_mesh, typename Mesh_data>
    Buffer_mesh_controller(VAO_ VAO__, D_VBO VBO, D_EBO EBO,D_cmd * cmd,D_cmd_m * cmd_m,D_mesh d_mesh,int id_bd, Mesh_data data){
        //std::cout<<"Buffer_mesh_controller"<<std::endl;

        first=VBO->add_element(id_bd,std::move(data->vertex));//тут всё равно нужно добавлять данные, в любом случае
        size_data=data->vertex.size();
        if constexpr(!std::is_same_v<D_EBO,std::nullptr_t>){
            first=EBO->add_element(id_bd,std::move(data->indices));
            size_data=data->indexes.size();
        }

        deleter=[VBO,EBO,id_bd](){
            VBO!=nullptr?VBO->delete_element(id_bd):0;
            if constexpr(!std::is_same_v<D_EBO,std::nullptr_t>){
                EBO!=nullptr?EBO->delete_element(id_bd):0;
            }
        };

        (*VAO__)->Get_VRAM().attach(VBO,EBO);

        //для ассинхроности загрузки данных
        auto C_cmd=cmd->template change_request();
        auto C_cmd_manadger=cmd_m->template change_request();

        for(auto &T:(*C_cmd_manadger.get_data())){
            if(T.id_mesh==id_bd){
                //std::cout<<T.id_mesh<<">>Buffer_mesh_controller:"<<size_data<<std::endl;
                if constexpr(std::is_same_v<decltype(T.cmd),GL_layer::DrawArraysIndirectCommand>){
                    T.cmd.count=size_data;
                    T.cmd.first=first;
                }
                if constexpr(std::is_same_v<decltype(T.cmd),GL_layer::DrawElementsIndirectCommand>){
                    T.cmd.count=size_data;
                    T.cmd.firstIndex=first;
                }

                (*C_cmd.get_data())[T.id]=T.cmd;
            }
        }
        d_mesh->add_element(id_bd,{static_cast<int>(first) ,static_cast<int>(size_data)});
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
