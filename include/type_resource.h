#ifndef TYPE_RESOURCE_H
#define TYPE_RESOURCE_H
#include "information_rendering.h"
//в отдельный файл:

#include "memory_managment.h"
#include "position.h"
#include "color.h"
#include "buffer_gl.h"
#include "buffer_mesh.h"
#include "texture.h"
#include "font.h"
#include "ceobject.h"
#include "draw_program.h"
#include "shader.h"
#include "them_center.h"
#include "any"
#include "incoming_signals_from_user.h"

#include<iostream>
#include <filesystem>

#define BINDLES_TEXTURE

inline static int id_global=1;

//const std::string Directory_2="/home/arhiv/Yandex.Disk/Cepheid_engine/";
const std::string Directory_2="";

const std::string Directory_pictures=Directory_2+"pictures/";
const std::string Directory_fonts=Directory_2+"fonts/";
const std::string Directory_shaders=Directory_2+"shaders/";
const std::string Directory_3D_object=Directory_2+"3D_objects/";

class Uniform_fun{
    GLint id_pos;
    GLint id_texture;
public:
    Uniform_fun(GLuint Program){
        id_pos=Uniform_Location(Program,"i");
        id_texture=Uniform_Location(Program,"ourTexture");
        //std::cout<<glGetError()<<"/-sheder-/"<<"///id_texture::"<<id_texture<<":"<<id_pos<<std::endl;
    }

    inline void operator()(int base) const{
        if(base!=-1){
            GL_layer::Uniform(id_texture,base);
            std::cout<<glGetError()<<"/-sheder-/"<<"///::"<<base<<":"<<std::endl;
        }
    }
    inline void operator()(int base,int id) const{
        GL_layer::Uniform(id_pos,id);
        //
    }
};


#ifdef BINDLES_TEXTURE
using Texture_S=Section_static<Image,Texture_controller_BINDLESS<GL_TEXTURE_2D>>;
using Font_S =  Section_static<Glifs,Font_controller_BINDLESS>;
#else
using Texture_S=Section<Image,Texture<GL_TEXTURE_2D>>;
using Font_S =Section_static<Glifs,Font_controller<Texture<GL_TEXTURE_2D>>>;
#endif

using Buffer_mesh_S =Section<Mesh,Buffer_mesh>;
template<std::size_t n>
using Shades_S =Section<std::array<std::string,n>,Shader<Uniform_fun>>;

using VAO_S =Section<Mesh_info,VAO<GL_VERTEX_ARRAY>>;
using Buffer_mesh_controller_S=Section_static<std::shared_ptr<Mesh_data>,Buffer_mesh_controller>;


using Storage_res=Storage_resource<
    //в первую очередь
    Unit_res<TYPE_ORDER_UPDATE::FIRST,std::string,VAO_S>,
    Unit_res<TYPE_ORDER_UPDATE::FIRST,std::string,Shades_S<2>>,
    //когда угодно, после первой...
    Unit_res<TYPE_ORDER_UPDATE::ANY  ,std::string,Texture_S>,
    Unit_res<TYPE_ORDER_UPDATE::ANY  ,std::string,Font_S>,
    Unit_res<TYPE_ORDER_UPDATE::ANY  ,std::string,Buffer_mesh_S>,
    Unit_res<TYPE_ORDER_UPDATE::ANY  ,std::string,Buffer_mesh_controller_S>
>;

struct Command_draw_info{
    int  first=0;
    int  count=0;
};

struct Material_RI{
    int id=0;
    int map_difusion=-1;
    Color color=0x0;
};

struct Samplers{
    GLuint64 texture;
};
struct Material_T{//аккуратно!!
    int id=0;
    int map_font=0;
    Color color;
};

struct Material_L{
    int id=0;
    int map_mesh=0;
    Color color;
};


auto fooooo(Position &pos){
    auto p=pos.pos();
    return p;
}

