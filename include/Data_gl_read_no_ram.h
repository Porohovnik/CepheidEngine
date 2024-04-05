#ifndef DATA_GL_READ_NO_RAM_H
#define DATA_GL_READ_NO_RAM_H
#include <vector>

#include "Data_ABC.h"
namespace CeEngine {
template<TIME_BIND bind_time,int bind_base,GL_layer::BUFFER_SETTING type,GL_layer::TYPE_BUFFER type_buffer,template<typename K,  GL_layer::BUFFER_SETTING a,GL_layer::TYPE_BUFFER c> class Buffer,typename T,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
class Data_gl_read_no_ram:public Data_ABC<bind_time,T,T>{
    mutable Buffer<T,type,type_buffer> SSBO_VRAM;
public:
    inline static constexpr TRIVIAL_DELETE trivial_delete=t_delete;

    inline bool new_frame(){return  false;}

    template<typename Memory, typename Info_environment>
    Data_gl_read_no_ram(Data_herald<Memory,Info_environment> herald):SSBO_VRAM(){}

    inline void bind(int id) const{}

    inline void bind() const{
        SSBO_VRAM.Bind(bind_base);
    }

    inline bool read_data(int begin,int end,std::vector<T> &data) const{
        if(data.size()<(end-begin)){
            data.resize(end-begin);
        }
        SSBO_VRAM.read_data(begin,end,data.data());
        return true;
    }


    auto change(int begin, T &&element){
        SSBO_VRAM.changes(&element,begin,begin+1);
        return true;
    }

    auto add_element(std::size_t id,std::vector<T> &&element){
        SSBO_VRAM.changes(element.data(),id,element.size());
    }

    bool delete_element(std::size_t id){
        return true;
    }

    inline T read_data(int pos) const{
        return SSBO_VRAM.read_data(pos);
    }

    constexpr auto * get_element() {
        return &SSBO_VRAM;
    }
};
}// namespace CeEngine
#endif // DATA_GL_READ_NO_RAM_H
