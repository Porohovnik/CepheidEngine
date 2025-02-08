#ifndef OBJECT_LIST_TEMPLATE_H
#define OBJECT_LIST_TEMPLATE_H
#include <ceobject.h>
#include <iostream>
#include <type_resource.h>
#include <interlayer_gl.h>

#include <charconv>
#include <universal_object.h>
#include "ceplagin_engine.h"
#include "utiliti_json.h"

template<typename Memory_,
         template<typename K, uint  L> class Buffer_,
         auto name_type,
         typename Data_p,
         typename Material,
         template<typename Memory,template<typename K, unsigned int  L> class Buffer,typename Data> class Base_controller,
         Data_p(* f)(std::any)>//обощить для разных способов
class CePrimitiv: protected Identification{
public:
    inline static std::string name_primitive=name_type;
    inline static std::function<Data_p(std::any)> fun_data=f;

    std::size_t id_global;
    Base_controller<Memory_,Buffer_,Data_p> * controller;
    std::function<void()> deleter;

    template<typename List>
    CePrimitiv(List * celist,Data_p data):
        controller(celist->template get_element<Base_controller<Memory_,Buffer_,Data_p>>()){

        this->id=       controller->add_data(data);
        //обновление, с учётом листа
        auto it= celist->template attach_object<Base_controller<Memory_,Buffer_,Data_p>>(this);//вынести
        controller->template new_status(celist->get_status(),this);

        deleter=[celist,it](){
            celist->template reattach_object<Base_controller<Memory_,Buffer_,Data_p>>(it);
        };


        id_global=controller->template get_element<Material>(this->id)->id;//придумать что-то более изящное
        std::cout<<name_primitive<<std::endl;
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
        //std::cout<<"l "<<id<<"|"<<(celist!=nullptr)<<std::endl;

//        if(celist!=nullptr){
//            celist->template reattach_id<Base_controller>(this);
//        }

        //std::cout<<"-1l"<<std::endl;
        //std::cout<<"-2l"<<std::endl;

        if(controller!=nullptr){
           deleter();
           controller->delete_element(this->id);
        }
        //std::cout<<"-0l"<<std::endl;
    }
};



Data_CePlate F_plate( std::any data_){
    auto data=std::any_cast<std::map<std::string,Sheme_data>>(data_);
    return Data_CePlate(data["pach_image"].data);
};

constexpr char name_CePlate[]="CePlate";
using CePlate=CePrimitiv<Storage_res,Buffer_GL,name_CePlate,Data_CePlate,Material_RI,Controller_Res_2d_form,F_plate>;

constexpr char name_Line[]="CeLine";
using CeLine=CePrimitiv<Storage_res,Buffer_GL,name_Line,Data_CeLine,Material_L,Controller_Res_2d_line,nullptr>;


Data_CeText F_Text(std::any  data_){
    auto data=std::any_cast<std::map<std::string,Sheme_data>>(data_);
    if(data["font"].data==""){
        std::cout<<"Problema, Font not set";
        assert(true);
    }

    return Data_CeText(data["font"].data,data["text"].data);
};


constexpr char name_CeText[]="CeText";
class CeText: public CePrimitiv<Storage_res,Buffer_GL,name_CeText,Data_CeText,Material_T,Controller_Res_2d_text,F_Text>{
public:
    template<typename List>
    CeText(List * celist_,Data_CeText data):
        CePrimitiv<Storage_res,Buffer_GL,name_CeText,Data_CeText,Material_T,Controller_Res_2d_text,F_Text>(celist_,data){}

    inline void change_text(char * ch){
        this->controller->template change_text<TYPE_TEXT::UNICOD>(this->id,ch);
    }
    inline void change_text_ansi(char * ch,int start,int size){
        this->controller->template change_text<TYPE_TEXT::ANSI>(this->id,ch,start,size);
    }
};



//////////////
///
///
///
///
///
/////////////



class CeTextField: public CeObject{
public:
    struct Data_inzilization{
        Data_CeText data_text{""};

        std::function<void(CeText * )> fun_bind_dll;
        inline static std::function f{[](std::string t, CeText * ptr){
                if(ptr!=nullptr){
                    std::cout<<"CeTextField: "<<t.data()<<std::endl;
                    ptr->change_text(t.data());
                }
        }};
        template<typename Engine>
        Data_inzilization(Engine * win,nlohmann::json &json,CePlagin_menedger * plagin_engine):fun_bind_dll(fun_out<1>(plagin_engine,json,f)){
            json["font"].get_to(data_text.path_font);
            json["text"].get_to(data_text.text);
        }

        Data_inzilization(std::filesystem::path path_font_,std::string text_buttom_,std::string  name_them_list_):data_text(path_font_,text_buttom_){}
        operator Data_CeText(){ return data_text;}
    };
    CeText  text;

    template<typename Engine>
    CeTextField(Engine *win_,typename Engine::List * celist_,Data_inzilization data):text(celist_,data){
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

    virtual void update_them(Them_centre_::Them_object_base_base * them){
        text.template change <Material_T>().get_data()->color=them->get_element_data<Enum_them::color_text>().normal;
    }
};

template<GLenum type_sheme_pixel,GLenum type_byte>
inline static void create_screenshot(std::size_t x,std::size_t size_x,std::size_t y,std::size_t size_y,std::filesystem::path path);


class CeBottom: public CeObject{
    std::function<void()> fun[4];
public:
    struct Data_inzilization{
        Data_CeText data_text{""};
        std::filesystem::path path_image="";

        inline constexpr static std::size_t N_bottom=2;
        std::function<void()> fun_rep[N_bottom];

        enum  TYPE_PASS:int {FORGET=0,REMEMBER};
        TYPE_PASS type_pass=TYPE_PASS::FORGET;