class DrawElementsIndirectCommand_menedger{
public:
    DrawElementsIndirectCommand cmd;
    std::size_t id=0;
    std::size_t id_mesh=0;
};

class DrawArraysIndirectCommand_menedger{
public:
    DrawArraysIndirectCommand cmd;
    std::size_t id=0;
    std::size_t id_mesh=0;
};

template<typename CMD,typename CMD_m,typename T>
inline bool visible_data_controller_any(T * t,std::list<std::list<Identification *> *> * ids,bool update=false){
    //std::cout<<"||5.5||"<<std::endl;
    if(update){
        auto ch = t->template change_request<Identification>();
        auto cmd=t->template change_request<CMD>();
        auto cmd_manadger=t->template change_request<CMD_m>();

        std::size_t size=0;
        for(auto &I:*ids){
            size+=I->size();
        }

        t->set_visible_size(size);//хммм

        ch.get_data()->resize(t->get_element_size());
        cmd.get_data()->resize(t->get_element_size());

        auto l_b=t->get_delete_ids().begin();

        std::size_t k=0;
        for(auto &I:*ids){
            for(auto &ID:*I){
                int id=ID->get_id();
                (*ch.get_data())[k]=id;
                (*cmd.get_data())[k]=(*cmd_manadger.get_data())[id].cmd;
                (*cmd_manadger.get_data())[id].id=k;

                k++;
            }
        }
        std::cout<<k<<"|"<<t->get_element_size()<<"||||||||||||||||^^^^^^^^"<<t->get_delete_ids().size()<<std::endl;
        return true;
    }
    return false;
}

template<typename Memory,template<typename K, uint  L> class Buffer>
using Res_2d_form__=
Storage_data<
    Memory,
#ifdef BINDLES_TEXTURE
    TYPE_OBJECT_TRIANGLES,
    TYPE_RENDERING_ELEMENT| TYPE_RENDERING_MYLTI | TYPE_RENDERING_INDERECT | TYPE_RENDERING_BIND_BUFFER,
    Data_gl      <TIME_BIND::SHOT, -1,GL_STATIC_DRAW,GL_DRAW_INDIRECT_BUFFER, Buffer,DrawElementsIndirectCommand,DrawElementsIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
    Data_res_one<TIME_BIND::FIRST_SHOT_FIRST_OBJECT,                          Memory,std::shared_ptr<Shades_S<2>>,-1>,
#else
    TYPE_OBJECT_TRIANGLES,
    TYPE_RENDERING_ELEMENT,
    Data_gl      <TIME_BIND::NEVER,-1,GL_STATIC_DRAW,GL_DRAW_INDIRECT_BUFFER, Buffer,DrawElementsIndirectCommand,DrawElementsIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
    Data_res_one<TIME_BIND::FIRST_SHOT_FIRST_OBJECT,                          Memory,std::shared_ptr<Shades_S<2>>,0>,
#endif

    Data_gl     <TIME_BIND::NEVER, -2,GL_STATIC_DRAW,GL_DRAW_INDIRECT_BUFFER, Buffer,DrawElementsIndirectCommand_menedger>,

    Data_gl     <TIME_BIND::SHOT,   0,GL_STATIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,Identification>,
    Data_gl     <TIME_BIND::SHOT,   1,GL_STATIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,Position,glm::mat4,fooooo>,
    Data_gl     <TIME_BIND::SHOT,   2,GL_STATIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,Material_RI>,
    Data_res_one<TIME_BIND::SHOT,                                             Memory,std::shared_ptr<Buffer_mesh_S>>,

#ifdef BINDLES_TEXTURE
    Data_res    <TIME_BIND::NEVER,                                            Memory,std::shared_ptr<Texture_S>,0>,
    Data_gl     <TIME_BIND::SHOT,  4,GL_STATIC_DRAW,GL_SHADER_STORAGE_BUFFER, Buffer,uint64_t,uint64_t,nullptr,TRIVIAL_DELETE::NO>
#else
    Data_res    <TIME_BIND::OBJECT,                                           Memory,std::shared_ptr<Texture_S>,0>
