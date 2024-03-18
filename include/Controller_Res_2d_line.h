#ifndef CONTROLLER_RES_2D_LINE_H
#define CONTROLLER_RES_2D_LINE_H
#include <memory>
#include <filesystem>

#include "controler_utilite.h"
#include "CeEnumeration.h"

#include "Storage_data.h"
#include "Data_gl.h"
#include "Data_res.h"
#include "Data_res_one.h"
#include "Data_gl_array_no_ram_no_allocate.h"

#include "Storage_resource.h"
#include "Unit_res.h"
#include "Section.h"
#include "Section_static.h"

#include "draw_program.h"

#include "Program_GPU.h"
#include "GLSL_code_data.h"

#include "VAO.h"
#include "Buffer_mesh_controler.h"
#include "mesh.h"


using VAO_S =Section<GL_layer::Mesh_info,VAO>;
using Buffer_mesh_controller_S=Section_static<std::shared_ptr<Mesh_data>,Buffer_mesh_controller>;

using Shades_S =Section<GL_layer::GLSL_code_data,Program_GPU>;

using Storage_res_line=Storage_resource<
    //в первую очередь
    Unit_res<TYPE_ORDER_UPDATE::FIRST,std::string,VAO_S>,
    Unit_res<TYPE_ORDER_UPDATE::FIRST,std::string,Shades_S>,
    //когда угодно, после первой...
    Unit_res<TYPE_ORDER_UPDATE::ANY  ,std::string,Buffer_mesh_controller_S>
>;


#include "Identification.h"
#include "position.h"
#include "color.h"

struct Material_L{
    int id=0;
    int map_mesh=0;
    Color color;
};

struct Command_draw_info{
    int  first=0;
    int  count=0;
};

using GL_layer::BUFFER_SETTING;
using GL_layer::TYPE_BUFFER;

#include "buffer_gl.h"

template<TIME_BIND bind_time,int bind_base,typename T,typename J=T,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_S=Data_gl  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::SHADER_STORAGE_BUFFER,CeEngine::Buffer_GL,T,J,fun,t_delete>;

template<TIME_BIND bind_time,int bind_base,typename T,typename J=T,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_I=Data_gl  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::DRAW_INDIRECT_BUFFER,CeEngine::Buffer_GL,T,J,fun,t_delete>;

template<TIME_BIND bind_time,int bind_base,typename T,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_array_no_ram_no_allocate_A=Data_gl_array_no_ram_no_allocate  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::ARRAY_BUFFER,CeEngine::Buffer_GL,T,t_delete>;

template<TIME_BIND bind_time,int bind_base,typename T,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_array_no_ram_no_allocate_E=Data_gl_array_no_ram_no_allocate  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::ELEMENT_ARRAY_BUFFER,CeEngine::Buffer_GL,T,t_delete>;


template<typename Memory>
using Res_2d_line__=
Storage_data<
    Memory,
    GL_layer::TYPE_OBJECT_DRAW::TYPE_OBJECT_LINE_STRIP,
    GL_layer::TYPE_RENDERING_ARRAY | GL_layer::TYPE_RENDERING_MYLTI | GL_layer::TYPE_RENDERING_INDERECT | GL_layer::TYPE_RENDERING_BIND_BUFFER,

    Data_res_one                         <TIME_BIND::FIRST_SHOT_LAST_OBJECT,                          Memory,std::shared_ptr<Shades_S>,-1>,
    Data_res_one                         <TIME_BIND::SHOT,                                            Memory,std::shared_ptr<VAO_S>,0>,
    Data_gl_I                            <TIME_BIND::NEVER,-2,GL_layer::DrawArraysIndirectCommand_menedger>,
    Data_gl_I                            <TIME_BIND::SHOT ,-1,GL_layer::DrawArraysIndirectCommand,GL_layer::DrawArraysIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
    Data_gl_S                            <TIME_BIND::SHOT , 0,Identification>,

    Data_gl_S                            <TIME_BIND::SHOT , 1,Position,glm::mat4,fooooo>,
    Data_gl_S                            <TIME_BIND::SHOT , 2,Material_L>,
    Data_gl_S                            <TIME_BIND::SHOT , 3,Command_draw_info,Command_draw_info,nullptr,TRIVIAL_DELETE::NO>,
    Data_gl_array_no_ram_no_allocate_A   <TIME_BIND::NEVER,-3,std::vector<float>,TRIVIAL_DELETE::NO>,
