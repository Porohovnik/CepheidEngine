#ifndef RENDERING_ENGINE_H
#define RENDERING_ENGINE_H
#include "tuple_utl.h"
#include "draw_program.h"
#include "CeEnumeration.h"
#include "Identification.h"

#include <iostream>
#include "interlayer_gl.h"
namespace CeEngine {
template<typename DRAW,typename TT>
inline static void  rend(TT &data, std::size_t i,std::size_t size){
    using T=std::remove_reference_t<TT>;
    if constexpr(T::template check_type<DRAW>()){
        //std::cout<<"%%%%%%%%%%%%%%%% "<<GL_layer::Get_Error()<<std::endl;
        if constexpr(T::type_draw() & GL_layer::TYPE_RENDERING_BIND_BUFFER){
            const DRAW * t=nullptr;

            GL_layer::Draw<T::type_object(),T::type_draw()>(t,size);
        }else{
            GL_layer::Draw<T::type_object(),T::type_draw()>(data.template get_element<DRAW>(i),size);
        }
        //std::cout<<"%%%%%%%%%%%%%%%% "<<GL_layer::Get_Error()<<std::endl;
    }
}

template<typename T,typename ...Arg>
inline static void  rend_program(T &data){
    //std::cout<<"::->"<<std::endl;
    data.template bind<TIME_BIND::FIRST_SHOT,
                       TIME_BIND::FIRST_SHOT_FIRST_OBJECT,
                       TIME_BIND::FIRST_SHOT_OBJECT,
                       TIME_BIND::FIRST_SHOT_LAST_OBJECT,
                       TIME_BIND::SHOT_FIRST_OBJECT,
                       TIME_BIND::SHOT,
                       TIME_BIND::SHOT_LAST_OBJECT,
                       TIME_BIND::LAST_SHOT_FIRST_OBJECT,
                       TIME_BIND::LAST_SHOT_OBJECT,
                       TIME_BIND::LAST_SHOT_LAST_OBJECT,
                       TIME_BIND::LAST_SHOT>();
    auto size=data.get_element_size();
    if constexpr(std::remove_reference_t<T>::template check_bind_time<TIME_BIND::FIRST_OBJECT,TIME_BIND::OBJECT,TIME_BIND::LAST_OBJECT>()){
        for(std::size_t i=0;i<size;i++){
            std::size_t id=i;
            if constexpr(std::remove_reference_t<T>::template check_type<Identification>()){
                id=data.template get_element<Identification>(i)->id;
            }
            data.template bind<TIME_BIND::FIRST_OBJECT,
                               TIME_BIND::FIRST_SHOT_FIRST_OBJECT,
                               TIME_BIND::OBJECT,
                               TIME_BIND::FIRST_SHOT_LAST_OBJECT,
                               TIME_BIND::LAST_OBJECT>(id);
            std::cout<<i<<"||||||||||||||||||||||rend|||||||||||||:"<<size<<std::endl;
            ((rend<Arg>(data,i,1)),...);
        }
    }else{
        ((rend<Arg>(data,-1,size)),...);
    }
}

template<typename RES>
inline void rendering(RES * resourse){
    //глобально для окна
    //тут присоедение общего буффера окна//с поразбитем на листы, лист 0-всё окно
    //тут присоедение по возможности фреймбуффера
    //присоеденение к фреймбуферу считывания.

    resourse->Get_Win_storage()->template bind<TIME_BIND::FIRST_SHOT,
            TIME_BIND::FIRST_SHOT_FIRST_OBJECT,
            TIME_BIND::FIRST_SHOT_OBJECT,
            TIME_BIND::FIRST_SHOT_LAST_OBJECT,
            TIME_BIND::SHOT_FIRST_OBJECT,
            TIME_BIND::SHOT,
            TIME_BIND::SHOT_LAST_OBJECT,
            TIME_BIND::LAST_SHOT_FIRST_OBJECT,
            TIME_BIND::LAST_SHOT_OBJECT,
            TIME_BIND::LAST_SHOT_LAST_OBJECT,
            TIME_BIND::LAST_SHOT>();

        //static_assert (std::is_same<decltype(T),const List_rule>(),";l;");
        //тут присоедение по возможности фреймбуффера
    tutl::TupleForeach(resourse->Get_Data(),[](auto &t){
        rend_program<decltype (t),GL_layer::DrawArraysIndirectCommand,GL_layer::DrawElementsIndirectCommand>(t);
    });

    //GL_layer::Finish();
   //
}
}// namespace CeEngine
#endif // RENDERING_ENGINE_H