#endif
>;

struct Data_CePlate{
    std::filesystem::path pach="";
    int type=0;
    //Color color={0.0f,0.0f,0.0f,1.0f};
    Color color={1.0f,1.0f,1.0f,1.0f};
    //Color color;
    Position pos{1.0f};
    Data_CePlate(std::filesystem::path  pach_image=""):pach(pach_image){}

};


template<typename Memory,template<typename K, uint  L> class Buffer,typename Data=Data_CePlate>
class Controller_Res_2d_form:public Res_2d_form__<Memory,Buffer>{
public:
    template<typename Info_environment>
    Controller_Res_2d_form(Data_herald<Memory,Info_environment> herald):Res_2d_form__<Memory,Buffer>(herald){
        this-> template  emplace<std::shared_ptr<Buffer_mesh_S>>("plate",plate_mesh);///
        this-> template  emplace<std::shared_ptr<Shades_S<2>>>("PLATE_SHEDER",nullptr,load_files(std::array<std::filesystem::path,2>
        {Directory_shaders+"Plate_vec_sheder.vert",
         Directory_shaders+"Plate_frag_sheder.frag"}));///

        std::cout<<"plate_inz"<<std::endl;
   }

   virtual void add_data_controller(std::size_t id,std::any &data){
        std::cout<<"PLATE"<<std::endl;
        auto &[pach,type,color,pos]=*(std::any_cast<Data>(&data));

        int id_map_bd=-1;
        if(pach!=""){            
            #ifdef BINDLES_TEXTURE
            this-> template add_element<std::shared_ptr<Texture_S>>(id,pach.string(),std::tuple(this-> template get_element_data_GL<uint64_t>()),load_image,pach,Map_diffusion);
            id_map_bd=(*this-> template get_element<std::shared_ptr<Texture_S>>(id))->get_id();  //а вот тут и  проблема, можно ли получть id в бд без бд?) Можно!
            #else
            this-> template add_element<std::shared_ptr<Texture_S>>(id,pach.string(),load_image,pach,Map_diffusion);
            id_map_bd=id;
            #endif

            std::cout<<id<<"<|>"<<id_map_bd<<"|"<<pach<<std::endl;
        }

        this-> template add_element<Position>(id,pos);
        this-> template add_element<Material_RI>(id,Material_RI{id_global++,id_map_bd,color});

        this-> template add_element<DrawElementsIndirectCommand_menedger>(id,DrawElementsIndirectCommand_menedger{});
    }

    virtual std::size_t visible_data_controller(std::list<std::list<Identification *> *> * ids,bool update=false){
        //std::cout<<"||5.4||"<<std::endl;
        return visible_data_controller_any<DrawElementsIndirectCommand,DrawElementsIndirectCommand_menedger>(this,ids,update);
    }
};

template<typename Memory,template<typename K, uint  L> class Buffer>
using Controller_Res_2d_form__=Controller_Res_2d_form<Memory,Buffer>;

template<typename Memory,template<typename K, uint  L> class Buffer>
using Res_2d_text__=
Storage_data<
    Memory,
#ifdef BINDLES_TEXTURE
    TYPE_OBJECT_TRIANGLES,
    TYPE_RENDERING_ELEMENT | TYPE_RENDERING_MYLTI | TYPE_RENDERING_INDERECT | TYPE_RENDERING_BIND_BUFFER,
    Data_gl      <TIME_BIND::SHOT,  -1,GL_STATIC_DRAW,GL_DRAW_INDIRECT_BUFFER,Buffer,DrawElementsIndirectCommand,DrawElementsIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
    Data_res_one <TIME_BIND::FIRST_SHOT_LAST_OBJECT,                          Memory,std::shared_ptr<Shades_S<2>>,-1>,
