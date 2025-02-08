#ifndef DATA_GL_ARRAY_NO_RAM_NO_ALLOCATE_H
#define DATA_GL_ARRAY_NO_RAM_NO_ALLOCATE_H
#include <vector>

#include "Data_ABC.h"
#include "free_memory_controler.h"
namespace CeEngine {
template<TIME_BIND bind_time,int bind_base,GL_layer::BUFFER_SETTING type,GL_layer::TYPE_BUFFER type_buffer,
         template<typename K, GL_layer::BUFFER_SETTING a,GL_layer::TYPE_BUFFER c> class Buffer,
         typename T,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES,std::size_t size_buffer=0>
class Data_gl_array_no_ram_no_allocate:public Data_ABC<bind_time,T,typename T::value_type>{
    struct Type_array{
        int begin=0;//из-за GLSL
        int max_size=0;//из-за GLSL
    };

    Allocator_free_space memory_controller;
    Buffer<typename T::value_type,type,type_buffer> SSBO_VRAM;

    std::vector<Type_array> vram_position;
public:
    inline static constexpr TRIVIAL_DELETE trivial_delete=t_delete;

    Data_gl_array_no_ram_no_allocate():SSBO_VRAM(nullptr,size_buffer),memory_controller(size_buffer){}

    inline bool new_frame(){return  false;}

// можно использовать для "резервирования" места, дабы потом заполнить
    std::size_t get_begin_of_controller(std::size_t size_max_text){
        auto b=memory_controller.pull_space(size_max_text);

        while(b.command!=0){
            auto k=SSBO_VRAM.get_max_size();
            SSBO_VRAM.expansion_buffer(k*1.5+size_max_text);
            memory_controller.expansion(k*0.5+size_max_text);
            b=memory_controller.pull_space(size_max_text);
        }
        return b.begin;
    }

    //element -array/vector
    auto add_element(std::size_t id,const T &&element){
        if(vram_position.size()<=id){
            vram_position.resize(id+1);
        }
        //расчёт оптимальное местоположения
        auto begin=get_begin_of_controller(element.size());

        vram_position[id]={static_cast<int>(begin),static_cast<int>(element.size())};

        //добавление элемента
        SSBO_VRAM.insert(begin,element.data(),element.size());

        return begin;
    }

    auto add_element(std::size_t id,std::size_t begin,const T &&element){
        if(vram_position.size()<=id){
            vram_position.resize(id+1);
        }

        vram_position[id]={static_cast<int>(begin),static_cast<int>(element.size())};

        SSBO_VRAM.insert(begin,element.data(),element.size());

        return begin;
    }

    inline auto change(std::size_t id, T&&element){
        auto [begin,max_size]=vram_position[id];

        memory_controller.push(begin,max_size);
        begin=get_begin_of_controller(element.size());
        vram_position[id]={begin,element.size()};


        SSBO_VRAM.changes(element.data(),begin,element.size()+begin);

        return true;

    }

    inline bool read_data(int begin,int end,std::vector<T> &data)const{
        if(data.size()<(end-begin)){
            data.resize(end-begin);
        }
        SSBO_VRAM.read_data(begin,end,data.data());
        return true;
    }

    bool delete_element(std::size_t id){
        if(vram_position.size()<=id){
            return true;
        }
        //std::cout<<"||||||||||||||||||||||delete_element|||||||||||||:"<<std::endl;
        auto [begin,max_size]=vram_position[id];
        memory_controller.push(begin,max_size);

        return true;
    }

    inline void bind(int id) const{}
    inline void bind() const{
        if constexpr(bind_time!=TIME_BIND::NEVER){
            SSBO_VRAM.Bind(bind_base);
        }
    }
    inline void Bind() const{//более приорететный вызов для удобства API
        SSBO_VRAM.Bind(bind_base);
    }

};
}// namespace CeEngine
#endif // DATA_GL_ARRAY_NO_RAM_NO_ALLOCATE_H
