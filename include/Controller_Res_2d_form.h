 #ifndef CONTROLLER_RES_2D_FORM_H
#define CONTROLLER_RES_2D_FORM_H

#include <memory>
#include <filesystem>

#include "controler_utilite.h"
#include "CeEnumeration.h"

#include "Storage_data.h"
#include "Data_gl.h"
#include "Data_gl_array_no_ram_no_allocate.h"
#include "Data_res.h"
#include "Data_res_one.h"

#include "Unit_res.h"
#include "Section.h"
#include "Section_static.h"
#include "Section_static_with_default_value.h"

#include "draw_program.h"

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


#include "VAO.h"
#include "Buffer_mesh_controler.h"
#include "mesh.h"

using VAO_S =Section<GL_layer::Mesh_info,VAO>;
using Buffer_mesh_controller_S=Section_static<std::shared_ptr<Mesh_data>,Buffer_mesh_controller>;

#include "buffer_mesh.h"

using Shades_S =Section<GL_layer::GLSL_code_data,Program_GPU>;


#include "Identification.h"
#include "position.h"
#include "color.h"

struct Material_RI{
    int id=0;
    int map_difusion=-1;
    Color color=0x0;
};

struct Command_draw_info_RI{
    int  firstIndex=0;
    int  count=0;
    int  baseVertex=0;
};

#include "buffer_gl.h"

template<TIME_BIND bind_time,int bind_base,typename T,typename J=T,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_S=Data_gl  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::SHADER_STORAGE_BUFFER,CeEngine::Buffer_GL,T,J,fun,t_delete>;

template<TIME_BIND bind_time,int bind_base,typename T,typename J=T,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_I=Data_gl  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::DRAW_INDIRECT_BUFFER,CeEngine::Buffer_GL,T,J,fun,t_delete>;

template<TIME_BIND bind_time,int bind_base,typename T,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_array_no_ram_no_allocate_A=Data_gl_array_no_ram_no_allocate  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::ARRAY_BUFFER,CeEngine::Buffer_GL,T,t_delete>;

template<TIME_BIND bind_time,int bind_base,typename T,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_array_no_ram_no_allocate_E=Data_gl_array_no_ram_no_allocate  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::ELEMENT_ARRAY_BUFFER,CeEngine::Buffer_GL,T,t_delete>;


using Res_2d_form__=
Storage_data<
#ifdef BINDLES_TEXTURE
    GL_layer::TYPE_OBJECT_DRAW::TYPE_OBJECT_TRIANGLES,
    GL_layer::TYPE_RENDERING_ELEMENT| GL_layer::TYPE_RENDERING_MYLTI | GL_layer::TYPE_RENDERING_INDERECT | GL_layer::TYPE_RENDERING_BIND_BUFFER,
    Data_gl_I     <TIME_BIND::SHOT, -1,GL_layer::DrawElementsIndirectCommand,GL_layer::DrawElementsIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
    Data_res_one  <TIME_BIND::FIRST_SHOT_FIRST_OBJECT,Shades_S,-1>,
#else
    GL_layer::TYPE_OBJECT_DRAW::TYPE_OBJECT_TRIANGLES,
    GL_layer::TYPE_RENDERING_ELEMENT,
    Data_gl_S     <TIME_BIND::NEVER,-1,GL_layer::DrawElementsIndirectCommand,GL_layer::DrawElementsIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
    Data_res_one  <TIME_BIND::FIRST_SHOT_FIRST_OBJECT, Shades_S,0>,
#endif
    Data_res_one  <TIME_BIND::SHOT,    VAO_S,0>,
    Data_gl_I     <TIME_BIND::NEVER,-2,GL_layer::DrawElementsIndirectCommand_menedger>,

    Data_gl_S     <TIME_BIND::SHOT,  0,Identification>,
    Data_gl_S     <TIME_BIND::SHOT,  1,Position,glm::mat4,fooooo>,
    Data_gl_S     <TIME_BIND::SHOT,  2,Material_RI>,

    Data_gl_array_no_ram_no_allocate_A   <TIME_BIND::NEVER,-3,std::vector<float>,TRIVIAL_DELETE::NO>,
    Data_gl_array_no_ram_no_allocate_E   <TIME_BIND::NEVER,-4,std::vector<uint>,TRIVIAL_DELETE::NO>,
    Data_gl_S                            <TIME_BIND::NEVER, 3,Command_draw_info_RI,Command_draw_info_RI,nullptr,TRIVIAL_DELETE::NO>,

    Data_res                             <TIME_BIND::NEVER,Unit_res,std::string,Buffer_mesh_controller_S>,
#ifdef BINDLES_TEXTURE
    Data_res    <TIME_BIND::NEVER,     Unit_res,std::string,Texture_S,0>,
    Data_gl_S   <TIME_BIND::SHOT,    4,uint64_t,uint64_t,nullptr,TRIVIAL_DELETE::NO>
#else
    Data_res    <TIME_BIND::OBJECT,    Memory,Texture_S,0>
#endif
>;


class Controller_Res_2d_form:public Res_2d_form__{
    struct Data_CePlate{
        std::filesystem::path pach="";
        std::string type_mesh="plate";
        std::shared_ptr<Mesh_data> data =nullptr;
        //Color color={0.0f,0.0f,0.0f,1.0f};
        Color color={1.0f,1.0f,1.0f,1.0f};
        //Color color;
        Position pos{1.0f};
        Data_CePlate(std::filesystem::path  pach_image=""):pach(pach_image){}
        Data_CePlate( std::shared_ptr<Mesh_data> data_,std::string type_mesh_):type_mesh(type_mesh_),data(data_){}
        Data_CePlate( std::shared_ptr<Mesh_data> data_,std::string type_mesh_,Color color_,Position pos_):
            type_mesh(type_mesh_),data(data_),color(color_),pos(pos_){}
        Data_CePlate(std::filesystem::path  pach_image_,std::string type_mesh_,Color color_,Position pos_):
            pach(pach_image_),type_mesh(type_mesh_),color(color_),pos(pos_){}
    };
public:
    using Data=Data_CePlate;

