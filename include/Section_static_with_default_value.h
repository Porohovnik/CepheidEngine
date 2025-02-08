#ifndef SECTION_STATIC_WITH_DEFAULT_VALUE_H
#define SECTION_STATIC_WITH_DEFAULT_VALUE_H
#include <Section_static.h>
namespace CeEngine {
template<typename J,typename T>
class Section_static_with_default_value:public Section_static<J,T>{
    template<typename T_st,size_t... i>
    void default_value(std::index_sequence<i...>,T_st  t_st){
         T::template default_value<decltype (std::get<i>(t_st))...,J>(std::get<i>(t_st)...,this->id_current);
    }

public:
    template<typename ...Arg_t,typename ...Arg>
    Section_static_with_default_value(std::tuple<Arg_t...> t_st,Arg && ...arg):
        Section_static<J,T>(std::make_index_sequence<sizeof... (Arg_t)>(),t_st,std::forward<Arg>(arg)...){
        default_value      (std::make_index_sequence<sizeof... (Arg_t)>(),t_st);
    }
};
}// namespace CeEngine
#endif // SECTION_STATIC_WITH_DEFAULT_VALUE_H
