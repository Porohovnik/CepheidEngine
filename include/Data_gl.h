#ifndef DATA_GL_H
#define DATA_GL_H
#include <vector>

#include "Data_ABC.h"
#include "Merge_controller.h"
namespace CeEngine {
template<TIME_BIND bind_time,int bind_base,GL_layer::BUFFER_SETTING type,GL_layer::TYPE_BUFFER type_buffer,template<typename K, GL_layer::BUFFER_SETTING a, GL_layer::TYPE_BUFFER c> class Buffer,typename T,typename J=T,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
class Data_gl:public Data_ABC<bind_time,T,J>{
    std::vector<T> RAM;
    Buffer<J,type,type_buffer> SSBO_VRAM;

    using Data =Data_gl<bind_time,bind_base,type,type_buffer,Buffer,T,J,fun,t_delete>;

    class Change_merge{
        Data * controller;
        std::ptrdiff_t id=-1;
    public:
        T * data;
        Change_merge(std::ptrdiff_t id_,Data *t,T *data_):id(id_),controller(t),data(data_){}

        inline auto confirm(){
            return controller->merg.add(id);
        }
    };

    class All_change{
        Data * controller;
    public:
        std::vector<T> * data;
        All_change(Data *t,std::vector<T> *data_):controller(t),data(data_){}

        inline auto confirm(){
            return controller->change_confirm(0,data->size());
        }
    };
public:
    inline static constexpr TRIVIAL_DELETE trivial_delete=t_delete;
    Merge_controller merg;

    template<typename Memory, typename Info_environment>
    Data_gl(Data_herald<Memory,Info_environment> herald):SSBO_VRAM(){}

    inline bool new_frame(){
        int b=false;
        auto t=merg.pop_data();
        for(auto &[ID,S]:t){
            change_confirm(ID-S,S);
            b+=true;
        }
        return b;
    }

    std::size_t add_element(std::size_t id,T &&element){
        if(RAM.size()<=id){
            RAM.resize(id+1);
        }

        RAM[id]=std::move(element);
        if constexpr(bind_time!=TIME_BIND::NEVER){
            if constexpr(fun!=nullptr){
                auto t=fun(RAM[id]);
                SSBO_VRAM.insert(id,&t);
            }else{
                SSBO_VRAM.insert(id,&RAM[id]);
            }
        }
        return id;
    }

    inline bool change(std::size_t id, T&&element){
        RAM[id]=element;
        return  change_confirm(id);
    }

    inline auto change_request(std::size_t id){
        return typename Data_ABC<bind_time,T,J>::template Change_guard<Change_merge>(id,this,&RAM[id]);
    }

    inline auto change_request(){
        return typename Data_ABC<bind_time,T,J>::template Change_guard<All_change>(this,&RAM);
    }

    inline bool change_confirm(std::size_t id){
        if constexpr(bind_time!=TIME_BIND::NEVER){
            if constexpr(fun!=nullptr){
                auto t=fun(RAM[id]);
                SSBO_VRAM.changes(&t,id,id+1);
            }else{
                SSBO_VRAM.changes(&RAM[id],id,id+1);
            }
        }
        return true;
    }

    inline bool change_confirm(std::size_t id, std::size_t size){
        if constexpr(bind_time!=TIME_BIND::NEVER){
            if constexpr(fun!=nullptr){
                std::vector<J> t;
                for(std::size_t i=0;i<size;i++){
                    t.emplace_back(fun(RAM[id+i]));
                }
                SSBO_VRAM.changes(t.data(),id,id+size);
            }else{
                SSBO_VRAM.changes(&RAM[id],id,id+size);
            }
        }
        return true;
    }


    inline bool delete_element(std::size_t id){
        // так надо, оптимизация, при удаление нечего тут не делаем)
        return true;
    }

    inline void bind(int id) const{}

    inline void bind() const{
        if constexpr(bind_time!=TIME_BIND::NEVER){
            SSBO_VRAM.Bind(bind_base);
        }
    }

    const auto * get_element(std::size_t id) const{
        return &RAM[id];
    }

    auto * get_element_not_const(std::size_t id){
        return &RAM[id];
    }

    inline bool read_data(int begin,int end,std::vector<T> &data) const{
        if(data.size()<(end-begin)){
            data.resize(end-begin);
        }
        SSBO_VRAM.read_data(begin,end,data.data());
        return true;
    }
};
}// namespace CeEngine
#endif // DATA_GL_H
