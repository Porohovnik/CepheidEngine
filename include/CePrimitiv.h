#ifndef CEPRIMITIV_H
#define CEPRIMITIV_H
#include <functional>
#include <any>

#include "Identification.h"
#include "color.h"
namespace CeEngine {
using uint =unsigned int;

template<auto name_type,
         typename Material,
         class Base_controller,
         typename Base_controller::Data(* f)(std::any)>//обощить для разных способов
class CePrimitiv: protected Identification{
public:
    inline static std::string name_primitive=name_type;
    inline static std::function<typename Base_controller::Data(std::any)> fun_data=f;
    using Data = typename Base_controller::Data;

    std::size_t id_global;
    Base_controller * controller;
    std::function<void()> deleter;

    template<typename List>
    CePrimitiv(List * celist,typename Base_controller::Data data):controller(celist->template get_element<Base_controller>()){
        this->id=       controller->add_data(data);
        //обновление, с учётом листа
        auto it= celist->template attach_object<Base_controller>(this);//вынести
        controller->template new_status(celist->get_status(),this);

        deleter=[celist,it](){
            celist->template reattach_object<Base_controller>(it);
        };

        id_global=controller->template get_element<Material>(this->id)->id;//придумать что-то более изящное
        //std::cout<<name_primitive<<std::endl;
    }

    template<typename TYPE>
    auto change(){
        return controller-> template change_request<TYPE>(this->id);
    }

    void change_color(std::string color){//придумать что-то более изящное
        Color color_new=std::stoi(std::string(color),nullptr,16);
        *(this->template change<Material>().get_data().color)=color_new;
    }

    template<typename TYPE>
    const TYPE * get(){
        return controller-> template get_element<TYPE>(this->id);
    }

    ~CePrimitiv(){
        if(controller!=nullptr){
           deleter();
           controller->delete_element(this->id);
        }
    }
};
}// namespace CeEngine
#endif // CEPRIMITIV_H
