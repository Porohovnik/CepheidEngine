#ifndef CONTROLLER_RES_2D_FORM_H
#define CONTROLLER_RES_2D_FORM_H

#include <memory>
#include <filesystem>

#include "controler_utilite.h"
#include "CeEnumeration.h"

#include "Storage_data.h"
#include "Data_gl.h"
#include "Data_res.h"
#include "Data_res_one.h"

#include "Storage_resource.h"
#include "Unit_res.h"
#include "Section.h"
#include "Section_static.h"

#include "draw_program.h"

#include "buffer_mesh.h"
#include "mesh.h"

#include "Program_GPU.h"
#include "GLSL_code_data.h"

#ifdef BINDLES_TEXTURE
#include "Texture_controller_BINDLESS.h"
#include "image.h"
using Texture_S=Section_static<Image,Texture_controller_BINDLESS>;
#else
#include "texture.h"
#include "image.h"
using Texture_S=Section<Image,Texture>;
#endif

using Buffer_mesh_S =Section<Mesh,Buffer_mesh>;
using Shades_S =Section<GL_layer::GLSL_code_data,Program_GPU>;

using Storage_res_2d_form=Storage_resource<
    //в первую очередь
    Unit_res<TYPE_ORDER_UPDATE::FIRST,std::string,Shades_S>,
    //когда угодно, после первой...
    Unit_res<TYPE_ORDER_UPDATE::ANY  ,std::string,Texture_S>,
    Unit_res<TYPE_ORDER_UPDATE::ANY  ,std::string,Buffer_mesh_S>
>;

#include "Identification.h"
#include "position.h"
#include "color.h"

struct Material_RI{
    int id=0;
    int map_difusion=-1;
    Color color=0x0;
};

#include "buffer_gl.h"

template<TIME_BIND bind_time,int bind_base,typename T,typename J=T,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_S=Data_gl  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::SHADER_STORAGE_BUFFER,CeEngine::Buffer_GL,T,J,fun,t_delete>;

template<TIME_BIND bind_time,int bind_base,typename T,typename J=T,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_I=Data_gl  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::DRAW_INDIRECT_BUFFER,CeEngine::Buffer_GL,T,J,fun,t_delete>;

template<typename Memory>
using Res_2d_form__=
Storage_data<
    Memory,
#ifdef BINDLES_TEXTURE
    GL_layer::TYPE_OBJECT_DRAW::TYPE_OBJECT_TRIANGLES,
    GL_layer::TYPE_RENDERING_ELEMENT| GL_layer::TYPE_RENDERING_MYLTI | GL_layer::TYPE_RENDERING_INDERECT | GL_layer::TYPE_RENDERING_BIND_BUFFER,
    Data_gl_I     <TIME_BIND::SHOT, -1,GL_layer::DrawElementsIndirectCommand,GL_layer::DrawElementsIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
    Data_res_one  <TIME_BIND::FIRST_SHOT_FIRST_OBJECT, Memory,std::shared_ptr<Shades_S>,-1>,
#else
    GL_layer::TYPE_OBJECT_DRAW::TYPE_OBJECT_TRIANGLES,
    GL_layer::TYPE_RENDERING_ELEMENT,
    Data_gl_S     <TIME_BIND::NEVER,-1,GL_layer::DrawElementsIndirectCommand,GL_layer::DrawElementsIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
    Data_res_one  <TIME_BIND::FIRST_SHOT_FIRST_OBJECT, Memory,std::shared_ptr<Shades_S>,0>,
#endif

    Data_gl_I     <TIME_BIND::NEVER,-2,GL_layer::DrawElementsIndirectCommand_menedger>,

    Data_gl_S     <TIME_BIND::SHOT,  0,Identification>,
    Data_gl_S     <TIME_BIND::SHOT,  1,Position,glm::mat4,fooooo>,
    Data_gl_S     <TIME_BIND::SHOT,  2,Material_RI>,
    Data_res_one  <TIME_BIND::SHOT,    Memory,std::shared_ptr<Buffer_mesh_S>>,

#ifdef BINDLES_TEXTURE
    Data_res    <TIME_BIND::NEVER,     Memory,std::shared_ptr<Texture_S>,0>,
    Data_gl_S   <TIME_BIND::SHOT,    4,uint64_t,uint64_t,nullptr,TRIVIAL_DELETE::NO>
#else
    Data_res    <TIME_BIND::OBJECT,    Memory,std::shared_ptr<Texture_S>,0>
#endif
>;


class Controller_Res_2d_form:public Res_2d_form__<Storage_res_2d_form>{
    struct Data_CePlate{
        std::filesystem::path pach="";
        int type=0;
        //Color color={0.0f,0.0f,0.0f,1.0f};
        Color color={1.0f,1.0f,1.0f,1.0f};
        //Color color;
        Position pos{1.0f};
        Data_CePlate(std::filesystem::path  pach_image=""):pach(pach_image){}
    };
public:
    using Data=Data_CePlate;

    template<typename Info_environment>
    Controller_Res_2d_form(Info_environment * info):Res_2d_form__<Storage_res_2d_form>(info){
        this-> template  emplace<std::shared_ptr<Buffer_mesh_S>>("plate",plate_mesh());///
        GL_layer::GLSL_code_data data_shaders;
        data_shaders.set_glsl_code<GL_layer::TYPE_SHADER::VERTEX_SHADER>  (Directory_shaders+"2d_object_shader.vert");
        data_shaders.set_glsl_code<GL_layer::TYPE_SHADER::FRAGMENT_SHADER>(Directory_shaders+"2d_object_shader.frag");

        this-> template  emplace<std::shared_ptr<Shades_S>>("2D_OBJECT_SHADER",data_shaders);///

        std::cout<<"plate_inz"<<std::endl;
   }

   virtual void add_data_controller(std::size_t id,std::any &data_){
        Data data=std::any_cast<Data>(data_);
        std::cout<<"PLATE"<<std::endl;

        int id_map_bd=-1;
        if(data.pach!=""){
            #ifdef BINDLES_TEXTURE
            this-> template add_element<std::shared_ptr<Texture_S>>(id,data.pach.string(),std::tuple(this-> template get_element_data_GL<uint64_t>()),data.pach);
            id_map_bd=(*this-> template get_element<std::shared_ptr<Texture_S>>(id))->get_id();  //а вот тут и  проблема, можно ли получть id в бд без бд?) Можно!
            #else
            this-> template add_element<std::shared_ptr<Texture_S>>(id,data.pach.string(),data.pach);
            id_map_bd=id;
            #endif

            std::cout<<id<<"<|>"<<id_map_bd<<"|"<<data.pach<<std::endl;
        }

        this-> template add_element<Position>(id,data.pos);
        this-> template add_element<Material_RI>(id,Material_RI{id_global++,id_map_bd,data.color});

        this-> template add_element<GL_layer::DrawElementsIndirectCommand_menedger>(id,GL_layer::DrawElementsIndirectCommand_menedger{});
    }

    virtual std::size_t visible_data_controller(std::list<std::list<Identification *> *> * ids,bool update=false){
        //std::cout<<"||5.4||"<<std::endl;
        return visible_data_controller_any<GL_layer::DrawElementsIndirectCommand,GL_layer::DrawElementsIndirectCommand_menedger>(this,ids,update);
    }
};
#endif // CONTROLLER_RES_2D_FORM_H
