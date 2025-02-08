#ifndef CONTEXT_H
#define CONTEXT_H
#include "window.h"

#include "rendering_engine.h"
#include "signal_engine.h"
#include "resource_engine.h"
#include "object_engine.h"


#include <chrono>
#include <thread>

template<class Object,typename Context_storage,typename ...Storage>
class Context:public Win{
public:
    inline static  Object_engine<Object>   objects{std::optional<Context>{}};//потом сделать иначе  //вообще плевать на окна

    using List= typename Resource_engine<Context_storage,Storage...>::List;
    Resource_engine<Context_storage,Storage...>    resource;//точно зависит от контекста окна

    Signal_engine<Object>                 signal{};//косвенно зависит от окна(позволяет нереагировать на одинаковые клавиши из обоих окон)

    Context(std::string name_okno_,int WIDTH, int HEIGHT,glm::vec4 background_color_):
        Win(name_okno_,WIDTH,HEIGHT),resource(background_color_,&this->ISFU,0){}//нельзя ресурсы инцилизировать, до glewInit



    inline static void * current_context=nullptr;//так можно, ибо констекст это общая для вообще всего
    void new_fraem(){ 
        if(current_context!=this){
            this->make_context();
        }
        current_context=this;

        bool change_view_win=false;
        resource.new_frame();
        if(resource.Get_cout_call()){
            rendering(&resource);
            change_view_win=true;
            this->swap_buffer();//
            std::cout<<"44444444444"<<std::endl;
        }else {
            std::chrono::milliseconds timespan(16);
            std::this_thread::sleep_for(timespan);
        }

        signal.new_frame(&ISFU);
        ISFU.Change_wind=change_view_win;
        ISFU.new_frame();
        //std::cout<<"555555555555"<<std::endl;
    }
};
#endif // CONTEXT_H
