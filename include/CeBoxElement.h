#ifndef CEBOXELEMENT_H
#define CEBOXELEMENT_H
#include "CePlate.h"
#include "CeText.h"

#include "utiliti_json.h"
namespace CeEngine {
class CeBoxElement: public CeObject{
    CePlate backraund;
    std::vector<std::shared_ptr<CeObject>> elements;
    //Scrollbar scrollbar;

    std::function<Position(int)> fun_pos;
public:
    struct Data_initialization{
        std::filesystem::path path_image="";
        std::function<Position(int)> fun_pos;

        template<typename Engine>
        Data_initialization(Engine * win,nlohmann::json &json,CePlagin_menedger * plugin_engine){
            get_to_check(json,"path_image",path_image);

            if(json["array_setting"].size()){
                glm::vec3 move_to={0.0f,0.0f,0.0f};
                glm::vec3 scale_to={0.0f,0.0f,0.0f};
                glm::vec3 move_start={0.0f,0.0f,0.0f};
                glm::vec3 scale_start={1.0f,1.0f,1.0f};
                int collum=-1;//-1 бесконечность

                std::cout<<json["array_setting"].size()<<std::endl;

                get_to_check(json["array_setting"],"x_offset",move_to.x);
                get_to_check(json["array_setting"],"y_offset",move_to.y);
                get_to_check(json["array_setting"],"z_offset",move_to.z);
                get_to_check(json["array_setting"],"x_start",move_start.x);
                get_to_check(json["array_setting"],"y_start",move_start.y);
                get_to_check(json["array_setting"],"z_start",move_start.z);
                get_to_check(json["array_setting"],"scale_x_offset",scale_to.x);
                get_to_check(json["array_setting"],"scale_y_offset",scale_to.y);
                get_to_check(json["array_setting"],"scale_z_offset",scale_to.z);
                get_to_check(json["array_setting"],"scale_x_start",scale_start.x);
                get_to_check(json["array_setting"],"scale_y_start",scale_start.y);
                get_to_check(json["array_setting"],"scale_z_start",scale_start.z);
                get_to_check(json["array_setting"],"collum_cout",collum);



                fun_pos = [=](int k){//пока так
                    int i=0;
                    int j=0;
                    k--;
                    if(collum!=-1){
                        i=k%collum;
                        j=k/ collum;
                    }else {
                        i=k;
                        j=k;
                    }


                    Position pos_t;
                    pos_t.move(move_start);
                    pos_t.move(glm::vec3{move_to.x*i,move_to.y*j,move_to.z});
                    pos_t.scale(scale_start);
                    pos_t.scale_add(glm::vec3{scale_to.x*i,scale_to.y*j,scale_to.z});

                    return pos_t;
                };
            }
        }
        Data_initialization(std::function<Position(int)> fun,std::filesystem::path path_image_=""): path_image(path_image_),fun_pos(fun){}
        operator CePlate::Data(){
            return CePlate::Data{path_image};
        }
    };

    template<typename Engine>
    CeBoxElement(Engine *win_,typename Engine::List * celist_,Data_initialization data):
        backraund(celist_,data),
        fun_pos(data.fun_pos){
        std::cout<<"CeBoxElement"<<std::endl;
    }
    //


    virtual void add_element(std::shared_ptr<CeObject> element){
        elements.emplace_back(element);
        if(fun_pos){
           Position pos=fun_pos(elements.size());

           elements.back()->change_position().position->new_move(pos.get_position_world());
           elements.back()->change_position().position->new_scale(pos.get_scale_object());
        }
        elements.back()->change_position().position->set_view(this->get_position());
    }

    template<typename F>
    void set_fun_pos(F f){
        fun_pos=f;
    }


    void installing_the_theme(){
        backraund.template change <Material_RI>().get_data()->color={1.0f, 1.0f, 1.0f, 1.0f};
    }

    virtual void update_position(const Position *view){
        backraund.template change <Position>().get_data()->set_view(view);
        for(auto &TT:elements){
            TT->change_position().position->set_view(view);
        }
    }

    virtual void update_theme(Them_centre_::Them_object_base_base * theme){
        backraund.template change <Material_RI>().get_data()->color=theme->get_element_data<Enum_them::color_background>().normal;
    }
};
}// namespace CeEngine
#endif // CEBOXELEMENT_H
