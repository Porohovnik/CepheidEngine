#ifndef CEBOTTOM_H
#define CEBOTTOM_H
#include "ceobject.h"
#include "CePlate.h"
#include "CeText.h"

#include "utiliti_json.h"

namespace CeEngine {
class CeButtom: public CeObject{
    std::function<void()> fun[4];
public:
    struct Data_initialization{
        CeText::Data data_text{""};
        std::filesystem::path path_image="";

        inline constexpr static std::size_t N_bottom=2;
        std::function<void()> fun_rep[N_bottom];

        enum  TYPE_PASS:int {FORGET=0,REMEMBER};
        TYPE_PASS type_pass=TYPE_PASS::FORGET;

        template<typename Engine>
        Data_initialization(Engine * win,nlohmann::json &json,CePlagin_menedger * plugin_engine){
            json["font"].get_to(data_text.path_font);
            json["text"].get_to(data_text.text);

            get_to_check(json,"path_image",path_image);

            std::vector<std::function<void(std::string id)>> funs[N_bottom];
            for(auto &[M,J]:json["funs_dll"].items()){
                std::string type="";
                get_to_check(J,"type",type);

                if(type=="launch"){
                    instal_array_fun_button(J,funs,get_data_dll<CePlagin_menedger,void,std::string>(plugin_engine,J));//тут чинит manager instal
                }
            }

            for(auto &[M,J]:json["funs_object_change"].items()){
                instal_array_fun_button(J,funs,fun_change_object(&win->objects,J));
            }

            for(auto &[M,J]:json["funs_utiliti_launch"].items()){
                instal_array_fun_button(J,funs,fun_screenshot(&win->ISFU,J,plugin_engine));
                instal_array_fun_button(J,funs,fun_change_list_status(&win->resource,J));
            }

            std::string id=get_id(json);
            for (std::size_t i=0;i<N_bottom;i++) {
                fun_rep[i]=[=](){
                    for(auto &T:funs[i]){
                        T(id);
                    };
                };
            }
        }
        Data_initialization(std::filesystem::path path_font_,std::string text_buttom_,std::string  name_them_list_):data_text(path_font_,text_buttom_){}
        operator CePlate::Data(){
            return CePlate::Data{path_image};
        }
        operator CeText::Data(){
            return data_text;
        }
    };

    bool pass=false;//пока так
    CePlate plate;
    CePlate plate_border;
    CeText  text;


    enum  TYPE_FUNCTION:int {IN_=0,REPEAT_LEFT,REPEAT_RIGHT,OUT_};

    template<typename Engine>
    CeButtom(Engine *win_,typename Engine::List * celist_,Data_initialization data):
        plate(celist_,data),plate_border(celist_,CePlate::Data{}),text(celist_,data){

        if(data.fun_rep[0]){
            this->connect_fun([this,data](){
                data.fun_rep[0]();
            },CeButtom::TYPE_FUNCTION::REPEAT_LEFT);
        }

        if(data.fun_rep[1]){
            this->connect_fun([this,data](){
                data.fun_rep[1]();
            },CeButtom::TYPE_FUNCTION::REPEAT_RIGHT);
        }

        //std::cout<<"CeBottom"<<std::endl;

        //по умолчанию, пользователи могут изменить

        auto fun_in_REMEMBER=[win_,this](){
            win_->set_cursor("STANDART_HAND");
            if(!this->pass){
                plate.template change <Material_RI>().get_data()->color=color_plate_change;
                plate_border.template change <Material_RI>().get_data()->color=color_border_change;
                text.template change <Material_T>().get_data()->color=color_text_change;
            }
        };
        auto fun_in_FORGET=[win_,this](){
            win_->set_cursor("STANDART_HAND");
            plate.template change <Material_RI>().get_data()->color=color_plate_change;
            plate_border.template change <Material_RI>().get_data()->color=color_border_change;
            text.template change <Material_T>().get_data()->color=color_text_change;
        };

        auto fun_out_REMEMBER= [win_,this](){
            win_->set_cursor("STANDART_ARROW");
            if(!this->pass){
                plate.template change <Material_RI>().get_data()->color=color_plate;
                plate_border.template change <Material_RI>().get_data()->color=color_border;
                text.template change <Material_T>().get_data()->color=color_text;
            }
        };
        auto fun_out_FORGET=[win_,this](){
            win_->set_cursor("STANDART_ARROW");
            plate.template change <Material_RI>().get_data()->color=color_plate;
            plate_border.template change <Material_RI>().get_data()->color=color_border;
            text.template change <Material_T>().get_data()->color=color_text;
        };

        switch (data.type_pass) {
            case Data_initialization::TYPE_PASS::REMEMBER :{
                this->connect_fun(fun_in_REMEMBER,CeButtom::TYPE_FUNCTION::IN_);
                this->connect_fun(fun_out_REMEMBER,CeButtom::TYPE_FUNCTION::OUT_);
            }break;
            case Data_initialization::TYPE_PASS::FORGET :{
                this->connect_fun(fun_in_FORGET,CeButtom::TYPE_FUNCTION::IN_);
                this->connect_fun(fun_out_FORGET,CeButtom::TYPE_FUNCTION::OUT_);
            }break;
        }
    }