#else
    TYPE_OBJECT_TRIANGLES,
    TYPE_RENDERING_ELEMENT | TYPE_RENDERING_INSTANSE,
    Data_gl      <TIME_BIND::NEVER,-1,GL_STATIC_DRAW,GL_DRAW_INDIRECT_BUFFER,Buffer,DrawElementsIndirectCommand,DrawElementsIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
    Data_res_one <TIME_BIND::FIRST_SHOT_LAST_OBJECT,                          Memory,std::shared_ptr<Shades_S<2>>,0>,
#endif
    Data_gl      <TIME_BIND::NEVER,-2,GL_STATIC_DRAW,GL_DRAW_INDIRECT_BUFFER, Buffer,DrawElementsIndirectCommand_menedger>,
    Data_gl      <TIME_BIND::SHOT,  0,GL_STATIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,Identification>,
    Data_gl      <TIME_BIND::SHOT,  1,GL_STATIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,Position,glm::mat4,fooooo>,
    Data_gl      <TIME_BIND::SHOT,  2,GL_STATIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,Material_T>,
                //некрасиво числа aloc и данных
    Data_gl_array<TIME_BIND::SHOT,  3,     4,GL_DYNAMIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,std::vector<int>>,//аккуратно!!
    Data_res_one <TIME_BIND::SHOT,                                            Memory,std::shared_ptr<Buffer_mesh_S>>,
                //некрасиво числа aloc и данных
    Data_gl_array<TIME_BIND::SHOT,  6,     7,GL_STATIC_DRAW ,GL_SHADER_STORAGE_BUFFER,Buffer,std::vector<Glif_info>,Glif_info,nullptr,TRIVIAL_DELETE::NO>,
#ifdef BINDLES_TEXTURE
    Data_gl      <TIME_BIND::SHOT,  5,GL_STATIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,uint64_t,uint64_t,nullptr,TRIVIAL_DELETE::NO>,
    Data_res     <TIME_BIND::NEVER,                                           Memory,std::shared_ptr<Font_S>,0>
#else
    Data_res    <TIME_BIND::OBJECT,                                        Memory,std::shared_ptr<Font_S>,0>
#endif
> ;

struct Data_CeText{
    std::filesystem::path path_font="";
    std::string text="";
    int size_max_text=0;
    int mod=-1;
    Color color={1.0f,1.0f,1.0f,1.0f};

    Position pos{1.0f};
    Data_CeText(std::filesystem::path path_font_,std::string text_=""):path_font(path_font_),text(text_){}
};

enum class TYPE_TEXT{ANSI=0,UNICOD=1};

template<typename Memory,template<typename K, uint  L> class Buffer,typename Data=Data_CeText>
class Controller_Res_2d_text: public Res_2d_text__<Memory,Buffer>{
public:
    template<typename Info_environment>
    Controller_Res_2d_text(Data_herald<Memory,Info_environment> herald):Res_2d_text__<Memory,Buffer>(herald){
        this->template emplace<std::shared_ptr<Buffer_mesh_S>>("plate",plate_mesh);///
        this->template emplace<std::shared_ptr<Shades_S<2>>>("TEXT_SHEDER",load_files<2>,std::array<std::filesystem::path,2>
        {Directory_shaders+"Text_vec_sheder.vert",
         Directory_shaders+"Text_frag_sheder.frag"});///

        std::cout<<"text_inz"<<std::endl;
    }

