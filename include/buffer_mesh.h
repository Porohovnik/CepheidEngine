#ifndef BUFFER_MESH_H
#define BUFFER_MESH_H
#include <iostream>

#include "mesh.h"
#include "buffer_gl.h"
#include "draw_program.h"
#include <functional>


template<GLenum n>
class VAO{
    GL_layer::Buffer_id<GL_VERTEX_ARRAY> buffer=0;
    const Mesh_info info;

public:
    VAO(const Mesh_info info_):buffer(GL_layer::create_buffer<GL_VERTEX_ARRAY>()),info(info_){}
    ~VAO(){
        GL_layer::delete_buffer(buffer);
    }

    template<typename VBO, typename EBO>
    void attach(VBO * vbo, EBO * ebo){
        GL_layer::Bind_guard bind_{buffer};
        vbo->Bind();
        GL_layer::set_atribute_Mesh_buffer(info);
        ebo->Bind();
    }

    template<typename VBO>
    void attach(VBO * vbo){
        GL_layer::Bind_guard bind_{buffer};
        vbo->Bind();
        GL_layer::set_atribute_Mesh_buffer(info);
    }


    inline void Bind([[maybe_unused]] int base, [[maybe_unused]] int id=-1) const{
        GL_layer::Bind_(buffer);
    }
};


class Buffer_mesh{
    VAO<GL_VERTEX_ARRAY> VAO_;//эта хрень может вызывать падения рендера

    Buffer_GL<float,GL_ARRAY_BUFFER> VBO;
    Buffer_GL<uint,GL_ELEMENT_ARRAY_BUFFER> EBO;
public:
    using Data=float;

    //для хранения нескольких мешей в одном буффере,должны быть одинаковые настройки
    Buffer_mesh(Mesh mesh):
        VAO_(mesh.get_info()),
        VBO(-1,GL_STATIC_DRAW,mesh.size_vertex(),mesh.data_vertex()),
        EBO(-1,GL_STATIC_DRAW,mesh.size_indeces(),mesh.data_indeces()) {

        VAO_.attach(&VBO,&EBO);
        std::cout<<"VAO:"<<"VBO:"<<mesh.size_vertex()<<std::endl;
    }

    inline void Bind(int base,int id=-1) const{
        VAO_.Bind(base,id);
    }

    ~Buffer_mesh(){
         std::cout<<"||||||||||||||||||||||Buffer_mesh|||||||||||||:"<<std::endl;
    }
};

class Buffer_mesh_controller{
    std::size_t first=0;
    std::size_t size_data=0;
    std::function<void()> deleter=nullptr;

    template<typename D_cmd, typename D_cmd_m >
    void construct(D_cmd * cmd,D_cmd_m * cmd_m,int id_bd){
        auto C_cmd=cmd->template change_request();
        auto C_cmd_manadger=cmd_m->template change_request();

        for(auto &T:(*C_cmd_manadger.get_data())){
            if(T.id_mesh==id_bd){
                std::cout<<T.id_mesh<<">>Buffer_mesh_controller:"<<size_data<<std::endl;
                if constexpr(std::is_same_v<decltype(T.cmd),DrawArraysIndirectCommand>){
                    T.cmd.count=size_data;
                    T.cmd.first=first;
                }
                if constexpr(std::is_same_v<decltype(T.cmd),DrawElementsIndirectCommand>){
                    T.cmd.count=size_data;
                    T.cmd.firstIndex=first;
                }

                (*C_cmd.get_data())[T.id]=T.cmd;
            }
        }
    }

    //сделать что-то с повторяймостью кода
public:
    template<typename VAO_, typename D_VBO,typename D_cmd, typename D_cmd_m >
    Buffer_mesh_controller(VAO_ * VAO__, D_VBO * VBO,std::nullptr_t,D_cmd * cmd,D_cmd_m * cmd_m, int id_bd, std::shared_ptr<Mesh_data> data){
        std::cout<<"Buffer_mesh_controller"<<std::endl;

        first=VBO->add_element(id_bd,std::move(data->vertex));
        size_data=data->vertex.size();

        std::cout<<"Buffer_mesh_controller:"<<first<<std::endl;
        (*VAO__)->Get_VRAM().attach(VBO);
        deleter= std::bind([](auto * VBO_,int id_bd_){
            if(VBO_!=nullptr){
                VBO_->delete_element(id_bd_);
            }},VBO,id_bd);

        this->construct(cmd,cmd_m,id_bd);
    }
    template<typename VAO_, typename D_VBO,typename D_cmd, typename D_cmd_m,typename D_mesh>
    Buffer_mesh_controller(VAO_ VAO__, D_VBO * VBO,std::nullptr_t,D_cmd * cmd,D_cmd_m * cmd_m,D_mesh d_mesh,int id_bd, std::shared_ptr<Mesh_data> data):
        Buffer_mesh_controller(VAO__,VBO,nullptr,cmd,cmd_m,id_bd,data){

        d_mesh->add_element(id_bd,{static_cast<int>(first) ,static_cast<int>(size_data)});
    }

    template<typename VAO_,typename D_VBO,typename D_EBO , typename D_cmd, typename D_cmd_m >
    Buffer_mesh_controller(VAO_ VAO__, D_VBO * VBO, D_EBO * EBO,D_cmd * cmd,D_cmd_m * cmd_m,int id_bd, std::shared_ptr<Mesh_data> data){
        std::cout<<"Buffer_mesh_controller"<<std::endl;

        first=VBO->add_element(id_bd,std::move(data->vertex));
        first=EBO->add_element(id_bd,std::move(data->indices));
        size_data=data->indices.size();

        (*VAO__)->Get_VRAM().attach(VBO,EBO);
        deleter= std::bind([](auto * VBO_,auto * EBO_,int id_bd_){
            if(VBO_!=nullptr && EBO_!=nullptr){
                VBO_->delete_element(id_bd_);
                EBO_->delete_element(id_bd_);
            }},VBO,EBO,id_bd);

        this->construct(cmd,cmd_m,id_bd);
    }

    template<typename VAO_,typename D_VBO,typename D_EBO , typename D_cmd, typename D_cmd_m,typename D_mesh>
    Buffer_mesh_controller(VAO_ VAO__, D_VBO * VBO, D_EBO * EBO,D_cmd * cmd,D_cmd_m * cmd_m,D_mesh d_mesh,int id_bd, std::shared_ptr<Mesh_data> data):
        Buffer_mesh_controller(VAO__,VBO,EBO,cmd,cmd_m,id_bd,data){

        d_mesh->add_element(id_bd,{static_cast<int>(first) ,static_cast<int>(size_data)});
    }

    ~Buffer_mesh_controller(){
        if(deleter!=nullptr){
            std::cout<<"||||||||||||||||||||||Buffer_mesh_controller|||||||||||||:"<<std::endl;
            deleter();
        }
    }
};

#endif // BUFFER_MESH_H
