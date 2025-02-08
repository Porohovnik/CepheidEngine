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


#include "Identification.h"
#include "position.h"
#include "color.h"

struct Material_L{
    int id=0;
    int map_mesh=0;
    int no_x=1;
    float width=0.001f;
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

template<TIME_BIND bind_time,int bind_base,typename T,TRIVIAL_DELETE t_delete ,std::size_t size_buffer >
using Data_gl_array_no_ram_no_allocate_A_s=Data_gl_array_no_ram_no_allocate  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::ARRAY_BUFFER,CeEngine::Buffer_GL,T,t_delete,size_buffer>;

template<TIME_BIND bind_time,int bind_base,typename T,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_array_no_ram_no_allocate_E=Data_gl_array_no_ram_no_allocate  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::ELEMENT_ARRAY_BUFFER,CeEngine::Buffer_GL,T,t_delete>;



using Res_2d_line__=
Storage_data<
#ifdef GL_DRAW_LINE
    GL_layer::TYPE_OBJECT_DRAW::TYPE_OBJECT_LINE_STRIP,
    GL_layer::TYPE_RENDERING_ARRAY| GL_layer::TYPE_RENDERING_MYLTI | GL_layer::TYPE_RENDERING_INDERECT | GL_layer::TYPE_RENDERING_BIND_BUFFER,
    Data_gl_I     <TIME_BIND::SHOT, -1,GL_layer::DrawArraysIndirectCommand,GL_layer::DrawArraysIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
#else
    GL_layer::TYPE_OBJECT_DRAW::TYPE_OBJECT_LINE_STRIP,
    GL_layer::TYPE_RENDERING_ARRAY,
    Data_gl_S     <TIME_BIND::NEVER,-1,GL_layer::DrawArraysIndirectCommand,GL_layer::DrawArraysIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
#endif
    Data_res_one  <TIME_BIND::FIRST_SHOT_FIRST_OBJECT,Shades_S,-1>,
    Data_res_one                         <TIME_BIND::SHOT,VAO_S,0>,
    Data_gl_I                            <TIME_BIND::NEVER,-2,GL_layer::DrawArraysIndirectCommand_menedger>,
    Data_gl_S                            <TIME_BIND::SHOT , 0,Identification>,
    Data_gl_S                            <TIME_BIND::SHOT , 1,Position,glm::mat4,fooooo>,
    Data_gl_S                            <TIME_BIND::SHOT , 2,Material_L>,
    Data_gl_S                            <TIME_BIND::SHOT , 3,Command_draw_info,Command_draw_info,nullptr,TRIVIAL_DELETE::NO>,
    //Data_gl_array_no_ram_no_allocate_A_s <TIME_BIND::NEVER,-3,std::vector<float>,TRIVIAL_DELETE::NO,400000000>,
    Data_gl_array_no_ram_no_allocate_A_s <TIME_BIND::NEVER,-3,std::vector<float>,TRIVIAL_DELETE::NO,0>,
//    Data_gl_array_no_ram_no_allocate_E    <TIME_BIND::NEVER,-4,std::vector<uint>,TRIVIAL_DELETE::NO>,
#ifdef GL_DRAW_LINE
    Data_res                             <TIME_BIND::NEVER,Unit_res,std::string,Buffer_mesh_controller_S>
#else
    Data_res                             <TIME_BIND::OBJECT,Unit_res,std::string,Buffer_mesh_controller_S>
#endif
>;


template <bool in_x_y>
class Controller_Res_2d_line:public Res_2d_line__{
    std::size_t iiiii=0;
    struct Data_CeLine{
        Color color  {1.0f,1.0f,1.0f,1.0f};
        Position pos {1.0f};
        float width=0.001f;
        std::shared_ptr<Mesh_data> data;
        std::string type_line="";
        Data_CeLine(std::shared_ptr<Mesh_data> mesh,std::string type_line_="",float width_=0.001f):data(std::move(mesh)),type_line(type_line_),width(width_){}
        Data_CeLine(std::shared_ptr<Mesh_data> mesh,Color color_,Position pos_,std::string type_line_="", float width_=0.001f):data(std::move(mesh)),color(color_),pos(pos_),type_line(type_line_),width(width_){}

    };
public:
    using Data=Data_CeLine;