    virtual void add_data_controller(std::size_t id,std::any &data){
        std::cout<<"TEXT"<<std::endl;
        auto &[path_font,text,size_max_text,mod,color,pos]=*(std::any_cast<Data>(&data));
        //тут формулка что из позиции вычесляет размер текста
        int size=40;

        std::pair<float,int> mapping[6]={
            {0.0f,20},{0.01,40},{0.05,50},{0.1,60},{0.4,80},{0.6,100}
        };
        for(auto &M:mapping){
            if(pos.pos()[1][1]/2.0f>=M.first){
                size=M.second;
            }
        }

        //std::cout<<pos.pos()[1][1]/2.0f<<"::"<<size<<std::endl;
        std::string hesh=path_font.string()+ std::to_string(size);//???

        std::vector<int> sumbols;
        string_to_unicod(text.c_str(),&sumbols);
        converting_unicode_to_glyph_number(sumbols);
        this->template add_element<std::vector<int>>(id,sumbols,size_max_text);


        #ifdef BINDLES_TEXTURE
        this->template add_element<std::shared_ptr<Font_S>>(
            id,hesh,
            std::tuple(this-> template get_element_data_GL<std::vector<Glif_info>>(),this-> template get_element_data_GL<uint64_t>())
            ,load_glif,path_font,size);
        #else
        this->template add_element<std::shared_ptr<Font_S>>(
            id,hesh,
            std::tuple(this-> template get_element_data_GL<std::vector<Glif_info>>())
            ,load_glif,path_font,size);
        #endif

        int id_map_bd=(*this-> template get_element<std::shared_ptr<Font_S>>(id))->get_id();//а вот тут и  проблема, можно ли получть id в бд без бд?) Можно!

        this->template add_element<Position>(id,pos);
        this->template add_element<Material_T>(id,Material_T{id_global++,id_map_bd,color});
        ///для биндле
        DrawElementsIndirectCommand_menedger cmd_m;
        cmd_m.cmd.instanceCount=sumbols.size();
        cmd_m.id=id;

        this->template add_element<DrawElementsIndirectCommand>(this->get_element_size(),cmd_m.cmd);
        this->template add_element<DrawElementsIndirectCommand_menedger>(id,cmd_m);
    }

    virtual std::size_t visible_data_controller(std::list<std::list<Identification *> *> * ids,bool update=false){
        return visible_data_controller_any<DrawElementsIndirectCommand,DrawElementsIndirectCommand_menedger>(this,ids,update);
    }

    template<TYPE_TEXT type_>
    bool change_text(std::size_t id,const char * ch,std::size_t start=0,std::size_t size_array=0){
        auto change=this->template change_request<std::vector<int>>(id);

        if constexpr(type_==TYPE_TEXT::UNICOD){
            string_to_unicod(ch,change.get_data());
        }
        if constexpr(type_==TYPE_TEXT::ANSI){

            for(std::size_t i=change.get_data()->size();i<start+size_array;i++){
                change.get_data()->push_back({});
            }
            for(std::size_t i=0;i<size_array;i++){
                (*change.get_data())[i+start]=ch[i];
            }
        }

        converting_unicode_to_glyph_number(*change.get_data());

        if(this->template get_element<DrawElementsIndirectCommand_menedger>(id)->cmd.instanceCount!=(*change.get_data()).size()){
            auto cmd_m=this->template change_request<DrawElementsIndirectCommand_menedger>(id);
            cmd_m.get_data()->cmd.instanceCount=(*change.get_data()).size();

            *(this->template change_request<DrawElementsIndirectCommand>(cmd_m.get_data()->id).get_data())=cmd_m.get_data()->cmd;
        }

        return true;
    }
};

template<typename Memory,template<typename K, uint  L> class Buffer>
using Controller_Res_2d_text__=Controller_Res_2d_text<Memory,Buffer>;



