#ifndef UNIVERSAL_OBJECT_H
#define UNIVERSAL_OBJECT_H

#include "pugixml.hpp"
#include <glm/vec4.hpp> // glm::vec4
#include <glm/vec3.hpp> // glm::vec3
#include "ceobject.h"


#include <iostream>
#include <map>
#include <set>
#include <functional>
#include <ctime>
#include "glm/vec4.hpp"
#include <string>
#include <queue>
#include <list>
#include <unordered_map>
#include <cassert>
#include <filesystem>
#include <charconv>
#include <variant>

#include "tuple_utl.h"
inline float get_namber(char const * t,float base=0.0f){
    if(*t=='\0'){
        return base;
    }
    std::string s=t;

    s[s.find('.')]=',';


    #ifdef WIN32
    return std::atof(s.data());
    #else
    float f;
    std::from_chars(s.begin().base(), s.end().base(),f);
    return f;
    #endif

}

inline glm::vec3 get_xml_pos(pugi::xml_node & xml,  std::string name,float base=0.0f){
    auto s=xml.child(name.data());

    return glm::vec3{get_namber(s.attribute("x").value(),base),
                     get_namber(s.attribute("y").value(),base),
                     get_namber(s.attribute("z").value(),base)};
}

inline std::tuple<std::size_t,std::size_t,std::string> get_for_data(pugi::xml_node & xml, std::map<std::string,std::pair<std::string,std::size_t>> psevdo={}){
    std::cout<<"*******"<<xml.attribute("index_begin").value()<<"******"<<std::endl;
    if( std::string{ xml.attribute("index_begin").value()}.size()==0){//оптимизировать
        if(psevdo.size()==0){
            std::cout<<"index_begin NOT found "<<std::endl;
            assert(true);
        }
        std::string name=xml.attribute("name").value();

        if(psevdo.find(name)==psevdo.end()){
            std::cout<<"index_begin or name_pseudonym NOT found "<<name<<std::endl;
            assert(true);
        }
        std::cout<<"*******|"<<psevdo[name].first<<"|******"<<std::endl;
        return {psevdo[name].second,
                psevdo[name].second+1,
                psevdo[name].first};
    }

    return {get_namber(xml.attribute("index_begin").value()),
            get_namber(xml.attribute("index_end").value(),get_namber(xml.attribute("index_begin").value()))+1,
            xml.attribute("type").value()
    };
}


template<typename T>
struct Setting_Object_parant_for{
    using Begin =std::size_t;
    using End =std::size_t;

    Begin begin=0;
    End   end=0;

    Setting_Object_parant_for( Begin begin_,End  end_):begin(begin_),end(end_){}
    Setting_Object_parant_for():begin(0),end(0){}

    std::map<std::string,T> data;
};

struct Sheme_data{
    std::string original;

    std::string data="";
    bool isdata=false;
};

struct Setting_Object_parant{
    std::size_t cout=0;
    bool base_type=true;

    std::map<std::size_t, Setting_Object_parant_for<glm::vec3>> placment;
    std::map<std::size_t, Setting_Object_parant_for<glm::vec3>> update_position;

    std::map<std::size_t, Setting_Object_parant_for<Sheme_data>> them_interface;
    std::map<std::size_t, Setting_Object_parant_for<Sheme_data>> sheme_data_object;
};

struct Setting_Object{
    std::unordered_map<std::string,Setting_Object_parant> object_parents{};

    static void print(glm::vec3 v){
        std::cout<<"<|"<<v.x<<"|"<<v.y<<"|"<<v.z<<"|>"<<std::endl;
    }
    static void print(Sheme_data s){
        std::cout<<"|"<<s.original<<"|"<<s.data<<"|"<<s.isdata<<"|"<<std::endl;
    }

    void print(){
        for(auto &T:object_parents){
            std::cout<<" "<<std::endl;
            std::cout<<T.first<<"|"<<T.second.cout<<std::endl;
            std::cout<<"placment"<<std::endl;
            for(auto &[B,J]:T.second.placment){
                for (auto & M:J.data) {
                    std::cout<<"|"<<M.first<<"|"<<J.begin<<"|"<<J.end<<"|"<<std::endl;
                    print(M.second);
                }
            }

            std::cout<<"update_position"<<std::endl;
            for(auto &[B,J]:T.second.update_position){
                for (auto & M:J.data) {
                    std::cout<<"|"<<M.first<<"|"<<J.begin<<"|"<<J.end<<"|"<<std::endl;
                    print(M.second);
                }
            }


            std::cout<<std::endl<<"sheme_data_object"<<std::endl;
            for(auto &[B,J]:T.second.sheme_data_object){
                for (auto & M:J.data) {
                    std::cout<<"|"<<M.first<<"|"<<J.begin<<"|"<<J.end<<"|"<<std::endl;
                    print(M.second);
                }
            }
        }

    }
};