    template<typename F>
    void connect_fun(F &&f,TYPE_FUNCTION type){
        fun[type]=std::forward<F>(f);
        if((fun[TYPE_FUNCTION::REPEAT_LEFT]!=nullptr && fun[TYPE_FUNCTION::IN_]!=nullptr &&  fun[TYPE_FUNCTION::OUT_]!=nullptr)){
            pass_program(pass);
        }
    }

    void pass_program(bool pass_){
        if(pass){
            pass=!pass;
            if(fun[TYPE_FUNCTION::IN_]!=nullptr){//возможны проблемы
                fun[TYPE_FUNCTION::IN_]();
            }
            pass=!pass;
            if(fun[TYPE_FUNCTION::REPEAT_LEFT]!=nullptr){//возможны проблемы
                fun[TYPE_FUNCTION::REPEAT_LEFT]();
            }

            if(fun[TYPE_FUNCTION::OUT_]!=nullptr){
                fun[TYPE_FUNCTION::OUT_]();
            }
        }
        pass=pass_;
    }

    const bool get_pass() const noexcept{
        return  pass;
    }

    void connect(Signal_engine<CeObject> * signal){
        //std::cout<<":__xx__:"<<plate.id_global<<std::endl;
        signal->connect(this,plate.id_global,-1);
        signal->connect(this,text.id_global,-1);
        signal->connect(this,plate_border.id_global,-1);
    }

    void placement_of_elements(){
        plate.template change <Position>().get_data()->scale(glm::vec4{1.0f-0.01*4, 1.0f-0.01*18, 1.0f, 1.0f});
        plate_border.template change <Position>().get_data()->scale(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});

        auto t=text.template change <Position>();
        t.get_data()->scale(glm::vec3{1.0f, 0.7f, 1.0f});
        t.get_data()->move(glm::vec3{-0.45f,0.1f,-0.1f});
    }


    virtual void operator()(Win_layer::Incoming_signals_from_user * ISFU,typename CeObject::Interaction_One_input_area){
        if(fun[TYPE_FUNCTION::IN_]!=nullptr){
            fun[TYPE_FUNCTION::IN_]();
        }
    }

    virtual void operator()(Win_layer::Incoming_signals_from_user * ISFU,typename CeObject::Exit_area){
        if(fun[TYPE_FUNCTION::OUT_]!=nullptr){
            fun[TYPE_FUNCTION::OUT_]();
        }
    }

    virtual void operator()(Win_layer::Incoming_signals_from_user * ISFU,typename CeObject::Interaction_Repetition_area){
        if(ISFU->keys[MOUSE_BUTTON_LEFT].action==1){
            ISFU->keys[MOUSE_BUTTON_LEFT].action=0;

            pass=!pass;
            if(fun[TYPE_FUNCTION::REPEAT_LEFT]!=nullptr){
                fun[TYPE_FUNCTION::REPEAT_LEFT]();
            }
        }
        if(ISFU->keys[MOUSE_BUTTON_RIGHT].action==1){
            ISFU->keys[MOUSE_BUTTON_RIGHT].action=0;

            if(fun[TYPE_FUNCTION::REPEAT_RIGHT]!=nullptr){
                fun[TYPE_FUNCTION::REPEAT_RIGHT]();
            }
        }
    }
    virtual void update_position(const Position * view){
        plate.template change <Position>().get_data()->set_view(view);
        plate_border.template change <Position>().get_data()->set_view(view);
        text.template change <Position>().get_data()->set_view(view);
    }


    Color color_border       ={1.0f,1.0f,1.0f,1.0f};
    Color color_text         ={1.0f,1.0f,1.0f,1.0f};
    Color color_plate        ={65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};

    Color color_border_change={65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};
    Color color_text_change  ={65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};
    Color color_plate_change =0XEEE6CBFF;


    void installing_the_theme(){
        plate_border.template change <Material_RI>().get_data()->color=color_border;
        text.        template change <Material_T>() .get_data()->color  =color_text;
        plate.       template change <Material_RI>().get_data()->color=color_plate;
    }

    virtual void update_theme(Them_centre_::Them_object_base_base * theme){

        color_border        =theme->get_element_data<Enum_them::color_border>().normal;
        color_text          =theme->get_element_data<Enum_them::color_text>().normal;
        color_plate         =theme->get_element_data<Enum_them::color_background>().normal;

        color_border_change =theme->get_element_data<Enum_them::color_border>().action_lift;
        color_text_change   =theme->get_element_data<Enum_them::color_text>().action_lift;
        color_plate_change  =theme->get_element_data<Enum_them::color_background>().action_lift;

        installing_the_theme();
    }
};
}// namespace CeEngine
#endif // CEBOTTOM_H