template<typename Memory,template<typename K, uint  L> class Buffer>
using Res_2d_line__=
Storage_data<
    Memory,
    TYPE_OBJECT_LINE_STRIP,
    TYPE_RENDERING_ARRAY | TYPE_RENDERING_MYLTI | TYPE_RENDERING_INDERECT | TYPE_RENDERING_BIND_BUFFER,

    Data_res_one                        <TIME_BIND::FIRST_SHOT_LAST_OBJECT,                          Memory,std::shared_ptr<Shades_S<2>>,-1>,
    Data_res_one                        <TIME_BIND::SHOT,                                            Memory,std::shared_ptr<VAO_S>,0>,
    Data_gl                             <TIME_BIND::NEVER,-2,GL_STATIC_DRAW,GL_DRAW_INDIRECT_BUFFER, Buffer,DrawArraysIndirectCommand_menedger>,
    Data_gl                             <TIME_BIND::SHOT ,-1,GL_STATIC_DRAW,GL_DRAW_INDIRECT_BUFFER, Buffer,DrawArraysIndirectCommand,DrawArraysIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
    Data_gl                             <TIME_BIND::SHOT , 0,GL_STATIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,Identification>,

    Data_gl                             <TIME_BIND::SHOT , 1,GL_STATIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,Position,glm::mat4,fooooo>,
    Data_gl                             <TIME_BIND::SHOT , 2,GL_STATIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,Material_L>,
    Data_gl                             <TIME_BIND::SHOT , 3,GL_STATIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,Command_draw_info,Command_draw_info,nullptr,TRIVIAL_DELETE::NO>,
    Data_gl_array_no_ram_no_allocate    <TIME_BIND::NEVER,-3,GL_STATIC_DRAW,GL_ARRAY_BUFFER,         Buffer,std::vector<float>,TRIVIAL_DELETE::NO>,
//    Data_gl_array_no_ram_no_allocate    <TIME_BIND::NEVER,-4,GL_STATIC_DRAW,GL_ELEMENT_ARRAY_BUFFER, Buffer,std::vector<uint>,TRIVIAL_DELETE::NO>,
    Data_res                            <TIME_BIND::NEVER,                                           Memory,std::shared_ptr<Buffer_mesh_controller_S>>
>;

struct Data_CeLine{
    Color color  {1.0f,1.0f,1.0f,1.0f};
    Position pos {1.0f};
    std::shared_ptr<Mesh_data> data;
    std::string type_line="";
    Data_CeLine(std::shared_ptr<Mesh_data> &&mesh,std::string type_line_=""):data(std::move(mesh)),type_line(type_line_){}
};


template<typename Memory,template<typename K, uint  L> class Buffer,typename Data=Data_CeLine>
class Controller_Res_2d_line:public Res_2d_line__<Memory,Buffer>{
    std::size_t iiiii=0;
public:
    template<typename Info_environment>
    Controller_Res_2d_line(Data_herald<Memory,Info_environment> herald):Res_2d_line__<Memory,Buffer>(herald){
        Mesh_info info_mesh;
        info_mesh.elements_cout[static_cast<int>(TYPE_data::position)]=1;
        //info_mesh.tex_cord_cout[0]=2;
        this->template emplace<std::shared_ptr<VAO_S>>("VAO_line",nullptr,info_mesh);

        this-> template emplace<std::shared_ptr<Shades_S<2>>>("LINE_SHEDER",load_files<2>,std::array<std::filesystem::path,2>
        {Directory_shaders+"Line_vec_sheder.vert",
         Directory_shaders+"Line_frag_sheder.frag"});///

        std::cout<<"line_inz"<<std::endl;
   }

   virtual void add_data_controller(std::size_t id,std::any &data){
        auto &[color,pos,data_mesh,type_line]=*(std::any_cast<Data>(&data));


        std::string type=type_line;
        if(type_line.size()==0){
            type=std::string{"id: "+std::to_string(iiiii++)};
        }
        this-> template add_element<std::shared_ptr<Buffer_mesh_controller_S>>(id,type,
            std::tuple( this-> template get_element<std::shared_ptr<VAO_S>>(),//хммм
                        this-> template get_element_data_GL<std::vector<float>>(),
                        nullptr,
                        this-> template get_element_data_GL<DrawArraysIndirectCommand>(),
                        this-> template get_element_data_GL<DrawArraysIndirectCommand_menedger>(),
                        this-> template get_element_data_GL<Command_draw_info>())
                       ,nullptr,data_mesh);

        DrawArraysIndirectCommand_menedger cmd_m;

        cmd_m.id_mesh=(*this-> template get_element<std::shared_ptr<Buffer_mesh_controller_S>>(id))->get_id();
        Material_L mat{id_global++,static_cast<int>(cmd_m.id_mesh),color};

        this-> template add_element<Position>(id,pos);
        this-> template add_element<Material_L>(id,mat);

        this->template add_element<DrawArraysIndirectCommand>(this->get_element_size(),cmd_m.cmd);
        this->template add_element<DrawArraysIndirectCommand_menedger>(id,cmd_m);
    }

