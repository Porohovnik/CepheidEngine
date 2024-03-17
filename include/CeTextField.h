#ifndef CETEXTFIELD_H
#define CETEXTFIELD_H
#include "ceobject.h"
#include "CeText.h"

#include "utiliti_json.h"
namespace CeEngine {
class CeTextField: public CeObject{
public:
    struct Data_initialization{
        CeText::Data data_text{""};

        std::function<void(CeText * )> fun_bind_dll;
        inline static std::function f{[](std::string t, CeText * ptr){
                if(ptr!=nullptr){
                    std::cout<<"CeTextField: "<<t.data()<<std::endl;
                    ptr->change_text(t.data());
                }
        }};

        operator CeText::Data(){ return data_text;}
        template<typename Engine>
        Data_initialization(Engine * win,nlohmann::json &json,CePlagin_menedger * plugin_engine):fun_bind_dll(fun_out<1>(plugin_engine,json,f)){
            json["font"].get_to(data_text.path_font);
            json["text"].get_to(data_text.text);
        }

        Data_initialization(std::filesystem::path path_font_,std::string text_buttom_,std::string  name_them_list_):data_text(path_font_,text_buttom_){}

    };
    CeText  text;

    template<typename Engine>
    CeTextField(Engine *win_,typename Engine::List * celist_,Data_initialization data):text(celist_,data){
        data.fun_bind_dll(&text);
    }

    void placement_of_elements(){
        auto t=text.template change <Position>();
        t.get_data()->scale(glm::vec3{1.0f, 0.7f, 1.0f});
        t.get_data()->move(glm::vec3{-0.45f,0.1f,-0.01f});
    }

    virtual void update_position(const Position * view){
        text.template change <Position>().get_data()->set_view(view);
    }

    void installing_the_theme(){
        text.template change <Material_T>().get_data()->color=0x141617FF;
    }

    virtual void update_theme(Them_centre_::Them_object_base_base * theme){
        text.template change <Material_T>().get_data()->color=theme->get_element_data<Enum_them::color_text>().normal;
    }
};
}// namespace CeEngine
#endif // CETEXTFIELD_H
