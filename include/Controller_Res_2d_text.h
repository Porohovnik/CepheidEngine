#ifndef CONTROLLER_RES_2D_TEXT_H
#define CONTROLLER_RES_2D_TEXT_H

#include <memory>
#include <filesystem>

#include "controler_utilite.h"
#include "CeEnumeration.h"

#include "Storage_data.h"
#include "Data_gl.h"
#include "Data_res.h"
#include "Data_res_one.h"
#include "Data_gl_array.h"

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
#include "Font_controller_BINDLESS.h"
#include "glifs.h"
using Font_S = Section_static<Data_font,Font_controller_BINDLESS>;
#else
#include "texture.h"
#include "font.h"
#include "glifs.h"
using Font_S = Section_static<Data_font,Font_controller<Texture>>;
#endif

using Buffer_mesh_S =Section<std::shared_ptr<Mesh_data>,Buffer_mesh>;
using Shades_S =Section<GL_layer::GLSL_code_data,Program_GPU>;

using Storage_res_text=Storage_resource<
    //в первую очередь
    Unit_res<TYPE_ORDER_UPDATE::FIRST,std::string,Shades_S>,
    //когда угодно, после первой...
    Unit_res<TYPE_ORDER_UPDATE::ANY  ,std::string,Font_S>,
    Unit_res<TYPE_ORDER_UPDATE::ANY  ,std::string,Buffer_mesh_S>
>;


#include "Identification.h"
#include "position.h"
#include "color.h"

struct Material_T{//аккуратно!!
    int id=0;
    int map_font=0;
    Color color;
};


#include "buffer_gl.h"

template<TIME_BIND bind_time,int bind_base,typename T,typename J=T,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_S=Data_gl  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::SHADER_STORAGE_BUFFER,CeEngine::Buffer_GL,T,J,fun,t_delete>;

template<TIME_BIND bind_time,int bind_base,typename T,typename J=T,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_I=Data_gl  <bind_time,bind_base,GL_layer::BUFFER_SETTING::STATIC,GL_layer::TYPE_BUFFER::DRAW_INDIRECT_BUFFER,CeEngine::Buffer_GL,T,J,fun,t_delete>;

template<TIME_BIND bind_time,int bind_base_aloc,int bind_base,typename T,typename J=typename T::value_type,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
using Data_gl_array_S=Data_gl_array<TIME_BIND::SHOT,  bind_base_aloc,     bind_base,GL_layer::BUFFER_SETTING::DYNAMIC ,GL_layer::TYPE_BUFFER::SHADER_STORAGE_BUFFER,CeEngine::Buffer_GL,T,J,fun,t_delete>;

template<typename Memory>
using Res_2d_text__=
Storage_data<
    Memory,
#ifdef BINDLES_TEXTURE
    GL_layer::TYPE_OBJECT_DRAW::TYPE_OBJECT_TRIANGLES,
    GL_layer::TYPE_RENDERING_ELEMENT | GL_layer::TYPE_RENDERING_MYLTI | GL_layer::TYPE_RENDERING_INDERECT | GL_layer::TYPE_RENDERING_BIND_BUFFER,
    Data_gl_I      <TIME_BIND::SHOT, -1,GL_layer::DrawElementsIndirectCommand,GL_layer::DrawElementsIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
    Data_res_one <TIME_BIND::FIRST_SHOT_LAST_OBJECT,                          Memory,std::shared_ptr<Shades_S>,-1>,
#else
    GL_layer::TYPE_OBJECT_DRAW::TYPE_OBJECT_TRIANGLES,
    GL_layer::TYPE_RENDERING_ELEMENT | GL_layer::TYPE_RENDERING_INSTANSE,
    Data_gl_I      <TIME_BIND::NEVER,-1,       GL_layer::DrawElementsIndirectCommand,GL_layer::DrawElementsIndirectCommand,nullptr,TRIVIAL_DELETE::NO>,
    Data_res_one <TIME_BIND::FIRST_SHOT_LAST_OBJECT,                          Memory,std::shared_ptr<Shades_S>,0>,
#endif
    Data_gl_I      <TIME_BIND::NEVER,-2,       GL_layer::DrawElementsIndirectCommand_menedger>,
    Data_gl_S      <TIME_BIND::SHOT,  0,       Identification>,
    Data_gl_S      <TIME_BIND::SHOT,  1,       Position,glm::mat4,fooooo>,
    Data_gl_S      <TIME_BIND::SHOT,  2,       Material_T>,
                //некрасиво числа aloc и данных
    Data_gl_array_S<TIME_BIND::SHOT,  3,     4,std::vector<int>>,//аккуратно!!
    Data_res_one <TIME_BIND::SHOT,                                            Memory,std::shared_ptr<Buffer_mesh_S>>,
                //некрасиво числа aloc и данных
    Data_gl_array_S<TIME_BIND::SHOT,  6,     7,std::vector<Glif_info>,Glif_info,nullptr,TRIVIAL_DELETE::NO>,
