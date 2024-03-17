#ifndef CEGRAPH_H
#define CEGRAPH_H
#include "ceobject.h"
#include "CeLine.h"
namespace CeEngine {
class CeGraph: public CeObject{
public:
    CeLine line;

    template<typename Engine>
    CeGraph(Engine *win_,typename Engine::List * celist_,CeLine::Data data):
        line(celist_,data){}

    void installing_the_theme(){
        line.template change <Material_L>().get_data()->color={65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};
    }
    void placement_of_elements(){

    }

    virtual void update_position(const Position *view){
        line.template change <Position>().get_data()->set_view(view);
    }

    virtual void update_theme(Them_centre_::Them_object_base_base * theme){
        line.template change <Material_L>().get_data()->color=theme->get_element_data<Enum_them::color_line>().normal;
    }
};
}// namespace CeEngine
#endif // CEGRAPH_H
