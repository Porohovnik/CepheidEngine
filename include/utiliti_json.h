#ifndef UTILITI_JSON_H
#define UTILITI_JSON_H
#include <nlohmann/json.hpp>
#include <vector>
#include <functional>
#include <string>

#include <iostream>
#include <list>
//вынести в отдельный файл
#include <glm/vec3.hpp> // glm::vec4
#include "position.h"

#include <incoming_signals_from_user.h>
#include "Create_screenshot.h"
#include "CePlagin_menedger.h"
#include <tuple_utl.h>
#include "celist.h"
namespace CeEngine {

template< typename T>
inline bool get_to_check(nlohmann::json &j,std::string_view key,T &t){
    if(j[key].size()){
        j[key].get_to(t);
        return true;
    }
    return false;
}


inline static std::size_t id_global_object=0;
inline std::string get_id(nlohmann::json &json){
    std::string id;
    get_to_check(json,"id",id);
    if(!id.size()){
        get_to_check(json,"name",id);
    }
    if(!id.size()){
        id="id_global_object:"+std::to_string(++id_global_object);
    }
    return id;
}


enum  TYPE_BUTTON:int {LIFT=0,RIGHT};
template<typename F>
void instal_array_fun_button(nlohmann::json &json,std::vector<F> array[], F fun){
    if(fun==nullptr){
        return ;
    }

    std::string type_button="lift";
    get_to_check(json,"type_button",type_button);

    if(fun!=nullptr){
        if(type_button=="right"){
            array[RIGHT].emplace_back(fun);
        }else {
            array[LIFT].emplace_back(fun);
        }
    }
}

template <typename Plagin_engine,typename Return,typename ...Arg>
inline auto get_data_dll(Plagin_engine * plugin_engine, nlohmann::json &json){
    std::string name_shared_lib="";
    std::string name_fun="";
    get_to_check(json,"name_shared_lib",name_shared_lib);
    get_to_check(json,"name_fun",name_fun);

    auto * plugin=plugin_engine->get_Shared_modul(name_shared_lib);
    if(plugin!=nullptr){
        auto t=plugin->template get_fun<Return,Arg...>(name_fun);
        std::cout<<name_shared_lib<<"#################"<<name_fun<<"|"<<(t.operator bool()==true)<<std::endl;
        return t;
    }else{
        return std::function<Return(Arg...)>{nullptr};
    }
}


template <typename Plagin_engine,typename ...Arg>
inline auto fun_in(Plagin_engine * plugin_engine,nlohmann::json &json){
    std::function<void(Arg...)> funs_get=nullptr;
    std::vector<std::function<void(Arg...)>> funs;

    for(auto &[M,J]:json["funs_dll"].items()){
        std::string type="";
        get_to_check(J,"type",type);
        if(type=="in_data"){
            auto fun=get_data_dll<Plagin_engine,void,Arg...>(plugin_engine,J);
            if(fun!=nullptr){
                funs.push_back(fun);
            }
        }
    }

    if(funs.size()){
        funs_get=[=](Arg... val){
            for(auto &T:funs){
                T(val...);
            }
        };
    }
    return funs_get;
}



template<size_t... i,typename F,typename ...Arg>
inline auto to_bind_in(F f,std::index_sequence<i...>, Arg...arg){
    if constexpr(sizeof... (i)){
        return std::bind(f,std::_Placeholder<(i+1)>{}...,arg...);
    }else {
        return std::bind(f,arg...);
    }
}

template<int N,typename F,typename ...Arg>
inline auto To_bind_in(F f,Arg...arg){
    return to_bind_in(f,std::make_index_sequence<N>(),std::forward<Arg>(arg)...);
}

template <std::size_t N,typename F,typename TT,typename ...Ptr>
inline void fun_out_class(TT funs_out, F f,std::string id ,Ptr...ptr){
    for(auto &T:funs_out){
        T(To_bind_in<N>(f,ptr...),id);
    }
}





template<typename Plagin_engine,typename F,typename ...Arg_1,typename ...Arg_2, size_t ...i>
inline constexpr std::function<void(Arg_2...)> fun_out_(Plagin_engine * plugin_engine,nlohmann::json &json,F f, std::tuple<Arg_1...> * , std::tuple<Arg_2...> *,std::index_sequence<i...>){
    std::vector<std::function<void(std::function<void(Arg_1...)>,std::string)>>  funs_out;

    for(auto &[M,J]:json["funs_dll"].items()){
        std::string type="";
        get_to_check(J,"type",type);
        if(type=="out_data"){
            auto fun=get_data_dll<Plagin_engine,void,std::function<void(Arg_1...)>,std::string>(plugin_engine,J);
            if(fun!=nullptr){
                funs_out.push_back(fun);
            }
        }
    }

    return std::bind(fun_out_class<sizeof... (Arg_1),F,decltype (funs_out),Arg_2...>,funs_out,f,get_id(json),std::_Placeholder<(i+1)>{}...);
}


template<std::size_t N,typename Plagin_engine,typename ...Arg>
inline constexpr auto fun_out(Plagin_engine * plugin_engine,nlohmann::json &json, std::function<void(Arg...)> f){
    return fun_out_(plugin_engine,json,f,tutl::Get_first_arguments<sizeof... (Arg)-N,Arg...>(),tutl::Get_last_arguments<N,Arg...>(),std::make_index_sequence<N>{});
}



auto fun_screenshot(Win_layer::Incoming_signals_from_user *ISFU, nlohmann::json &json, CeEngine::CePlagin_menedger * plugin_engine){
    std::function<void(std::string)> funs_get=nullptr;

    std::string type="";
    get_to_check(json,"type",type);

    if(type=="screenshot"){
        float x=0.0;
        float y=0.0;
        float width=0.0;
        float height=0.0;
        std::string path="";
        std::string win_name="";

        if(json["setting"].size()){
            get_to_check(json["setting"],"x",x);
            get_to_check(json["setting"],"y",y);
            get_to_check(json["setting"],"width",width);
            get_to_check(json["setting"],"height",height);
            get_to_check(json["setting"],"path",path);
            get_to_check(json["setting"],"win_name",win_name);

            std::function<std::string()> fun_path=nullptr;
            if(json["setting"]["path_setting"].size()){
                fun_path=get_data_dll<CeEngine::CePlagin_menedger,std::string>(plugin_engine,json["setting"]["path_setting"]);
            }


            funs_get =[=](std::string s){
                auto global_path=std::filesystem::current_path();
                if(fun_path){
                    global_path/=fun_path();
                }

                auto v=     ISFU->otn_to_absolute_position(x,y);
                auto v_size=ISFU->size_otn_to_absolute_position(width,height);

                std::cout<<v.x<<"|"<<v_size.x<<"|"<<v.y<<"|"<<v_size.y<<std::endl;

                global_path/=path+".png";
                CeEngine::create_screenshot(v.x,v_size.x,v.y,v_size.y,global_path);
                std::cout<<"||"<<std::endl;
            };

            std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#####################@@@@@@@@@@@"<<std::endl;
        }
    }

    return funs_get;
}



template <typename Object_engine>
inline auto fun_change_object(Object_engine *obj_eingine,nlohmann::json &json){
    std::function<void(std::string)> funs_get=nullptr;

    std::string type="";
    get_to_check(json,"type",type);

    if(type=="change_position"){
        std::string name_object="";
        get_to_check(json,"name_object",name_object);

        std::string type_button="lift";
        get_to_check(json,"type_button",type_button);

        if(json["setting"].size()){
            glm::vec3 move_to={0.0f,0.0f,0.0f};
            glm::vec3 scale_to={0.0f,0.0f,0.0f};

            glm::vec3 move_repite={0.0f,0.0f,0.0f};
            glm::vec3 move_new_repite={0.0f,0.0f,0.0f};
            glm::vec3 scale_repite={0.0f,0.0f,0.0f};

            glm::vec3 move_start={0.0f,0.0f,0.0f};
            glm::vec3 scale_start={1.0f,1.0f,1.0f};

            glm::bvec3 move_start_b={0,0,0};
            glm::bvec3 move_new_repite_b={0,0,0};
            glm::bvec3 scale_start_b={0,0,0};

            get_to_check(json["setting"],"x_repite",move_repite.x);
            get_to_check(json["setting"],"y_repite",move_repite.y);
            get_to_check(json["setting"],"z_repite",move_repite.z);

            move_new_repite_b.x=get_to_check(json["setting"],"x_new_repite",move_new_repite.x);
            move_new_repite_b.y=get_to_check(json["setting"],"y_new_repite",move_new_repite.y);
            move_new_repite_b.z=get_to_check(json["setting"],"z_new_repite",move_new_repite.z);

            get_to_check(json["setting"],"x_offset",move_to.x);
            get_to_check(json["setting"],"y_offset",move_to.y);
            get_to_check(json["setting"],"z_offset",move_to.z);

            move_start_b.x=get_to_check(json["setting"],"x_start",move_start.x);
            move_start_b.y=get_to_check(json["setting"],"y_start",move_start.y);
            move_start_b.z=get_to_check(json["setting"],"z_start",move_start.z);

            get_to_check(json["setting"],"scale_x_offset",scale_to.x);
            get_to_check(json["setting"],"scale_y_offset",scale_to.y);
            get_to_check(json["setting"],"scale_z_offset",scale_to.z);

            get_to_check(json["setting"],"scale_x_repite",scale_repite.x);
            get_to_check(json["setting"],"scale_y_repite",scale_repite.y);
            get_to_check(json["setting"],"scale_z_repite",scale_repite.z);

            scale_start_b.x=get_to_check(json["setting"],"scale_x_start",scale_start.x);
            scale_start_b.y=get_to_check(json["setting"],"scale_y_start",scale_start.y);
            scale_start_b.z=get_to_check(json["setting"],"scale_z_start",scale_start.z);


            auto m=[=](std::string s){
                auto t=obj_eingine->get_CeObject(name_object);
                static std::map<decltype (t.get()),std::pair<bool,glm::vec3>> repite;

                if(t){
                    repite.try_emplace(t.get(),std::pair<bool,glm::vec3>{false,glm::vec3{}});
                    auto ch=t->change_position();

                    ch.position->new_move(ch.position->get_position_world()*(glm::vec3{move_start_b^true})+move_start*glm::vec3{move_start_b});
                    ch.position->move(move_to);

                    if(repite[t.get()].first){
                        repite[t.get()].first=false;
                        ch.position->scale_add(-scale_repite);

                        if(move_new_repite_b.x && move_new_repite_b.y && move_new_repite_b.z){
                            ch.position->new_move(repite[t.get()].second*glm::vec3{move_new_repite_b});
                        }

                        ch.position->move(-move_repite);
                    }else {
                        repite[t.get()].first=true;
                        repite[t.get()].second=ch.position->get_position_world();
                        if(move_new_repite_b.x && move_new_repite_b.y && move_new_repite_b.z){
                            ch.position->new_move(repite[t.get()].second*(glm::vec3{move_new_repite_b^true})+move_new_repite*glm::vec3{move_new_repite_b});
                        }
                        ch.position->scale_add(scale_repite);
                        ch.position->move(move_repite);
                    }

                    ch.position->new_scale(ch.position->get_scale_object()*(glm::vec3{scale_start_b^true})+scale_start*glm::vec3{scale_start_b});
                    ch.position->scale_add(scale_to);
                }
            };
            funs_get=m;
        }
    }
    return funs_get;
}

template <typename Resurse>
inline auto fun_change_list_status(Resurse *resource,nlohmann::json &json){
    std::function<void(std::string)> funs_get=nullptr;

    std::string type="";
    get_to_check(json,"type",type);

    if(type=="change_list"){
        std::string name_list="";
        get_to_check(json,"name_list",name_list);

        std::string set_status="";
        get_to_check(json,"set_status",set_status);

        CeEngine::Type_Status status;

        if(set_status=="CLEAR"){
            status=CeEngine::Type_Status::CLEAR;
        }
        if(set_status=="DISABLE"){
            status=CeEngine::Type_Status::DISABLE;
        }
        if(set_status=="NOVISEBLE"){
            status=CeEngine::Type_Status::NOVISEBLE;
        }
        if(set_status=="VISEBLE"){
            status=CeEngine::Type_Status::VISEBLE;
        }

        std::string type_button="lift";
        get_to_check(json,"type_button",type_button);


        funs_get =[=](std::string s){
            if(name_list!=""){
                auto celist=resource->get_list(name_list);
                celist->set_status(status);
                std::cout<<"1||"<<std::endl;
            }else{
                resource->status_change_all(status);
                std::cout<<"2||"<<std::endl;
            }
        };
    }
    return funs_get;
}


template<typename Object ,typename Plagin_engine,typename Engine>
std::vector<std::shared_ptr<Object>> load_data_json(Plagin_engine * plugin_engine,Engine* win,nlohmann::json &j){
    std::vector<std::shared_ptr<Object>> array_get;
     for(auto &[M,J]:j["list_object"].items()){
         //потом привязка к будущему
         std::vector<std::shared_ptr<Object>> array;
         if(J["list_object"].size()){
             array=load_data_json<Object,Plagin_engine,Engine>(plugin_engine,win,J);
         }

         std::string type;
         std::string name="1";
         std::string list="list_1";


         glm::vec3 move={0.0f,0.0f,0.0f};
         glm::vec3 scale={1.0f,1.0f,1.0f};


         get_to_check(J,"type",type);
         get_to_check(J,"name",name);
         get_to_check(J,"list",list);


         get_to_check(J,"x",move.x);
         get_to_check(J,"y",move.y);
         get_to_check(J,"z",move.z);

         get_to_check(J,"scale_x",scale.x);
         get_to_check(J,"scale_y",scale.y);
         get_to_check(J,"scale_z",scale.z);

         std::cout<<name<<" | "<<M<<":: "<<type<<" ::"<<std::endl;
         auto celist=win->resource.get_list(list);
         auto t=Engine::objects.create_object_to_json(name,type,win,celist,J,plugin_engine);

         auto ch=t->change_position();
         ch.position->move(move);
         ch.position->new_scale(scale);


         //сначала привязка к прошлому
         for(auto &T:array){
             t->add_element(T);
         }
         array_get.push_back(t);
     }
    return array_get;
}
} // namespace CeEngine
#endif // UTILITI_JSON_H
