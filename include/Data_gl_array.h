#ifndef DATA_GL_ARRAY_H
#define DATA_GL_ARRAY_H
#include <vector>

#include "Data_ABC.h"
#include "free_memory_controler.h"
namespace CeEngine {
template<TIME_BIND bind_time,int bind_base_aloc,int bind_base,GL_layer::BUFFER_SETTING type,GL_layer::TYPE_BUFFER type_buffer,template<typename K, GL_layer::BUFFER_SETTING a,GL_layer::TYPE_BUFFER c> class Buffer,typename T,typename J=typename T::value_type,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
class Data_gl_array:public Data_ABC<bind_time,T,J>{
    struct Type_array{
        int begin=0;//из-за GLSL
        int max_size=0;//из-за GLSL
    };

    using Data =Data_gl_array<bind_time,bind_base_aloc,bind_base,type,type_buffer,Buffer,T,J,fun,t_delete>;

    class Change{
        Data * controller;
        std::ptrdiff_t id=-1;
    public:
        T * data;
        Change(std::ptrdiff_t id_,Data *t,T *data_):id(id_),controller(t),data(data_){}

        inline auto confirm(){
            return controller->change_confirm(id);
        }
    };

    std::vector<std::pair<Type_array,T>> RAM;
    Allocator_free_space memory_controller;

    Buffer<J,type,type_buffer> SSBO_VRAM;
    Buffer<Type_array,type,type_buffer> SSBO_VRAM_allocate;
public:
    inline static constexpr TRIVIAL_DELETE trivial_delete=t_delete;

    template<typename Memory, typename Info_environment>
    Data_gl_array(Data_herald<Memory,Info_environment> herald):
        SSBO_VRAM(),SSBO_VRAM_allocate(),memory_controller(0){}

    inline bool new_frame(){return  false;}

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
    auto add_element(std::size_t id,T &&element,std::size_t size_max_text_){
        if(RAM.size()<=id){
            RAM.resize(id+1);
        }

        std::size_t size_max_text=size_max_text_;
        //защита от странного ввода
        if(element.size()>size_max_text){
            size_max_text=element.size();
        }
        //расчёт оптимальное местоположения
        auto begin=get_begin_of_controller(size_max_text);
        //добавление элемента

        //сделать проверку на переполнение int или изменить тип на GPU
        RAM[id]={{static_cast<int>(begin),static_cast<int>(size_max_text)},element};

        if constexpr(bind_time!=TIME_BIND::NEVER){
            if constexpr(fun!=nullptr){
                auto t=fun(RAM[id].second);
                SSBO_VRAM.insert(begin,&t,t.size(),0);
            }else{
                SSBO_VRAM.insert(begin,RAM[id].second.data(),RAM[id].second.size());
            }

            SSBO_VRAM_allocate.insert(id,&RAM[id].first);
        }
        return begin;
    }


    inline auto change(std::size_t id, T&&element){
        RAM[id].second=element;
        return change_confirm(id);
    }

    auto change_request(std::size_t id){
        return typename Data_ABC<bind_time,T,J>::template Change_guard<Change>(static_cast<std::ptrdiff_t>(id),this,&RAM[id].second);
    }

    auto change_confirm(std::size_t id){
        auto [begin,max_size]=RAM[id].first;
        std::size_t size_element=RAM[id].second.size();
        std::size_t size_max_text=max_size;

        if(size_element>max_size){
            size_max_text=size_element;
            memory_controller.push(begin,max_size);
            begin=get_begin_of_controller(size_max_text);

            //сделать проверку на переполнение int или изменить тип на GPU
            RAM[id].first={static_cast<int>(begin),static_cast<int>(size_max_text)};
        }
        //std::cout<<size_element<<"|"<<size_max_text<<std::endl;

        if constexpr(bind_time!=TIME_BIND::NEVER){
            if constexpr(fun!=nullptr){
                auto t=fun(RAM[id].second);
                SSBO_VRAM.changes(t.data(),begin,t.size()+begin);
            }else{
                SSBO_VRAM.changes(RAM[id].second.data(),begin,RAM[id].second.size()+begin);
            }

            SSBO_VRAM_allocate.changes(&RAM[id].first,id,id+1);
        }
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
        if constexpr(bind_time!=TIME_BIND::NEVER){
            if(RAM.size()<=id){
                //std::cout<<RAM.size()<<"text|"<<id<<std::endl;
                return true;
            }

            if(id>=1150){
                //std::cout<<id<<"|>"<<RAM[id].first.begin<<"|"<<RAM[id].first.max_size<<std::endl;
            }

            memory_controller.push(RAM[id].first.begin,RAM[id].first.max_size);
            RAM[id].first.max_size=0;
        }
        return true;
    }

    inline void bind(int id) const{}
    inline void bind() const{
        if constexpr(bind_time!=TIME_BIND::NEVER){
            SSBO_VRAM.Bind(bind_base);
            SSBO_VRAM_allocate.Bind(bind_base_aloc);
        }
    }
    const auto * get_element(std::size_t id) const{
        return &RAM[id];
    }


    auto * get_element_not_const(std::size_t id) const{
        return &RAM[id];
    }
};
}// namespace CeEngine
#endif // DATA_GL_ARRAY_H