//    Data_gl_array_no_ram_no_allocate_E    <TIME_BIND::NEVER,-4,std::vector<uint>,TRIVIAL_DELETE::NO>,
    Data_res                             <TIME_BIND::NEVER,      Memory,std::shared_ptr<Buffer_mesh_controller_S>>
>;


class Controller_Res_2d_line:public Res_2d_line__<Storage_res_line>{
    std::size_t iiiii=0;
    struct Data_CeLine{
        Color color  {1.0f,1.0f,1.0f,1.0f};
        Position pos {1.0f};
        std::shared_ptr<Mesh_data> data;
        std::string type_line="";
        Data_CeLine(std::shared_ptr<Mesh_data> &&mesh,std::string type_line_=""):data(std::move(mesh)),type_line(type_line_){}
    };
public:
    using Data=Data_CeLine;

    template<typename Info_environment>
    Controller_Res_2d_line(Info_environment * info):Res_2d_line__<Storage_res_line>(info){
        GL_layer::Mesh_info info_mesh;
        info_mesh.set<GL_layer::TYPE_data::position>(1);

        //info_mesh.tex_cord_cout[0]=2;
        this->template emplace<std::shared_ptr<VAO_S>>("VAO_line",info_mesh);

        GL_layer::GLSL_code_data data_shaders;
        data_shaders.set_glsl_code<GL_layer::TYPE_SHADER::VERTEX_SHADER>  (Directory_shaders+"line_shader.vert");
        data_shaders.set_glsl_code<GL_layer::TYPE_SHADER::FRAGMENT_SHADER>(Directory_shaders+"line_shader.frag");

        this-> template emplace<std::shared_ptr<Shades_S>>("LINE_SHADER",data_shaders);///

        std::cout<<"line_inz"<<std::endl;
   }

    virtual void add_data_controller(std::size_t id,std::any &data_){
         Data data=std::any_cast<Data>(data_);
        std::cout<<"LINE"<<std::endl;

        std::string type=data.type_line;
        if(data.type_line.size()==0){
            type=std::string{"id: "+std::to_string(iiiii++)};
        }
        this-> template add_element<std::shared_ptr<Buffer_mesh_controller_S>>(id,type,
            std::tuple( this-> template get_element<std::shared_ptr<VAO_S>>(),//хммм
                        this-> template get_element_data_GL<std::vector<float>>(),
                        nullptr,
                        this-> template get_element_data_GL<GL_layer::DrawArraysIndirectCommand>(),
                        this-> template get_element_data_GL<GL_layer::DrawArraysIndirectCommand_menedger>(),
                        this-> template get_element_data_GL<Command_draw_info>())
                       ,data.data);

        GL_layer::DrawArraysIndirectCommand_menedger cmd_m;

        cmd_m.id_mesh=(*this-> template get_element<std::shared_ptr<Buffer_mesh_controller_S>>(id))->get_id();
        Material_L mat{id_global++,static_cast<int>(cmd_m.id_mesh),data.color};

        this-> template add_element<Position>(id,data.pos);
        this-> template add_element<Material_L>(id,mat);

        this->template add_element<GL_layer::DrawArraysIndirectCommand>(this->get_element_size(),cmd_m.cmd);
        this->template add_element<GL_layer::DrawArraysIndirectCommand_menedger>(id,cmd_m);
    }

    virtual std::size_t visible_data_controller(std::list<std::list<Identification *> *> * ids,bool update=false){
        return visible_data_controller_any<GL_layer::DrawArraysIndirectCommand,GL_layer::DrawArraysIndirectCommand_menedger>(this,ids,update);
    }
};
#endif // CONTROLLER_RES_2D_LINE_H
