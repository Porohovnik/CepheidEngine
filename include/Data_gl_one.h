#ifndef DATA_GL_ONE_H
#define DATA_GL_ONE_H

#include "Data_ABC.h"
namespace CeEngine {
template<TIME_BIND bind_time,int bind_base,GL_layer::BUFFER_SETTING type,GL_layer::TYPE_BUFFER type_buffer,template<typename K, GL_layer::BUFFER_SETTING a,GL_layer::TYPE_BUFFER c> class Buffer,typename T,typename J=T,J fun(T&)=nullptr>
class Data_gl_one:public Data_ABC<bind_time,T,J>{
    T RAM;
    Buffer<J,type,Data_ABC<bind_time,T,J>::isNever_to_NULL(type_buffer)> SSBO_VRAM;
public:
    inline static constexpr TYPE_CONTAINER type_container=TYPE_CONTAINER::ONE;


    Data_gl_one():SSBO_VRAM(nullptr,1){}

    bool emplace(T&&element){
        RAM=element;
        if constexpr(bind_time!=TIME_BIND::NEVER){
            if constexpr(fun!=nullptr){
                auto t=fun(RAM);
                SSBO_VRAM.changes(&t,0,1);
            }else{
                SSBO_VRAM.changes(&RAM,0,1);
            }
        }
        return true;
    }

    bool earse(){
//        RAM=nullptr;
//        if constexpr(bind_time!=NEVER){
//            SSBO_VRAM.excl0.ude(0,1);
//        }
        return true;
    }

    inline void bind(int id) const{}

    inline void bind() const{
        if constexpr(bind_time!=TIME_BIND::NEVER){
            SSBO_VRAM.Bind(bind_base);
        }
    }
    constexpr auto * get_element(int id=0) const{
        return RAM;
    }

    inline T read_data(int pos) const{
        return SSBO_VRAM.read_data(0);
    }
};
}// namespace CeEngine
#endif // DATA_GL_ONE_H
