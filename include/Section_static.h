#ifndef SECTION_STATIC_H
#define SECTION_STATIC_H
#include "Section.h"
namespace CeEngine {
template<typename J,typename T>
class Section_static:public Section<J,T>{
protected:
    inline static int id=0;
    int id_current;
    std::function<void()> to_vram=nullptr;
public:
    template<typename T_st,size_t... i,typename ...Arg>//нужен массив бд
    Section_static(std::index_sequence<i...>,T_st  t_st ,Arg && ...arg):id_current(id),Section<J,T>(std::forward<Arg>(arg)...){
        to_vram=[this,t_st](){
            this->VRAM.emplace(std::get<i>(t_st)...,this->id_current,std::move(this->RAM.value()));
        };

        id++;
    }

    template<typename ...Arg_t,typename ...Arg>
    Section_static(std::tuple<Arg_t...> t_st,Arg && ...arg):
        Section_static<J,T>(std::make_index_sequence<sizeof... (Arg_t)>(),t_st,std::forward<Arg>(arg)...){}


    void to_VRAM(){
        if(this->RAM && !this->VRAM && to_vram){
            to_vram();//предпологается наличие статичных переменных
            this->RAM.reset();
        }
    }

     int get_id() const noexcept{
        return id_current;
    }
};
}// namespace CeEngine
#endif // SECTION_STATIC_H