template<typename  ...Arg>
class CeCollected_in_parts: public CeObject,public std::enable_shared_from_this<CeCollected_in_parts<Arg...>>{

    using Object = std::shared_ptr<std::variant<CeCollected_in_parts<Arg...>, Arg...>>;
    std::vector<Object> collection_object;

    std::function<void(std::vector<Object> *)> fun_intr_pos;
    std::function<void(std::vector<Object> * ,const Position*)> fun_update_pos;

public:
    virtual void placement_of_elements(){
        fun_intr_pos(&collection_object);//всегда не нулевой
    }
    virtual void update_position(const Position *view){
        fun_update_pos(&collection_object,view);//всегда не нулевой
    }

    template<typename Engine>
    CeCollected_in_parts(Engine *win_,typename Engine::List * celist_,Setting_Object setting, std::map<std::string,Sheme_data> data){

        std::vector<std::function<void(std::vector<Object> *)>> to_placment;
        std::vector<std::function<void(std::vector<Object> *,const Position*)>> to_update;
        
        std::cout<<"|||||||||||"<<data.size()<<std::endl;
        //placment два варианта:
        //1) объекты, позиция которых задаётся функциями(как предопределённые, так и через дату)
        //2) объекты  зависимые от положения друг другу (присоединённые)
        
        
        //update  варианта:
        //1) объекты, позиция которых задаётся функциями(как предопределённые, так и через дату)
        //2) объекты  зависимые от положения друг другу (присоединённые)]
        //3) условные выражения в зависимости от размеров окна
        //4) присоединение к основному объекту
        //5) параметры изменения позиции, встраиваемые в функции
        
//        for(auto &[Name,T]:setting.object_parents){
//            for (auto &[B,J]:T.sheme_data_object) {
//                std::cout<<"-----------"<<std::endl;
//                for (auto &[SS,P]:J.data) {
//                    std::cout<<SS<<"|"<<P.data<<"|"<<P.original<<std::endl;
//                }
//            }
//        }
//        std::cout<<"||:::::::::||>"<<std::endl;
//        for (auto &[BB,T]:data) {
//            std::cout<<BB<<"|"<<T.data<<std::endl;
//        }
        
        std::size_t i_global=0;
        std::size_t i_global_s=0;
        for(auto &[Name,T]:setting.object_parents){

            for (auto &[B,J]:T.placment) {
                std::vector<std::function<void(Position *)>> fun_pos;

                for(auto &[fun_name,data]:J.data){//вынести
                    if(fun_name=="move") {fun_pos.emplace_back(std::bind(&Position::move,std::_Placeholder<1>{},data));}
                    if(fun_name=="scale"){fun_pos.emplace_back(std::bind(&Position::scale,std::_Placeholder<1>{},data));}
                }

                std::size_t ID_palcment=J.begin-1;
                std::size_t end=J.end-1;

                to_placment.push_back([ID_palcment,end,fun_pos,i_global](std::vector<Object> * objs){
                    for (std::size_t i=ID_palcment;i<end;i++) {
                        std::visit(tutl::overloaded{
                            [&](CeCollected_in_parts<Arg...> & obj) { auto t = obj.change_position();
                                                                             auto pos=t.position;
                                                                             tutl::array_to_for(fun_pos,pos);},
                            [&](Arg & arg)                         { auto t = arg.template change <Position>();
                                                                             auto pos=t.get_data();
                                                                             tutl::array_to_for(fun_pos, pos);}...
                        }, *(*objs)[i+i_global]);
                    }
                });
            }


            for (auto &[B,J]:T.update_position) {
                std::vector<std::function<void(Position *,const Position *)>> fun_pos;

                for(auto &[fun_name,data]:J.data){//вынести
                    if(fun_name=="set_view") {fun_pos.emplace_back(std::bind(&Position::set_view ,std::_Placeholder<1>{},std::_Placeholder<2>{}));}
                }

                std::size_t ID_palcment=J.begin-1;
                std::size_t end=J.end-1;

                to_update.push_back([ID_palcment,end,fun_pos,i_global](std::vector<Object> * objs,const Position* pos_view){
                    for (std::size_t i=ID_palcment;i<end;i++) {
                        std::visit(tutl::overloaded{
                            [&](CeCollected_in_parts<Arg...> & obj) { auto t = obj.change_position();
                                                                             auto pos=t.position;
                                                                             tutl::array_to_for(fun_pos,pos,pos_view);},
                            [&](Arg & arg)                         { auto t = arg.template change <Position>();
                                                                             auto pos=t.get_data();
                                                                             tutl::array_to_for(fun_pos,pos,pos_view);}...
                        }, *(*objs)[i+i_global]);
                    }
                });
            }


            //инициализация встроенных объектов
            for (auto &[B,J]:T.sheme_data_object) {
                    //для создания объектов в объекте, нужен их тип и схема инцилизации информации от внешнего источника

                    for (auto &[BB,T]:data) {
                        auto t=J.data.find(BB);
                        if(t!=J.data.end()){
                            auto e = J.data.extract(t);
                            std::cout<<BB<<"|"<<e.empty()<<"|"<<data.size()<<"|"<<e.mapped().original<<std::endl;
                            e.key()=e.mapped().original;
                            e.mapped().data=T.data;
                            e.mapped().isdata=T.isdata;
                            J.data.insert(std::move(e));

                        }
                    }
                for (std::size_t i=J.begin;i<J.end;i++){
                    collection_object.emplace_back(win_->objects.template create_to_template_object<Arg...>(win_,celist_,Name,J.data));//а вот тут уже особенности
                }
                i_global_s+=J.end-J.begin;
            }
            i_global=i_global_s;//не всё очевидно
        }
        fun_intr_pos=tutl::get_to_for_array_fun(to_placment);
        fun_update_pos=tutl::get_to_for_array_fun(to_update);
    }

};



