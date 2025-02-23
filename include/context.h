#ifndef CONTEXT_H
#define CONTEXT_H
#include "window.h"

#include "rendering_engine.h"
#include "signal_engine.h"
#include "resource_engine.h"
#include "object_engine.h"

#include <chrono>
#include <thread>
namespace CeEngine {
template<class Object,typename Context_storage,typename ...Storage>
class Context:public Win_layer::Win{
    bool isGLEW_true=false;

public:
    inline static  Object_engine<Object>   objects{std::optional<Context>{}};//потом сделать иначе  //вообще плевать на окна

    using List= typename Resource_engine<Context_storage,Storage...>::List;
    Resource_engine<Context_storage,Storage...>    resource;//точно зависит от контекста окна

    Signal_engine<Object>                 signal{};//косвенно зависит от окна(позволяет нереагировать на одинаковые клавиши из обоих окон)

    Context(std::string name_okno_,int WIDTH, int HEIGHT,Color background_color_):
        Win(name_okno_,WIDTH,HEIGHT),isGLEW_true(GL_layer::Init_GLEW()), resource(background_color_,&this->ISFU){}//нельзя ресурсы инцилизировать, до glewInit


    inline static void * current_context=nullptr;//так можно, ибо констекст это общая для вообще всего
    void new_fraem(){
        if(current_context!=this){
            this->make_context();//Win
        }
        current_context=this;

        resource.new_frame();
        resource.Get_Win_storage()->Change_wind=resource.Get_cout_call();
        if(resource.Get_Win_storage()->Change_wind){
            rendering(&resource);
            this->swap_buffer();//Win
            //std::cout<<"44444444444"<<std::endl;
        }else {
            std::chrono::milliseconds timespan(16);
            std::this_thread::sleep_for(timespan);
        }

        signal.new_frame(&ISFU);
        ISFU.new_frame();
        //std::cout<<"555555555555"<<std::endl;
    }
};
}// namespace CeEngine
#endif // CONTEXT_H
