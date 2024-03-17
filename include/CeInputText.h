#ifndef CEINPUTTEXT_H
#define CEINPUTTEXT_H
#include "CeButtom.h"
namespace CeEngine {
struct Setting_input{
    enum  TYPE_INPUT:int {NAMBER=0,ANSI,UNICOD};

    TYPE_INPUT type=NAMBER;
    std::size_t cout_charachers_max=10;
    bool clear_old_value_to_new_input=true;
};

class CeInputText: public CeObject{
    std::shared_ptr<CeButtom> field;
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


    struct Data_initialization{
        typename CeButtom::Data_initialization data_bottom;
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
        Data_initialization(Engine * win,nlohmann::json &json,CePlagin_menedger * plugin_engine):data_bottom(win,json,plugin_engine),fun_bind_dll(fun_out<1>(plugin_engine,json,f)){
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

                auto fun=get_data_dll<CePlagin_menedger,void,std::string>(plugin_engine,J);
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

            auto t=fun_in<CePlagin_menedger,std::string,std::string>(plugin_engine,json);
            if(t){
                fun_rep =To_bind_in<1>(t,id);
            }

        }
        Data_initialization(std::filesystem::path path_font_,std::string value,std::string  name_them_list_):
            data_bottom(path_font_,value,name_them_list_){}

        operator typename CeButtom::Data_initialization(){
            return data_bottom;
        }
    };



    template<typename Engine>
    CeInputText(Engine *win_,typename Engine::List * celist_,Data_initialization data):
        fun_in_data(data.fun_rep),
        value(data.data_bottom.data_text.text),
        id(data.id),
        formative_fun(data.formative_fun),
        fun_active(data.funs_active),
        fun_accept(data.funs_accept){

        field=CeObject::template create_object_no_base<CeButtom>(win_,celist_,data);

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

        },CeButtom::TYPE_FUNCTION::REPEAT_LEFT);
    }

    void connect(Signal_engine<CeObject> * signal){
        fun_connect=    [signal](CeInputText * obj){
            signal->connect    (obj,                             typename CeObject::Input_signal{});
            signal->connect    (obj,          KEY_BACKSPACE,typename CeObject::Key_signal{});
            signal->connect    (obj,          KEY_ENTER    ,typename CeObject::Key_signal{});
        };

        fun_disconnect =[signal](CeInputText * obj){
            signal->dis_connect(obj,                   typename CeObject::Input_signal{});
            signal->dis_connect(obj,KEY_BACKSPACE,typename CeObject::Key_signal{});
            signal->dis_connect(obj,KEY_ENTER    ,typename CeObject::Key_signal{});
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


    virtual void operator()(Win_layer::Incoming_signals_from_user * ISFU, typename CeObject::Key_signal){
        if(ISFU->keys[KEY_BACKSPACE].action==1 && value.size()>0){
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

        if(ISFU->keys[KEY_ENTER].action==1){
            accept();
        }
    }

    virtual void operator()(Win_layer::Incoming_signals_from_user * ISFU,typename CeObject::Input_signal){
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
        Them_centre_::Them_object_base_base theme;

        theme.get_element<Enum_them::color_border>()    ->data.emplace(Color{0.0f, 0.0f, 0.0f, 1.0f});
        theme.get_element<Enum_them::color_text>()      ->data.emplace(Color{65*1.0f/256.0f, 74*1.0f/256.0f, 76*1.0f/256.0f, 1.0f});
        theme.get_element<Enum_them::color_background>()->data.emplace(Color{1.0f,1.0f,1.0f,1.0f});

        theme.get_element<Enum_them::color_border>()    ->data->action_lift={0.3f, 0.3f, 0.3f, 1.0f};
        theme.get_element<Enum_them::color_text>()      ->data->action_lift={65*1.0f/256.0f, 74*1.0f/256.0f, 76*1.0f/256.0f, 1.0f};
        theme.get_element<Enum_them::color_background>()->data->action_lift={1.0f,1.0f,1.0f,1.0f};

        update_theme(&theme);
    }


    virtual void update_position(const Position * view){
        field->change_position().position->set_view(view);
    }
    virtual void update_theme(Them_centre_::Them_object_base_base * theme){//хмм, а если пихать сюда массив?)
        field->update_theme(theme);
    }
};
}// namespace CeEngine
#endif // CEINPUTTEXT_H