std::unordered_map<std::string,Setting_Object> object_intilization(std::filesystem::path path){
    pugi::xml_document doc;
    doc.load_file(path.c_str());

    std::unordered_map<std::string,Setting_Object> temlates;

    for (auto &Object: doc){

        // особая обработка ->
//        for (auto &J: Object.attributes()) {
//            std::cout<<Object.name() <<"|"<<J.value()<<std::endl;
//        }
    // <-
        temlates[Object.attribute("name").value()]={};
        auto & setting=temlates[Object.attribute("name").value()];
        std::cout<<"{"<<Object.attribute("name").value()<<"}"<<std::endl;

        std::map<std::string,std::pair<std::string,std::size_t>> pseudonyms;

        for (auto &M: Object.children("Object_parent")) {
            setting.object_parents[M.attribute("type").value()].cout=get_namber(M.attribute("count").value());
            for (auto &S:M.children("pseudonym")) {
                pseudonyms[S.attribute("name").value()]={M.attribute("type").value(),get_namber(S.attribute("id").value())};
            }
        }


        for (auto &D: Object.child("dataShem").children("Object_bind") ) {
            auto [begin,end, type] =get_for_data(D,pseudonyms);
            std::size_t key=(end<<sizeof(std::size_t)*8/2)+begin;

            if(setting.object_parents[type].sheme_data_object.find(key)==setting.object_parents[type].sheme_data_object.end()){
                setting.object_parents[type].sheme_data_object[key]={begin,end};
            }
            auto &t=setting.object_parents[type].sheme_data_object[key];

            std::cout<<type<<"|"<<begin<<"*************"<<key<<"******************"<<end<<"|"<<setting.object_parents[type].placment.size()<<std::endl;

            for(auto &Data_set:D.children("data_set")){
                t.data[Data_set.attributes_begin()->name()]=Sheme_data{Data_set.attributes_begin()->name(),Data_set.attributes_begin()->value(),true};
            }

            for(auto &Data_set:D.children("data_name")){
                t.data[Data_set.attributes_begin()->value()]=Sheme_data{Data_set.attributes_begin()->name(),"",false};
            }

        }

        for (auto &D: Object.child("placment").children("Object_bind") ) {
            auto [begin,end, type] =get_for_data(D,pseudonyms);
            std::size_t key=(end<<sizeof(std::size_t)*8/2)+begin;

            if(setting.object_parents[type].placment.find(key)==setting.object_parents[type].placment.end()){
                setting.object_parents[type].placment[key]={begin,end};
            }
            auto &t=setting.object_parents[type].placment[key];

            t.data["scale"] = get_xml_pos(D,"scale",1.0f);
            t.data["move"]  = get_xml_pos(D,"move");
        }

        for (auto &D: Object.child("update_position").children("Object_bind") ) {
            auto [begin,end, type] =get_for_data(D,pseudonyms);
            std::size_t key=(end<<sizeof(std::size_t)*8/2)+begin;

            if(setting.object_parents[type].update_position.find(key)==setting.object_parents[type].update_position.end()){
                setting.object_parents[type].update_position[key]={begin,end};
            }
            auto &t=setting.object_parents[type].update_position[key];

//            t.data["scale"] = get_xml_pos(D,"scale",1.0f);
//            t.data["move"]  = get_xml_pos(D,"move");
            t.data["set_view"]  = get_xml_pos(D,"set_view",0.0f);
        }


        std::cout <<"|<"<<">|"<<std::endl;
        setting.print();
    }

    return temlates;
}

#endif // UNIVERSAL_OBJECT_H