    virtual std::size_t visible_data_controller(std::list<std::list<Identification *> *> * ids,bool update=false){
        return visible_data_controller_any<DrawArraysIndirectCommand,DrawArraysIndirectCommand_menedger>(this,ids,update);
    }
};

template<typename Memory,template<typename K, uint  L> class Buffer>
using Controller_Res_2d_line__=Controller_Res_2d_line<Memory,Buffer>;



struct Win_data{
    int widht=0;
};

template<typename Memory,template<typename K, uint  L> class Buffer>
using Win_storage__=
Storage_data<
    Memory,
    TYPE_OBJECT_TRIANGLES,
    TYPE_RENDERING_ELEMENT,
//    Data_res_one<TIME_BIND::FIRST_SHOT_FIRST_OBJECT,                               Memory,std::shared_ptr<Shades_S<2>>,0>,
//    Data_res_one <TIME_BIND::SHOT,                                          Memory,std::shared_ptr<Buffer_mesh_S>>,
    Data_gl_read_no_ram <TIME_BIND::SHOT,10,GL_DYNAMIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,GLuint,TRIVIAL_DELETE::NO>,
    Data_gl_one         <TIME_BIND::SHOT,11,GL_DYNAMIC_DRAW,GL_SHADER_STORAGE_BUFFER,Buffer,Win_data>
>;
template<typename Memory,template<typename K, uint  L> class Buffer>
class Controller_Win_storage:public Win_storage__<Memory,Buffer>{
    Incoming_signals_from_user * ISFU;//пока констекст жив, это тоже
    glm::vec4 background_color;
public:
    Controller_Win_storage(glm::vec4 background_color_,Data_herald<Memory,Incoming_signals_from_user *> herald):
        Win_storage__<Memory,Buffer>(herald),background_color(background_color_),ISFU(herald.info){
        std::cout<<"Win_inz"<<std::endl;

//        this-> template  emplace<std::shared_ptr<Buffer_mesh_S>>(plate_mesh().hash(),plate_mesh);///
//        this-> template  emplace<std::shared_ptr<Shades_S<2>>>("WIN_SHEDER",load_files<2>,std::array<std::filesystem::path,2>
//        {Directory_shaders+"Win_vec_sheder.vert",
//         Directory_shaders+"Win_frag_sheder.frag"});///
//        this->new_status(Type_Status::VISEBLE);
    }

    virtual bool new_frame_controller(){
        if(ISFU->isChangeSize){
            glViewport(0, 0, ISFU->width,ISFU->height);
            this->template get_element<GLuint>()->expansion_not_save_data(ISFU->width*ISFU->height);
            std::cout<<"/-expansion-/"<<"///"<<std::endl;
            this->template emplace<Win_data>(Win_data{(ISFU->width)});
        }
        int begin=ISFU->get_point_cursor_left_down_riding();
        ISFU->id_detector=this->template read_data<GLuint>(begin);

        if(ISFU->Change_wind){
            glClearColor(background_color.r, background_color.g,background_color.b,background_color.a);
            glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //this->template change<GLuint>(begin,0);
        }

        return ISFU->isChangeSize;
    }

};

#include <unordered_map>


template<template<
             class Object,
             typename Memory,
             template<typename K, uint  L> class Buffer,
             template<typename M,template<typename T__,uint n__> class B> class Win_storage,
             template<typename M,template<typename T_,uint n_> class B>  class ... Arg
          > class T>
using Milieu_=T<CeObject,Storage_res,Buffer_GL,Controller_Win_storage,Controller_Res_2d_form__,Controller_Res_2d_text__,Controller_Res_2d_line__>;


#endif // TYPE_RESOURCE_H