#ifdef BINDLES_TEXTURE
    Data_gl_S      <TIME_BIND::SHOT,  5,       uint64_t,uint64_t,nullptr,TRIVIAL_DELETE::NO>,
    Data_res     <TIME_BIND::NEVER,                                           Memory,std::shared_ptr<Font_S>,-1>
#else
    Data_res    <TIME_BIND::OBJECT,                                           Memory,std::shared_ptr<Font_S>,0>
#endif
> ;


enum class TYPE_TEXT{ANSI=0,UNICOD=1};

class Controller_Res_2d_text: public Res_2d_text__<Storage_res_text>{
    struct Data_CeText{
        std::filesystem::path path_font="";
        std::string text="";
        int size_max_text=0;
        int mod=-1;
        Color color={1.0f,1.0f,1.0f,1.0f};

        Position pos{1.0f};
        Data_CeText(std::filesystem::path path_font_,std::string text_=""):path_font(path_font_),text(text_){}
    };
public:
    using Data=Data_CeText;

    template<typename Info_environment>
    Controller_Res_2d_text(Info_environment * info):Res_2d_text__<Storage_res_text>(info){
        this->template emplace<std::shared_ptr<Buffer_mesh_S>>("plate",plate_mesh());///

        GL_layer::GLSL_code_data data_shaders;
        data_shaders.set_glsl_code<GL_layer::TYPE_SHADER::VERTEX_SHADER>  (Directory_shaders+"text_shader.vert");
        data_shaders.set_glsl_code<GL_layer::TYPE_SHADER::FRAGMENT_SHADER>(Directory_shaders+"text_shader.frag");

        this->template emplace<std::shared_ptr<Shades_S>>("TEXT_SHADER",data_shaders);///

        std::cout<<"text_inz"<<std::endl;
    }

    virtual void add_data_controller(std::size_t id,std::any &data_){
        Data data=std::any_cast<Data>(data_);
        std::cout<<"TEXT"<<std::endl;
        //тут формулка что из позиции вычесляет размер текста
        std::size_t size=40;

        std::pair<float,int> mapping[6]={
            {0.0f,20},{0.01,40},{0.05,50},{0.1,60},{0.4,80},{0.6,100}
        };
        for(auto &M:mapping){
            if(data.pos.pos()[1][1]/2.0f>=M.first){
                size=M.second;
            }
        }

        //std::cout<<pos.pos()[1][1]/2.0f<<"::"<<size<<std::endl;
        std::string hesh=data.path_font.string()+ std::to_string(size);//???

        std::vector<int> sumbols;
        string_to_unicod(data.text.c_str(),&sumbols);
        converting_unicode_to_glyph_number(sumbols);
        this->template add_element<std::vector<int>>(id,sumbols,data.size_max_text);


        #ifdef BINDLES_TEXTURE
        this->template add_element<std::shared_ptr<Font_S>>(
            id,hesh,
            std::tuple(this-> template get_element_data_GL<std::vector<Glif_info>>(),this-> template get_element_data_GL<uint64_t>())
            ,data.path_font,size);
        #else
        this->template add_element<std::shared_ptr<Font_S>>(
            id,hesh,
            std::tuple(this-> template get_element_data_GL<std::vector<Glif_info>>())
            ,data.path_font,size);
        #endif

        int id_map_bd=(*this-> template get_element<std::shared_ptr<Font_S>>(id))->get_id();//а вот тут и  проблема, можно ли получть id в бд без бд?) Можно!

        this->template add_element<Position>(id,data.pos);
        this->template add_element<Material_T>(id,Material_T{id_global++,id_map_bd,data.color});
        ///для биндле
        GL_layer::DrawElementsIndirectCommand_menedger cmd_m;
        cmd_m.cmd.instanceCount=sumbols.size();
        cmd_m.id=id;

        this->template add_element<GL_layer::DrawElementsIndirectCommand>(this->get_element_size(),cmd_m.cmd);
        this->template add_element<GL_layer::DrawElementsIndirectCommand_menedger>(id,cmd_m);
    }

    virtual std::size_t visible_data_controller(std::list<std::list<Identification *> *> * ids,bool update=false){
        return visible_data_controller_any<GL_layer::DrawElementsIndirectCommand,GL_layer::DrawElementsIndirectCommand_menedger>(this,ids,update);
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

        if(this->template get_element<GL_layer::DrawElementsIndirectCommand_menedger>(id)->cmd.instanceCount!=(*change.get_data()).size()){
            auto cmd_m=this->template change_request<GL_layer::DrawElementsIndirectCommand_menedger>(id);
            cmd_m.get_data()->cmd.instanceCount=(*change.get_data()).size();

            *(this->template change_request<GL_layer::DrawElementsIndirectCommand>(cmd_m.get_data()->id).get_data())=cmd_m.get_data()->cmd;
        }

        return true;
    }
};

#endif // CONTROLLER_RES_2D_TEXT_H
