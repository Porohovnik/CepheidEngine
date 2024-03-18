#ifndef CONTROLLER_WIN_STORAGE_H
#define CONTROLLER_WIN_STORAGE_H
#include <memory>
#include <filesystem>

#include "controler_utilite.h"
#include "CeEnumeration.h"

#include "Storage_data.h"
#include "Data_gl_one.h"
#include "Data_gl_read_no_ram.h"

#include "Storage_resource.h"
#include "Unit_res.h"
#include "Section.h"
#include "Section_static.h"

#include "draw_program.h"

#include "Program_GPU.h"
#include "GLSL_code_data.h"


using Shades_S =Section<GL_layer::GLSL_code_data,Program_GPU>;
using Storage_res_win=Storage_resource<
    //в первую очередь
    Unit_res<TYPE_ORDER_UPDATE::FIRST,std::string,Shades_S>
>;


struct Win_data{
    int widht=0;
};

using GL_layer::BUFFER_SETTING;
using GL_layer::TYPE_BUFFER;

template<typename Memory,template<typename K, GL_layer::BUFFER_SETTING a,GL_layer::TYPE_BUFFER c> class Buffer>
using Win_storage__=
Storage_data<
    Memory,
    GL_layer::TYPE_OBJECT_DRAW::TYPE_OBJECT_TRIANGLES,
    GL_layer::TYPE_RENDERING_ELEMENT,
//    Data_res_one<TIME_BIND::FIRST_SHOT_FIRST_OBJECT,                               Memory,std::shared_ptr<Shades_S<2>>,0>,
//    Data_res_one <TIME_BIND::SHOT,                                          Memory,std::shared_ptr<Buffer_mesh_S>>,
    Data_gl_read_no_ram <TIME_BIND::SHOT,10,BUFFER_SETTING::DYNAMIC,TYPE_BUFFER::SHADER_STORAGE_BUFFER,Buffer,GL_layer::GLuint,TRIVIAL_DELETE::NO>,
    Data_gl_one         <TIME_BIND::SHOT,11,BUFFER_SETTING::DYNAMIC,TYPE_BUFFER::SHADER_STORAGE_BUFFER,Buffer,Win_data>
>;

#include "buffer_gl.h"
#include "incoming_signals_from_user.h"
#include "color.h"

class Controller_Win_storage:public Win_storage__<Storage_res_win,Buffer_GL>{
    Win_layer::Incoming_signals_from_user * ISFU;//пока констекст жив, это тоже
    Color background_color;
public:
    Controller_Win_storage(Color background_color_,Win_layer::Incoming_signals_from_user * info):
        Win_storage__<Storage_res_win,Buffer_GL>(info),background_color(background_color_),ISFU(info){
        std::cout<<"Win_inz"<<std::endl;

//        this-> template  emplace<std::shared_ptr<Buffer_mesh_S>>(plate_mesh().hash(),plate_mesh);//
//        this->new_status(Type_Status::VISEBLE);
    }

    bool t=false;
    virtual bool new_frame_controller(){
        if(ISFU->isChangeSize){
            GL_layer::Viewport(0, 0, ISFU->width,ISFU->height);
            this->template get_element<GL_layer::GLuint>()->expansion_not_save_data(ISFU->width*ISFU->height);
            std::cout<<"/-expansion-/"<<"///"<<std::endl;
            this->template emplace<Win_data>(Win_data{(ISFU->width)});
            t=ISFU->isChangeSize;
        }
        int begin=ISFU->get_point_cursor_left_down_riding();
        ISFU->id_detector=this->template read_data<GL_layer::GLuint>(begin);
        //std::cout<<"/-expansion-/"<<ISFU->id_detector<<std::endl;

        if(ISFU->Change_wind){
            GL_layer::ClearColor(background_color.r, background_color.g,background_color.b,background_color.a);
            GL_layer::Clear(GL_layer::get_value_GL<GL_layer::TYPE_SETTING_CLEAR::COLOR_BUFFER_BIT>() |
                            GL_layer::get_value_GL<GL_layer::TYPE_SETTING_CLEAR::DEPTH_BUFFER_BIT>() );

            //this->template change<GLuint>(begin,0);
        }


        return ISFU->isChangeSize;
    }

};

#endif // CONTROLLER_WIN_STORAGE_H