        template<typename Engine>
        Data_inzilization(Engine * win,nlohmann::json &json,CePlagin_menedger * plagin_engine){
            json["font"].get_to(data_text.path_font);
            json["text"].get_to(data_text.text);

            get_to_check(json,"path_image",path_image);

            std::vector<std::function<void(std::string id)>> funs[N_bottom];
            for(auto &[M,J]:json["funs_dll"].items()){
                std::string type="";
                get_to_check(J,"type",type);

                if(type=="launch"){
                    instal_array_fun_button(J,funs,get_data_dll<CePlagin_menedger,void,std::string>(plagin_engine,J));
                }
            }

            for(auto &[M,J]:json["funs_object_change"].items()){
                instal_array_fun_button(J,funs,fun_change_object(&win->objects,J));
            }

            for(auto &[M,J]:json["funs_utiliti_launch"].items()){
                instal_array_fun_button(J,funs,fun_screenshot(&win->ISFU,J,plagin_engine));
                instal_array_fun_button(J,funs,fun_change_list_status(&win->resource,J));
            }

            for (std::size_t i=0;i<N_bottom;i++) {
                fun_rep[i]=std::bind([=](std::string id){
                    for(auto &T:funs[i]){
                        T(id);
                    };
                },get_id(json));
            }
        }
        Data_inzilization(std::filesystem::path path_font_,std::string text_buttom_,std::string  name_them_list_):data_text(path_font_,text_buttom_){}
        operator Data_CePlate(){
            return Data_CePlate{path_image};
        }
        operator Data_CeText(){
            return data_text;
        }
    };

    bool pass=false;//пока так
    CePlate plate;
    CePlate plate_border;
    CeText  text;


    enum  TYPE_FUNCTION:int {IN_=0,REPEATE_LIFT,REPEATE_RIGHT,OUT_};

    template<typename Engine>
    CeBottom(Engine *win_,typename Engine::List * celist_,Data_inzilization data):
        plate(celist_,data),plate_border(celist_,Data_CePlate{}),text(celist_,data){

        if(data.fun_rep[0]){
            this->connect_fun([this,data](){
                data.fun_rep[0]();
            },CeBottom::TYPE_FUNCTION::REPEATE_LIFT);
        }

        if(data.fun_rep[1]){
            this->connect_fun([this,data](){
                data.fun_rep[1]();
            },CeBottom::TYPE_FUNCTION::REPEATE_RIGHT);
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
            case Data_inzilization::TYPE_PASS::REMEMBER :{
                this->connect_fun(fun_in_REMEMBER,CeBottom::TYPE_FUNCTION::IN_);
                this->connect_fun(fun_out_REMEMBER,CeBottom::TYPE_FUNCTION::OUT_);
            }break;
            case Data_inzilization::TYPE_PASS::FORGET :{
                this->connect_fun(fun_in_FORGET,CeBottom::TYPE_FUNCTION::IN_);
                this->connect_fun(fun_out_FORGET,CeBottom::TYPE_FUNCTION::OUT_);
            }break;
        }
    }

    template<typename F>
    void connect_fun(F &&f,TYPE_FUNCTION type){
        fun[type]=std::forward<F>(f);
        if((fun[TYPE_FUNCTION::REPEATE_LIFT]!=nullptr && fun[TYPE_FUNCTION::IN_]!=nullptr &&  fun[TYPE_FUNCTION::OUT_]!=nullptr)){
            pass_programm(pass);
        }
    }