    std::unordered_map<std::string,std::shared_ptr<Mesh_data>> object_2d;

    template<typename Info_environment>
    Controller_Res_2d_form(Info_environment * info){
        GL_layer::Mesh_info info_mesh;
        info_mesh.set<GL_layer::TYPE_data::position>(3);
        info_mesh.set_texture<0>(2);

        this->template emplace<VAO_S>(info_mesh);

        object_2d["plate"]=plate_mesh();

        GL_layer::GLSL_code_data data_shaders;
        data_shaders.set_glsl_code<GL_layer::TYPE_SHADER::VERTEX_SHADER>  (Directory_shaders+"2d_object_shader.vert");
        data_shaders.set_glsl_code<GL_layer::TYPE_SHADER::FRAGMENT_SHADER>(Directory_shaders+"2d_object_shader.frag");

        this-> template  emplace<Shades_S>(data_shaders);///

        std::cout<<"plate_inz"<<std::endl;
   }

   virtual void add_data_controller(std::size_t id,std::any &data_){
        Data data=std::any_cast<Data>(data_);
        std::cout<<"PLATE"<<std::endl;

        int id_map_bd=-1;
        if(data.pach!=""){
            #ifdef BINDLES_TEXTURE
            this-> template add_element<Texture_S>(id,data.pach.string(),std::tuple(this-> template get_element_data_GL<uint64_t>()),data.pach);
            id_map_bd=(*this-> template get_element<Texture_S>(id))->get_id();  //а вот тут и  проблема, можно ли получть id в бд без бд?) Можно!
            #else
            this-> template add_element<Texture_S>(id,data.pach.string(),data.pach);
            id_map_bd=id;
            #endif

            std::cout<<id<<"<|>"<<id_map_bd<<"|"<<data.pach<<std::endl;
        }

        std::size_t id_mesh=0;
        std::shared_ptr<Mesh_data> mesh_data=data.data;

        //загрузка разных мешей
        if(this-> template isData_storage<Buffer_mesh_controller_S>(data.type_mesh)){
            this-> template add_element<Buffer_mesh_controller_S>(id,data.type_mesh);
            id_mesh=(*this-> template get_element<Buffer_mesh_controller_S>(id))->get_id();
        }else {
            //загрузка примитива из библиотеке 2d_примитивов, если он есть //пока так
            if(object_2d.find(data.type_mesh)!=object_2d.end()){
                 mesh_data=object_2d[data.type_mesh];
            }

            Command_draw_info_RI cdi;
            cdi.baseVertex =static_cast<int>(this->template get_element_data_GL<std::vector<float>>()->get_begin_of_controller(mesh_data->vertex.size()));
            cdi.firstIndex=static_cast<int>(this->template get_element_data_GL<std::vector<uint>>()->get_begin_of_controller(mesh_data->indexes.size()));
            cdi.count=static_cast<int>(mesh_data->indexes.size());


            this-> template add_element<Buffer_mesh_controller_S>(id,data.type_mesh,
                std::tuple( this-> template get_element<VAO_S>(),//хммм
                            this-> template get_element_data_GL<std::vector<float>>(),
                            cdi.baseVertex,
                            this-> template get_element_data_GL<std::vector<uint>>(),
                            cdi.firstIndex)
                ,mesh_data);

            id_mesh=(*this-> template get_element<Buffer_mesh_controller_S>(id))->get_id();

            cdi.baseVertex=cdi.baseVertex/5;//каждая вершина имеет размер в 5 float
            this-> template add_element<Command_draw_info_RI>(id_mesh,cdi);

            //std::cout<<id<<"%%%%%%%%%%%&&&&&&&&&&&&&&&&&&&&&&&&&%%"<<id_mesh<<"__"<<cdi.baseVertex<<":"<< cdi.count<<"|"<<cdi.firstIndex<<std::endl;
        }

        GL_layer::DrawElementsIndirectCommand_menedger cmd_m;

        cmd_m.cmd.count= this-> template get_element<Command_draw_info_RI>(id_mesh)->count;
        cmd_m.cmd.firstIndex= this-> template get_element<Command_draw_info_RI>(id_mesh)->firstIndex;
        cmd_m.cmd.baseVertex=this-> template get_element<Command_draw_info_RI>(id_mesh)->baseVertex;
        std::cout<<id_mesh<<"__"<<cmd_m.cmd.baseVertex<<":"<< cmd_m.cmd.count<<"|"<<cmd_m.cmd.firstIndex<<std::endl;

        this-> template add_element<Position>(id,data.pos);
        this-> template add_element<Material_RI>(id,Material_RI{id_global++,id_map_bd,data.color});

        this-> template add_element<GL_layer::DrawElementsIndirectCommand_menedger>(id,cmd_m);
        this-> template add_element<GL_layer::DrawElementsIndirectCommand>(id,cmd_m.cmd);
    }

    virtual std::size_t visible_data_controller(std::list<std::list<Identification *> *> * ids,bool update=false){
        //std::cout<<"||5.4||"<<std::endl;
        return visible_data_controller_any<GL_layer::DrawElementsIndirectCommand,GL_layer::DrawElementsIndirectCommand_menedger>(this,ids,update);
    }
};
#endif // CONTROLLER_RES_2D_FORM_H