    template<typename Info_environment>
    Controller_Res_2d_line(Info_environment * info){
        GL_layer::Mesh_info info_mesh;
        if constexpr(in_x_y){
            info_mesh.set<GL_layer::TYPE_data::position>(1);
        }
        info_mesh.set<GL_layer::TYPE_data::normal>(1);

        this->template emplace<VAO_S>(info_mesh);

        GL_layer::GLSL_code_data data_shaders;
        data_shaders.set_glsl_code<GL_layer::TYPE_SHADER::VERTEX_SHADER>  (Directory_shaders+"line_shader.vert");
        //data_shaders.set_glsl_code<GL_layer::TYPE_SHADER::GEOMETRY_SHADER>(Directory_shaders+"line_shader_v2.geom");
        data_shaders.set_glsl_code<GL_layer::TYPE_SHADER::FRAGMENT_SHADER>(Directory_shaders+"line_shader.frag");

        this-> template emplace<Shades_S>(data_shaders);///

        std::cout<<"line_inz"<<std::endl;
   }

    virtual void add_data_controller(std::size_t id,std::any &data_){
        Data data=std::any_cast<Data>(data_);
        std::cout<<"LINE"<<std::endl;

        std::string type=data.type_line;
        if(data.type_line.size()==0){
            type=std::string{"id: "+std::to_string(iiiii++)};
        }


        std::size_t id_mesh=0;

        if(this-> template isData_storage<Buffer_mesh_controller_S>(type)){
            this-> template add_element<Buffer_mesh_controller_S>(id,type);
            id_mesh=(*this-> template get_element<Buffer_mesh_controller_S>(id))->get_id();
        }else {
            Command_draw_info cdi;
            cdi.first=static_cast<int>(this->template get_element_data_GL<std::vector<float>>()->get_begin_of_controller(data.data->vertex.size()));
            cdi.count=static_cast<int>(data.data->vertex.size());

            this-> template add_element<Buffer_mesh_controller_S>(id,type,
                std::tuple( this-> template get_element<VAO_S>(),//хммм
                            this-> template get_element_data_GL<std::vector<float>>(),
                            cdi.first,
                            nullptr,
                            0)
                ,data.data);

            cdi.first=cdi.first/(1+in_x_y);
            cdi.count=cdi.count/(1+in_x_y);

            id_mesh=(*this-> template get_element<Buffer_mesh_controller_S>(id))->get_id();
            this-> template add_element<Command_draw_info>(id_mesh,cdi);
        }


        GL_layer::DrawArraysIndirectCommand_menedger cmd_m;

        cmd_m.cmd.count= this-> template get_element<Command_draw_info>(id_mesh)->count;
        cmd_m.cmd.first= this-> template get_element<Command_draw_info>(id_mesh)->first;

        Material_L mat{id_global++,static_cast<int>(id_mesh),!in_x_y,data.width,data.color};

        this-> template add_element<Position>(id,data.pos);
        this-> template add_element<Material_L>(id,mat);

        this->template add_element<GL_layer::DrawArraysIndirectCommand>(id,cmd_m.cmd);
        this->template add_element<GL_layer::DrawArraysIndirectCommand_menedger>(id,cmd_m);
    }

    virtual std::size_t visible_data_controller(std::list<std::list<Identification *> *> * ids,bool update=false){
        return visible_data_controller_any<GL_layer::DrawArraysIndirectCommand,GL_layer::DrawArraysIndirectCommand_menedger>(this,ids,update);
    }
};
#endif // CONTROLLER_RES_2D_LINE_H