    void pass_programm(bool pass_){
        if(pass){
            pass=!pass;
            if(fun[TYPE_FUNCTION::IN_]!=nullptr){//возможны проблемы
                fun[TYPE_FUNCTION::IN_]();
            }
            pass=!pass;
            if(fun[TYPE_FUNCTION::REPEATE_LIFT]!=nullptr){//возможны проблемы
                fun[TYPE_FUNCTION::REPEATE_LIFT]();
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


    virtual void operator()(Incoming_signals_from_user * ISFU,typename CeObject::Interaction_One_input_area){
        if(fun[TYPE_FUNCTION::IN_]!=nullptr){
            fun[TYPE_FUNCTION::IN_]();
        }
    }

    virtual void operator()(Incoming_signals_from_user * ISFU,typename CeObject::Exit_area){
        if(fun[TYPE_FUNCTION::OUT_]!=nullptr){
            fun[TYPE_FUNCTION::OUT_]();
        }
    }

    virtual void operator()(Incoming_signals_from_user * ISFU,typename CeObject::Interaction_Repetition_area){
        if(ISFU->keys[GLFW_MOUSE_BUTTON_LEFT].action==1){
            ISFU->keys[GLFW_MOUSE_BUTTON_LEFT].action=0;

            pass=!pass;
            if(fun[TYPE_FUNCTION::REPEATE_LIFT]!=nullptr){
                fun[TYPE_FUNCTION::REPEATE_LIFT]();
            }
        }
        if(ISFU->keys[GLFW_MOUSE_BUTTON_RIGHT].action==1){
            ISFU->keys[GLFW_MOUSE_BUTTON_RIGHT].action=0;

            if(fun[TYPE_FUNCTION::REPEATE_RIGHT]!=nullptr){
                fun[TYPE_FUNCTION::REPEATE_RIGHT]();
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

    virtual void update_them(Them_centre_::Them_object_base_base * them){

        color_border        =them->get_element_data<Enum_them::color_border>().normal;
        color_text          =them->get_element_data<Enum_them::color_text>().normal;
        color_plate         =them->get_element_data<Enum_them::color_background>().normal;

        color_border_change =them->get_element_data<Enum_them::color_border>().action_lift;
        color_text_change   =them->get_element_data<Enum_them::color_text>().action_lift;
        color_plate_change  =them->get_element_data<Enum_them::color_background>().action_lift;

        installing_the_theme();
    }

};


struct Setting_input{
    enum  TYPE_INPUT:int {NAMBER=0,ANSI,UNICOD};

    TYPE_INPUT type=NAMBER;
    std::size_t cout_charachers_max=10;
    bool clear_old_value_to_new_input=true;
};


class CeInputText: public CeObject{
    std::shared_ptr<CeBottom> field;
    std::string id="";
    std::string value="";
    Setting_input setting;
    std::function<std::string(std::string)> formative_fun;
    std::function<std::string(std::string)> limit_fun;


    inline static std::array<char,50> whitespace=tutl::Tuple_to_array<char>(tutl::create_tuple<50,char>(' '));
    inline static CeInputText * current_active=nullptr;

    std::function<void(CeInputText *)> fun_connect=nullptr;
    std::function<void(CeInputText *)> fun_disconnect=nullptr;
public:
    std::function<void(std::string)> fun_in_data;
    std::function<void()> fun_active;
    std::function<void()> fun_accept;


    void set_value(std::string value_){
        value=value_;
        if(limit_fun!=nullptr){
            value=limit_fun(value);
        }

        switch (setting.type) {
            case Setting_input::TYPE_INPUT::NAMBER :
            case Setting_input::TYPE_INPUT::ANSI :{
                std::cout<<"OUT: "<<value<<std::endl;
                if(value.size()<setting.cout_charachers_max){
//                    field->text.change_text_ansi(value.data(),0,value.size());
//                    //заполнение пустотой
//                    field->text.change_text_ansi(whitespace.data(),value.size(),setting.cout_charachers_max-value.size());
                    field->text.change_text(value.data());
                }
            };
            break;
            case Setting_input::TYPE_INPUT::UNICOD :{
                field->text.change_text(value.data());
            };
            break;
        }

        std::cout<<"---set_value---"<<std::endl;
        accept();
    }


    struct Data_inzilization{
        typename CeBottom::Data_inzilization data_bottom;
        std::string id="";

        std::function<void(CeInputText * )> fun_bind_dll;
        inline static std::function f{[](std::string t, CeInputText * ptr){
            if(ptr!=nullptr){
                ptr->set_value(t.data());
            }
        }};

        //фильтр
        std::function<std::string(std::string)> formative_fun;
        std::function<void(std::string)> fun_rep=nullptr;


        std::function<void()> funs_active;
        std::function<void()> funs_accept;

        template<typename Engine>
        Data_inzilization(Engine * win,nlohmann::json &json,CePlagin_menedger * plagin_engine):data_bottom(win,json,plagin_engine),fun_bind_dll(fun_out<1>(plagin_engine,json,f)){
            std::string value="";
            get_to_check(json,"value",value);

            id=get_id(json);

            std::ifstream json_file("save_imput.json");
            if(json_file.is_open()){
                nlohmann::json json;
                json_file >> json;
                get_to_check(json,id,value);
            }

            if(value.size()){
               data_bottom.data_text.text =value;
            }


            std::vector<std::function<void()>> funs_actives;
            std::vector<std::function<void()>> funs_accepts;

            for(auto &[M,J]:json["funs_dll"].items()){
                std::string type_action="";
                get_to_check(J,"type",type_action);

                auto fun=get_data_dll<CePlagin_menedger,void,std::string>(plagin_engine,J);
                if(fun!=nullptr){
                    if(type_action=="active"){
                        funs_actives.push_back(To_bind_in<0>(fun,id));
                    }
                    if(type_action=="accept"){
                        funs_accepts.push_back(To_bind_in<0>(fun,id));
                    }
                }

            }

            for(auto &[M,J]:json["funs_object_change"].items()){
                std::string type_action="active";
                get_to_check(J,"type_action",type_action);

                auto fun=fun_change_object(&win->objects, J);
                if(fun!=nullptr){
                    if(type_action=="active"){
                        funs_actives.push_back(To_bind_in<0>(fun,id));
                    }
                    if(type_action=="accept"){
                        funs_accepts.push_back(To_bind_in<0>(fun,id));
                    }
                }
            }
            if(funs_actives.size()){
                funs_active=[=](){
                    for(auto &T:funs_actives){
                        T();
                    }
                };
            }
            if(funs_accepts.size()){
                funs_accept=[=](){
                    for(auto &T:funs_accepts){
                        T();
                    }
                };
            }


            if(json["namber_setting"].size()){
                float min=0.0f;
                float max=0.0f;
                int accurate=1;
                //фильтр
                bool min_b=get_to_check(json["namber_setting"],"min",min);
                bool max_b=get_to_check(json["namber_setting"],"max",max);
                bool accurate_b=get_to_check(json["namber_setting"],"accurate",accurate);

                if(min_b || max_b || accurate_b){
                    formative_fun=[=](std::string value){
                        std::string s=value;
                        if(!s.size()){
                            s=std::to_string(min);
                        }

                        float t=std::stof(s);

                        if(min_b && t<min){
                            t=min;
                        }
                        if(max_b && t>max){
                            t=max;
                        }

                        s=std::to_string(t);

                        s.erase(s.size()-6+accurate,s.size()-1);
                        if(accurate==0){
                            s.erase(s.find_first_of(','));
                        }
                        std::cout<<value<<"|"<<t<<"|"<<s<<std::endl;
                        return s;
                    };
                }
            }

            auto t=fun_in<CePlagin_menedger,std::string,std::string>(plagin_engine,json);
            if(t){
                fun_rep =To_bind_in<1>(t,id);
            }

        }
        Data_inzilization(std::filesystem::path path_font_,std::string value,std::string  name_them_list_):
            data_bottom(path_font_,value,name_them_list_){}

        operator typename CeBottom::Data_inzilization(){
            return data_bottom;
        }
    };



    template<typename Engine>
    CeInputText(Engine *win_,typename Engine::List * celist_,Data_inzilization data):
        fun_in_data(data.fun_rep),
        value(data.data_bottom.data_text.text),
        id(data.id),
        formative_fun(data.formative_fun),
        fun_active(data.funs_active),
        fun_accept(data.funs_accept){

        field=CeObject::template create_object_no_base<CeBottom>(win_,celist_,data);

        if(fun_in_data){
            fun_in_data(value);
        }

        data.fun_bind_dll(this);

        field->connect_fun([this](){

            if(current_active!=nullptr){
                std::cout<<"---button---"<<std::endl;
                current_active->accept();
            }
            if(fun_active){//акуратно
                std::cout<<"|||||||||||||"<<std::endl;
                fun_active();
            }

            if(setting.clear_old_value_to_new_input){
                value.clear();
                field->text.change_text_ansi(whitespace.data(),value.size(),setting.cout_charachers_max-value.size());
            }

            fun_connect(this);
            current_active=this;

        },CeBottom::TYPE_FUNCTION::REPEATE_LIFT);
    }

    void connect(Signal_engine<CeObject> * signal){
        fun_connect=    [signal](CeInputText * obj){
            signal->connect    (obj,                             typename CeObject::Input_signal{});
            signal->connect    (obj,          GLFW_KEY_BACKSPACE,typename CeObject::Key_signal{});
            signal->connect    (obj,          GLFW_KEY_ENTER    ,typename CeObject::Key_signal{});
        };

        fun_disconnect =[signal](CeInputText * obj){
            signal->dis_connect(obj,                   typename CeObject::Input_signal{});
            signal->dis_connect(obj,GLFW_KEY_BACKSPACE,typename CeObject::Key_signal{});
            signal->dis_connect(obj,GLFW_KEY_ENTER    ,typename CeObject::Key_signal{});
        };
    }

    ~CeInputText(){
        nlohmann::json json;
        std::ifstream json_file_r;
        json_file_r.open("save_imput.json");

        if(json_file_r.is_open()){
            json_file_r >> json;
        }
        json_file_r.close();

        std::ofstream json_file_w("save_imput.json");

        json[id]=value;
        json_file_w << std::setw(4) << json << std::endl;
    }


    void accept(){
        std::cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<std::endl;
        if(current_active!=nullptr){
            fun_disconnect(current_active);
        }

        if(formative_fun!=nullptr){
            value=formative_fun(value);
            field->text.change_text(value.data());
        }
        if(fun_accept){
            std::cout<<"##############"<<std::endl;
            fun_accept();
        }
        if(fun_in_data){
            fun_in_data(value);
        }
    }


    virtual void operator()(Incoming_signals_from_user * ISFU, typename CeObject::Key_signal){
        if(ISFU->keys[GLFW_KEY_BACKSPACE].action==1 && value.size()>0){
            switch (setting.type) {
                case Setting_input::TYPE_INPUT::NAMBER :
                case Setting_input::TYPE_INPUT::ANSI :{
                    if(value.size()>0){
                        if(value.back()>256){
                           value.pop_back();
                        }
                        value.pop_back();
                    }

                    if(limit_fun!=nullptr){
                        value=limit_fun(value);
                    }
//                    field->text.change_text_ansi(value.data(),0,value.size());
//                    //заполнение пустотой
//                    field->text.change_text_ansi(whitespace.data(),value.size(),setting.cout_charachers_max-value.size());
                    field->text.change_text(value.data());
                };
                break;
                case Setting_input::TYPE_INPUT::UNICOD :{

                };
                break;
            }
        }

        if(ISFU->keys[GLFW_KEY_ENTER].action==1){
            accept();
        }
    }

    virtual void operator()(Incoming_signals_from_user * ISFU,typename CeObject::Input_signal){
        switch (setting.type) {
            case Setting_input::TYPE_INPUT::NAMBER :
            case Setting_input::TYPE_INPUT::ANSI :{
                if(ISFU->input_code_text<256 && value.size()<setting.cout_charachers_max){
                    value+=static_cast<char>(ISFU->input_code_text);
                    if(limit_fun!=nullptr){
                        value=limit_fun(value);
                    }
//                    field->text.change_text_ansi(value.data(),0,value.size());
//                    //заполнение пустотой
//                    field->text.change_text_ansi(whitespace.data(),value.size(),setting.cout_charachers_max-value.size());

                    field->text.change_text(value.data());

                }
            };
            break;
            case Setting_input::TYPE_INPUT::UNICOD :{
                if(!(ISFU->input_code_text<256)){
                    //                    char out[2];
                    //                    wchar_t *in=new wchar_t(static_cast<wchar_t>(codepoint));
                    //                    std::wcstombs(out,in,2);
                    //                    value+=out;
                    //                    if(formative_fun!=nullptr){
                    //                        value=formative_fun(value);
                    //                    }
                    //                    field.text.change_text(value.data());
                }
            };
            break;
        }

    }
    
    

    void installing_the_theme(){
        Them_centre_::Them_object_base_base them;
        
        them.get_element<Enum_them::color_border>()    ->data.emplace(Color{0.0f, 0.0f, 0.0f, 1.0f});
        them.get_element<Enum_them::color_text>()      ->data.emplace(Color{65*1.0f/256.0f, 74*1.0f/256.0f, 76*1.0f/256.0f, 1.0f});
        them.get_element<Enum_them::color_background>()->data.emplace(Color{1.0f,1.0f,1.0f,1.0f});
            
        them.get_element<Enum_them::color_border>()    ->data->action_lift={0.3f, 0.3f, 0.3f, 1.0f};
        them.get_element<Enum_them::color_text>()      ->data->action_lift={65*1.0f/256.0f, 74*1.0f/256.0f, 76*1.0f/256.0f, 1.0f};
        them.get_element<Enum_them::color_background>()->data->action_lift={1.0f,1.0f,1.0f,1.0f};

        update_them(&them);
    }
    
    
    virtual void update_position(const Position * view){
        field->change_position().position->set_view(view);
    }
    virtual void update_them(Them_centre_::Them_object_base_base * them){//хмм, а если пихать сюда массив?)
        field->update_them(them);
    }
};
//////////////
///
///
///
///
///
/////////////


class CeScrollbar: public CeObject{
    Position pos_elements=glm::vec3{0.0f,0.0f,0.0f};

    CePlate Scrollbar;
    CeBottom Scrollbar_joustick;
    CeBottom Scrollbar_plus;
    CeBottom Scrollbar_minus;
    float pos=0.0f;//relevation size page,max=1

public:
    template<typename Engine>
    CeScrollbar(Engine *win_,typename Engine::List * celist_,typename Engine::Data data){
        std::cout<<"CeScrollbar"<<std::endl;

    };
    //

    void connect(Signal_engine<CeObject> * signal){
        signal->connect(this,Scrollbar.id_global,-1);
    }

    virtual void operator()(typename CeObject::Infiniti){

    }

    virtual void operator()(int id,typename CeObject::Interaction_Repetition_area){

    }
};


class CeBoxElement: public CeObject{
    CePlate backraund;
    std::vector<std::shared_ptr<CeObject>> elements;
    //Scrollbar scrollbar;

    std::function<Position(int)> fun_pos;
public:
    struct Data_inzilization{
        std::filesystem::path path_image="";
        std::function<Position(int)> fun_pos;

        template<typename Engine>
        Data_inzilization(Engine * win,nlohmann::json &json,CePlagin_menedger * plagin_engine){
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
        Data_inzilization(std::function<Position(int)> fun,std::filesystem::path path_image_=""): path_image(path_image_),fun_pos(fun){}
        operator Data_CePlate(){
            return Data_CePlate{path_image};
        }
    };

    template<typename Engine>
    CeBoxElement(Engine *win_,typename Engine::List * celist_,Data_inzilization data):
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
        backraund.template change <Material_RI>().get_data()->color=glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
    }

    virtual void update_position(const Position *view){
        backraund.template change <Position>().get_data()->set_view(view);
        for(auto &TT:elements){
            TT->change_position().position->set_view(view);
        }
    }

    virtual void update_them(Them_centre_::Them_object_base_base * them){
        backraund.template change <Material_RI>().get_data()->color=them->get_element_data<Enum_them::color_background>().normal;
    }

};
//////////////
///
///
///
///
///
/////////////

glm::vec2 Sphere_to_Mercator(glm::vec2 lon_lang){
    const float a=6370997;
    const float b=6356752.314;
    float f=(a-b)/b;
    float e1=std::sqrt(2*f-f*f);
    float e2=std::sqrt(1-b/a*b/a);




    auto lon_r_lang_r=glm::abs(glm::radians(lon_lang));

    float x=lon_r_lang_r.y*a;
    float t=-1+glm::tan(static_cast<float>(3.14/4.0+lon_r_lang_r.x/2.0)*std::pow((1-e1*std::sin(lon_r_lang_r.x))/(1+1-e1*std::sin(lon_r_lang_r.x)),e1/2.0f));
    float y=a*std::log1p(t);

    if(lon_lang.y<0){
        x=-x;
    }
    if(lon_lang.x<0){
        y=-y;
    }

    //std::cout<<lon_lang.x<<"|"<<lon_lang.y<<" xy: "<<x<<"|"<<y<<std::endl;
    return {x,y};
}
inline static glm::vec2 max=Sphere_to_Mercator(glm::vec2{82.5,180 });
inline static glm::vec2 max_2=Sphere_to_Mercator(glm::vec2{76,180 });

glm::vec2 Sphere_to_Mercator_normalizate(glm::vec2 lon_lang){
    if(lon_lang.x>0){
        return Sphere_to_Mercator(lon_lang)/max;
    }else{
        return Sphere_to_Mercator(lon_lang)/max_2;
    }
}

#include <list>


class CeGraph: public CeObject{
public:
    CeLine line;

    template<typename Engine>
    CeGraph(Engine *win_,typename Engine::List * celist_,typename CeObject::Data data):
        line(celist_,std::any_cast<Data_CeLine>(data)){}

    void installing_the_theme(){
        line.template change <Material_L>().get_data()->color=glm::vec4{65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};
    }
    void placement_of_elements(){

    }

    virtual void update_position(const Position *view){
        line.template change <Position>().get_data()->set_view(view);
    }

    virtual void update_them(Them_centre_::Them_object_base_base * them){
        line.template change <Material_L>().get_data()->color=them->get_element_data<Enum_them::color_line>().normal;
    }
};



struct Setting_axis{
    enum TYPE_POSITON_AXIS:int{VERTICAL=0, HORIZONTAL};\

    std::string name_axis;
    float max=0;
    float min=0;
    int type=HORIZONTAL;
    int cout_divisions=0;
    int accurate=2;
    std::filesystem::path path_font;

    std::vector<std::pair<std::string,float>> dimensions;
};


class CeAxis: public CeObject{
    Setting_axis setting;

    CeText name_axis;
    CeLine line;
    std::vector<std::shared_ptr<CeLine>> divisions_line;
   // std::list<CeLine<Engine>> grid;
    std::vector<std::shared_ptr<CeText>> name_divisions;

    float max=0;
    float min=0;

    float max_const=0;
    float min_const=0;

    float max_d=0;
    float min_d=0;

public:
    struct Graf_contoroller{
        float max;
        float min;
        std::shared_ptr<CeGraph> graf;
    };
private:
    std::unordered_map<std::string,Graf_contoroller> graf_menedgment;

public:
    struct Data_inzilization{
        Setting_axis setting;
        template<typename Engine>
        Data_inzilization(Engine * win,nlohmann::json &json,CePlagin_menedger * plagin_engine){}

        Data_inzilization(Setting_axis setting_):
            setting(setting_){}
    };

    template<typename Engine>
    CeAxis(Engine *win_,typename Engine::List * celist_,Data_inzilization data):
        setting(data.setting),
        name_axis(celist_,Data_CeText(setting.path_font,setting.name_axis)),
        line(celist_,Data_CeLine(line_mesh().get_data(),"line")){

        std::cout<<setting.name_axis<<" CeAxis: "<<setting.path_font<<std::endl;
        for (std::size_t i=0;i<setting.cout_divisions;i++) {
           divisions_line.emplace_back(std::make_shared<CeLine>(celist_,Data_CeLine(line_mesh().get_data(),"line")));
           // grid.          emplace_back(win_,celist_,Data_CeLine(line_mesh()));
           name_divisions.emplace_back(std::make_shared<CeText>(celist_,Data_CeText(setting.path_font,"1.0")));
        }
    }

    void change_divisions_text(float max,float min){
        float sum=max-min;

        std::size_t i=0;
        for (auto &T:name_divisions) {
            auto s=std::to_string(min+i*sum/static_cast<float>(setting.cout_divisions-1));
            s.erase(s.size()-6+setting.accurate,s.size()-1);
            if(setting.accurate==0){
                s.erase(s.find_first_of(','));
            }
            //std::cout<<i<<"|:::::::::::::"<<s<<std::endl;
            T->change_text(s.data());
            i++;
        }
    }

    void add_graf(std::string name,Graf_contoroller graf_contrl){
        graf_menedgment.emplace(name,graf_contrl);
        change();
    }

    void delete_graf(std::string name){
        graf_menedgment.erase(name);
        if(graf_menedgment.size()){
            change();
        }
    }

    void change(){

        max=std::max_element(graf_menedgment.begin(),graf_menedgment.end(),[](const auto &a,const auto &b){return std::abs(a.second.max)<std::abs(b.second.max);})->second.max;
        min=std::min_element(graf_menedgment.begin(),graf_menedgment.end(),[](const auto &a,const auto &b){return std::abs(a.second.min)<std::abs(b.second.min);})->second.min;


        if(static_cast<int>(min_const)!=static_cast<int>(max_const)){

            max=min_const;
            min=max_const;

        }
        if(static_cast<int>(min_d)!=static_cast<int>(max_d)){
            max=max_d;
            min=min_d;

        }

        change_divisions_text(max,min);
        std::cout<<"max:"<<max<<"|"<<"min:"<<min<<std::endl;


        for(auto &[s,t]:graf_menedgment){
            auto move_old= t.graf->change_position().position->get_position_world();
            auto scale_old=t.graf->change_position().position->get_scale_object();
            if(setting.type==Setting_axis::VERTICAL){

                t.graf->change_position().position->new_move(glm::vec3{move_old.x,-0.5f,-0.01f});
                t.graf->change_position().position   ->new_scale(glm::vec3{scale_old.x,1.0f,1.0f});

                t.graf->change_position().position->move(glm::vec3{0.0f,-min/(max-min),0.0f});
                t.graf->change_position().position->scale(glm::vec3{1.0f,1.0f/(max-min),1.0f});
            }
            if(setting.type==Setting_axis::HORIZONTAL){
                t.graf->change_position().position->new_move(glm::vec3{-0.5f,move_old.y,-0.01f});
                t.graf->change_position().position->new_scale(glm::vec3{1.0f,scale_old.y,1.0f});

                t.graf->change_position().position->move(glm::vec3{-min/(max-min),0.0f,0.0f});
                t.graf->change_position().position->scale(glm::vec3{1.0f/(max-min),1.0f,1.0f});
            }

        }
    }

    void placement_of_elements(){
        float t=1.0f/static_cast<float>(setting.cout_divisions-1) ;
        line.template change <Position>().get_data()->scale(glm::vec3{1.0f,1.0f,1.0f});
        line.template change <Position>().get_data()->new_move(glm::vec3{0.0f,0.0f,-0.01f});

        name_axis.template change <Position>().get_data()->scale(glm::vec3{0.07f,0.07f,1.0f});

        if(setting.type==Setting_axis::VERTICAL){
            name_axis.template change <Position>().get_data()->rotete(glm::vec3(1.0f, 1.0f, 0.0f),-180.0f);
            name_axis.template change <Position>().get_data()->new_move(glm::vec3{1.08f,-0.12f,-0.01f});
        }else {
            name_axis.template change <Position>().get_data()->new_move(glm::vec3{0.5f,-0.12f,-0.01f});
        }

        std::size_t i=0;
        for (auto &T:divisions_line) {
             T->template change <Position>().get_data()->scale(glm::vec3{0.05f,0.05f,1.0f});
             T->template change <Position>().get_data()->rotete(glm::vec3(1.0f, 1.0f, 0.0f),180.0f);
             T->template change <Position>().get_data()->new_move(glm::vec3{0.00f+t*i,-0.025f,-0.01f});
             i++;
        }
//        i=0;
//        for (auto &T:grid) {
//             T.template change <Position>().get_data()->scale(glm::vec3{2.0f,2.0f,1.0f});
//             T.template change <Position>().get_data()->rotete(glm::vec3(1.0f, 1.0f, 0.0f),180.0f);
//             T.template change <Position>().get_data()->new_move(glm::vec3{-1.0f+t*i,-0.1f,0.0f});
//             i++;
//        }
        i=0;
        for (auto &T:name_divisions) {
            T->template change <Position>().get_data()->scale(glm::vec3{0.06f,0.06f,1.0f});
            if(setting.type==Setting_axis::VERTICAL){
                T->template change <Position>().get_data()->rotete(glm::vec3(1.0f, 1.0f, 0.0f),-180.0f);
                T->template change <Position>().get_data()->new_move(glm::vec3{0.00f+t*i,-0.08f,-0.01f});
            }else {
                T->template change <Position>().get_data()->new_move(glm::vec3{-0.01f+t*i,-0.05f,-0.01f});
            }

            i++;
        }
    }


    void installing_the_theme(){
        line.template change <Material_L>().get_data()->color=glm::vec4{65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};

        name_axis.template change <Material_T>().get_data()->color=glm::vec4{65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};

        for (auto &T:divisions_line) {
             T->template change <Material_L>().get_data()->color=glm::vec4{65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};
        }

        for (auto &T:name_divisions) {
             T->template change <Material_T>().get_data()->color=glm::vec4{65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};
        }


    }

    virtual void update_them(Them_centre_::Them_object_base_base * them){//а вот тут нужен будет массив
        line.template change <Material_L>().get_data()->color=them->get_element_data<Enum_them::color_line>().normal;


        name_axis.template change <Material_T>().get_data()->color=them->get_element_data<Enum_them::color_text>().normal;

        for (auto &T:divisions_line) {
             T->template change <Material_L>().get_data()->color=them->get_element_data<Enum_them::color_line>().normal;
        }

        for (auto &T:name_divisions) {
             T->template change <Material_T>().get_data()->color=them->get_element_data<Enum_them::color_text>().normal;
        }
    }



    virtual void update_position(const Position *view){
        name_axis.template change <Position>().get_data()->set_view(view);
        line.template change <Position>().get_data()->set_view(view);
        for(auto TT:divisions_line){
            TT->template change <Position>().get_data()->set_view(view);
        }
//        for(auto TT:grid){
//            TT->change_position().position->set_view(view);
//        }
        for(auto TT:name_divisions){
            TT->template change <Position>().get_data()->set_view(view);
        }
    }

};



class CeGraphics: public CeObject{
    CePlate backgraund;

    std::unordered_map<std::string, std::shared_ptr<CeAxis>> axis_H;
    std::unordered_map<std::string, std::shared_ptr<CeAxis>> axis_V;

    std::unordered_map<std::string, std::shared_ptr<CeGraph>> lines;

public:
    template<typename Engine>
    void add_element(std::vector<float> && data_,Color color,std::string list,std::string name,std::string name_x_axis,std::string name_y_axis,Engine* win){
        //обработка данных
        float size=data_.size()-1;

        float max=*std::max_element(data_.begin(),data_.end());
        float min=*std::min_element(data_.begin(),data_.end());

        //загрузка листа
        auto celist=win->resource.get_list(list);

        //создание графика


        if(lines.find(name)!=lines.end()){
            lines.erase(name);

            axis_H[name_x_axis]->delete_graf(name);
            axis_V[name_y_axis]->delete_graf(name);
            std::cout<<lines.size()<<"|dddddddddddddddddddddddddddddddddddd|"<<std::endl;
        }

        auto line=std::make_shared<CeGraph>(win,celist,Data_CeLine(std::make_shared<Mesh_data>(Mesh_data{std::move(data_)})));

        lines.emplace(name,line);

        //настройка графика


        Them_centre_::Them_object_base_base them;
        them.set_data<Enum_them::color_line>(color);

        lines[name]->update_them(&them);

        std::cout<<max<<"|jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj|"<<min<<std::endl;
        lines[name]->change_position().position->move(glm::vec3{-0.5f,-0.5f-min/(max-min),-0.1f});
        lines[name]->change_position().position->scale(glm::vec3{1.0f/size,1.0f/(max-min),1.0f});
        //присоединение осей
        axis_H[name_x_axis]->add_graf(name,typename CeAxis::Graf_contoroller{size,0,line});
        axis_V[name_y_axis]->add_graf(name,typename CeAxis::Graf_contoroller{max,min,line});

        this->change_position();
    }


    struct Data_inzilization{
        std::filesystem::path path_image="";

        std::unordered_map<std::string,Setting_axis> setting_axis_H;
        std::unordered_map<std::string,Setting_axis> setting_axis_V;

        std::function<void(void *,CeGraphics *)> funs_out;

        template<typename Engine>
        inline static std::function f{
            [](std::vector<float> && data,uint32_t color,std::string list,std::string name,std::string x,std::string y, void * win, CeGraphics * ptr){
            if(ptr!=nullptr){
               ptr->add_element(std::move(data),color,list,name,x,y,static_cast<Engine *>(win));
            }
        }};


        template<typename Engine>
        Data_inzilization(Engine * win,nlohmann::json &json,CePlagin_menedger * plagin_engine):funs_out(fun_out<2>(plagin_engine,json,f<Engine>)){
            get_to_check(json,"path_image",path_image);

            for(auto &[M,J]:json["Setting_axis"].items()){
                Setting_axis setting;
                get_to_check(J,"type",setting.type);
                get_to_check(J,"name_axis",setting.name_axis);
                get_to_check(J,"accurate",setting.accurate);
                get_to_check(J,"cout_divisions",setting.cout_divisions);
                get_to_check(J,"font",setting.path_font);

                if(setting.type==1){
                    setting_axis_H.emplace(setting.name_axis,setting);

                }else{
                    setting_axis_V.emplace(setting.name_axis,setting);
                }
            }
        }

        Data_inzilization(std::filesystem::path path_image_):path_image(path_image_){}
        operator Data_CePlate(){
            return Data_CePlate{path_image};
        }
    };

    template<typename Engine>
    CeGraphics(Engine *win_,typename Engine::List * celist_,Data_inzilization data):
        backgraund(celist_,data){

        //std::cout<<data.funs_out.size()<<">"<<std::endl;
        data.funs_out(win_,this);

        //переделать
        for (auto &[S,T]:data.setting_axis_H) {
            auto t=std::make_shared<CeAxis>(win_,celist_,typename CeAxis::Data_inzilization(T));
            axis_H.emplace(T.name_axis,t);
            axis_H[T.name_axis]->placement_of_elements();
            axis_H[T.name_axis]->installing_the_theme();
        }

        for (auto &[S,T]:data.setting_axis_V) {
            auto t=std::make_shared<CeAxis>(win_,celist_,typename CeAxis::Data_inzilization(T));
            axis_V.emplace(T.name_axis,t);
            axis_V[T.name_axis]->placement_of_elements();
            axis_V[T.name_axis]->installing_the_theme();
        }
    }


    void placement_of_elements(){
        int i=0;
        for(auto &[SS,TT]:axis_H){
            TT->change_position().position->move(glm::vec3{-0.5f,-0.5f-i*0.2,-0.01f});
            i++;
        }

        i=0;
        for(auto &[SS,TT]:axis_V){
            TT->change_position().position->rotete(glm::vec3(1.0f, 1.0f, 0.0f),180.0f);
            TT->change_position().position->move(glm::vec3{-0.5f-i*0.15,-0.5f,-0.03f});
            i++;
        }
    }

    virtual void update_position(const Position *view){
        backgraund.template change <Position>().get_data()->set_view(view);
        for(auto &[SS,TT]:lines){
            TT->change_position().position->set_view(view);
        }
        for(auto &[SS,TT]:axis_H){
            TT->change_position().position->set_view(view);
        }
        for(auto &[SS,TT]:axis_V){
            TT->change_position().position->set_view(view);
        }
    }



    void installing_the_theme(){

    }

    virtual void update_them(Them_centre_::Them_object_base_base * them){//а вот тут нужен будет массив

    }

};




class CeWorldMap: public CeObject{
    CePlate map;
    CePlate marker;
    std::vector<std::shared_ptr<CeObject>> markers;

    glm::mat4 proj{1.0};

    float Zoom=60.0f;

    std::function<void(std::string,std::string)> fun_get_lan_long;
public:
    struct Data_inzilization{
        enum  TYPE_PASS:int {FORGET=0,REMEMBER};

        std::string name_them_list="";
        std::filesystem::path path_image="";
        std::filesystem::path path_marker_image="";

        std::function<void(std::string,std::string)> fun_rep=nullptr;

        template<typename Engine>
        Data_inzilization(Engine * win,nlohmann::json &json, CePlagin_menedger * plagin_engine){
            get_to_check(json,"them",name_them_list);
            get_to_check(json,"path_image",path_image);
            get_to_check(json,"path_marker_image",path_marker_image);
            fun_rep =fun_in<CePlagin_menedger,std::string,std::string>(plagin_engine,json);
        };
        Data_inzilization(std::filesystem::path path_image_,std::filesystem::path path_marker_image_):
        path_image(path_image_),path_marker_image(path_marker_image_){}
        operator Data_CePlate(){
            return Data_CePlate{path_image};
        }

        auto get_data_marker(){
            return Data_CePlate{path_marker_image};
        }

    };


    template<typename Engine>
    CeWorldMap(Engine *win_,typename Engine::List * celist_,Data_inzilization data):
        map(celist_,data),
        marker(celist_,data.get_data_marker()),
        fun_get_lan_long(data.fun_rep){
        std::cout<<"CeWorldMap"<<std::endl; 
        proj=glm::perspective(glm::radians(Zoom), 1.0f, 0.1f, 1000.0f);
        proj[2][2]=-1.0;
        *this->change_position().position=glm::vec3{0.0f,0.0f,-2.0f};
        this->change_position().position->set_projection(&proj);
    }
    //
    virtual void add_element(std::shared_ptr<CeObject> element){
        markers.emplace_back(element);
        markers.back()->change_position().position->set_view(this->get_position());
    }

   void connect(Signal_engine<CeObject> * signal){
        signal->connect(this,map.id_global,-1);
    }

    virtual void operator()(Incoming_signals_from_user * ISFU,typename CeObject::Interaction_Repetition_area){
        //auto scale = this->position_object.get_scale_object();
        if(ISFU->keys[GLFW_MOUSE_BUTTON_LEFT].action==1){
           auto pos_w = this->get_position()->get_position_world();
           auto move=ISFU->move_to_cursor();

           float x_sm_1=(pos_w.x+move.x)/proj[0][0]-0.5;
           float x_sm_2=(pos_w.x+move.x)/proj[0][0]+0.5;

           float y_sm_1=(pos_w.y+move.y)/proj[1][1]-0.5;
           float y_sm_2=(pos_w.y+move.y)/proj[1][1]+0.5;

           bool b=(x_sm_1>0)+(x_sm_2<0)+(y_sm_1>0)+(y_sm_2<0);

           if(!b){
               this->change_position().position->move(glm::vec3{move.x/proj[0][0],move.y/proj[1][1],0.0f});
           }
        }

        if(ISFU->yoffset!=0){
            //pos_map.new_scale_offset(pos_map.get_scale_object()+glm::vec3{win->ISFU->yoffset*0.25f,win->ISFU->yoffset*0.25f,0.0f},-pos_map.cursor_position());
            Zoom+=-ISFU->yoffset;
            if(Zoom<1.0){
                Zoom=1.0;
            }
            if(Zoom>180.0){
                Zoom=180.0;
            }
            //std::cout<<Zoom<<std::endl;
            proj=glm::perspective(glm::radians(Zoom), 1.0f, 0.1f, 1000.0f);
            proj[2][2]=-1.0;
            this->change_position();

            //pos_map.new_scale_offset(pos_map.get_scale_object()+glm::vec3{win->ISFU->yoffset*0.1f,win->ISFU->yoffset*0.1f,0.0f},glm::vec3{-0.5,-0.5,0});
        }
        auto t=ISFU->cursor_position();
        auto pos_w = this->get_position()->get_position_world();

        if(ISFU->keys[GLFW_MOUSE_BUTTON_RIGHT].action==1){
            marker.template change <Position>().get_data()->new_move(glm::vec3{t.x/proj[0][0]-pos_w.x,t.y/proj[1][1]-pos_w.y+1/(16.0f*3.0f)*0.6, 0.9500001f});
            std::cout<<(t.x/proj[0][0]-pos_w.x)<<"|"<<(t.y/proj[1][1]-pos_w.y)<<std::endl;
            if(fun_get_lan_long){
                fun_get_lan_long(std::to_string((t.y/proj[1][1]-pos_w.y)*82*2),std::to_string((t.x/proj[0][0]-pos_w.x)*2*180));
            }
        }

        //


    };

    void placement_of_elements(){
        map.template change <Position>().get_data()->move(glm::vec3{0.0f, 0.0f, 0.95f});
        //marker.template change <Position>().get_data()->new_move(glm::vec3{0.0f, 0.0f, 0.9500001f});
        marker.template change <Position>().get_data()->scale(glm::vec3{1/(16.0f*3.0f),1/(16.0f*3.0f), 1.0f});
    }

    virtual void update_position(const Position *view){
        map.template change <Position>().get_data()->set_view(view);
        marker.template change <Position>().get_data()->set_view(view);
        for(auto &TT:markers){
            TT->change_position().position->set_view(view);
        }
    }

    void installing_the_theme(){

    }

    virtual void update_them(Them_centre_::Them_object_base_base * them){//а вот тут нужен будет массив

    }
};
////////////
///
///
///
///
///
//////////////


class CeFPS: public CeObject{
    int FPS=0;
    CeText text;
    char fps[9]={'F','P','S',':',' ','0','0','0'};
public:

    using Data_inzilization = Data_CeText;

    template<typename Engine>
    CeFPS(Engine *win_,typename Engine::List * celist_,Data_CeText data):
        text( celist_,data){
        std::cout<<"CeFPS"<<std::endl;
    }

    void connect(Signal_engine<CeObject> * signal){
        signal->connect(this,typename CeObject::Infiniti{});
    }


    virtual void operator()(Incoming_signals_from_user * ISFU,typename CeObject::Infiniti){
        FPS=static_cast<int>(1/(ISFU->deltaTime));
        std::to_chars(&fps[5],&fps[7],FPS);

        //std::string s=std::string{"F"};
        text.change_text(fps);
        //std::cout<<FPS<<std::endl;
    }
};



class Stroke_of_fate: public CeObject{
    CeText text_1;
    CeText text_2;
    bool m=false;
public:
    template <typename Engine>
    Stroke_of_fate(Engine *win_,typename Engine::List * celist_,typename Engine::Data data):
        text_1(win_,celist_,Data_CeText{Directory_fonts+"SourceCodePro-Regular.ttf","Создатель: "}),
        text_2(win_,celist_,Data_CeText{Directory_fonts+"SourceCodePro-Regular.ttf","Кузнецов Даниил Андреевич"}){
        text_1.template change <Material_T>().get_data()->color=glm::vec4{65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};
        text_2.template change <Material_T>().get_data()->color=glm::vec4{65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};


    }

    void connect(Signal_engine<CeObject> * signal){
        signal->connect(this,GLFW_KEY_L,typename CeObject::Key_signal{});
        signal->connect(this,GLFW_KEY_H,typename CeObject::Key_signal{});///
        signal->connect(this,typename CeObject::Infiniti{});
    }

    virtual void operator()(Incoming_signals_from_user * ISFU, typename CeObject::Key_signal){;
        if(ISFU->keys[GLFW_KEY_L].action && ISFU->keys[GLFW_KEY_H].action){
            if(m){
                text_1.template change <Position>().get_data()->new_move(glm::vec3{-1000.9f,0.22f,-0.9f});
                text_2.template change <Position>().get_data()->new_move(glm::vec3{-1000.9f,-0.25f,-0.9f});
                m=false;
            }else {
                text_1.template change <Position>().get_data()->new_move(glm::vec3{-0.9f,0.22f,-0.9f});
                text_2.template change <Position>().get_data()->new_move(glm::vec3{-0.9f,-0.25f,-0.9f});
                m=true;
            }
        }
    }

    void placement_of_elements(){
        text_1.template change <Position>().get_data()->scale(glm::vec3{0.22f,0.22f,1.0f});
        text_1.template change <Position>().get_data()->new_move(glm::vec3{-1000.9f,0.22f,-0.9f});
        text_2.template change <Position>().get_data()->scale(glm::vec3{0.22f,0.22f,1.0f});
        text_2.template change <Position>().get_data()->new_move(glm::vec3{-1000.9f,-0.25f,-0.9f});
    }

};


//class CeDetectedClic: public CeObject{
//    CeText text;
//    char t_[3]={' ',' ',' '};
//public:
//    template<typename Engine>
//    CeDetectedClic(Engine *win_,typename Engine::List * celist_,Data_CeText data):
//        text(win_,celist_,data){
//        std::cout<<"CeDetect"<<std::endl;
//    }

//    void connect(Signal_engine<CeObject> * signal){
//         signal->connect(this,0,id_global);
//    }

//    virtual void operator()(Incoming_signals_from_user * ISFU,typename CeObject::Interaction_Repetition_area){
//        std::to_chars(&t_[0],&t_[3],ISFU->);
//        text.change_text(t_);
//        t_[0]=' ';
//        t_[1]=' ';
//        t_[2]=' ';
//    }
//};



#endif // OBJECT_LIST_TEMPLATE_H
