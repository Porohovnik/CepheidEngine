#ifndef CEPLANE_H
#define CEPLANE_H
#include "CePlate.h"
#include "CeObjectSVG.h"

#include <variant>
#include "tuple_utl.h"
namespace CeEngine {
class CePlane: public CeObject{
    std::variant<CePlate,CeObjectSVG> object;
public:
    struct Data_initialization{
        std::filesystem::path path;
        std::string type_mesh="plate";
        Color color;
        Position pos{1.0f};
        Data_initialization(std::filesystem::path  pach_image=" "):path(pach_image){}
    };


    template<typename Engine>
    CePlane(Engine *win_,typename Engine::List * celist_,Data_initialization data){
        if(data.path.extension().string()==".svg"){
            object.emplace<CeObjectSVG>(win_,celist_,CeObjectSVG::Data_initialization(data.path,data.color,data.pos));
        }else {
            object.emplace<CePlate>(win_,celist_,CePlate::Data{data.path,data.type_mesh,data.color,data.pos});
        }
    }

    virtual void update_theme(Them_centre_::Them_object_base_base * theme){
        std::visit(tutl::overloaded{
            [&](CePlate & obj)     {obj.template change <Material_RI>().get_data()->color=theme->get_element_data<Enum_them::color_background>().normal;},
            [&](CeObjectSVG & obj) {}
        }, object);
    }

    virtual void update_position(const Position *view){
        std::visit(tutl::overloaded{
            [&](CePlate & obj)     {obj.template change <Position>().get_data()->set_view(view);},
            [&](CeObjectSVG & obj) {obj.change_position().position->set_view(view);}
        }, object);
    }

};
}// namespace CeEngine
#endif // CEPLANE_H
