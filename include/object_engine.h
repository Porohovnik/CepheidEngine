#ifndef OBJECT_ENGINE_H
#define OBJECT_ENGINE_H
#include "ceobject_fabric.h"
#include "utiliti_json.h"
#include "universal_object.h"
#include "Object_list_template.h"

//разбить на куски
template<typename Base>
class Object_engine{
    using Obj_ptr=std::shared_ptr<Base>;
    using Data=typename Base::Data;
    using Key=std::string;

    std::size_t global_id_object=0;

    std::unordered_map<std::string,Obj_ptr> objects;
public:
    template<typename Engine>
    Object_engine(std::optional<Engine>){

    //    add_type_fabric<CePlate>            ("CePlate");
    //    add_type_fabric<CeText>             ("CeText");
    //    add_type_fabric<CeFPS>              ("CeFPS");
    //     add_type_fabric<CeClic>            ("CeClic");
        add_type_fabric<Engine,CeBottom>           ("CeBottom");
        add_type_fabric<Engine,CeBoxElement>   ("CeBoxElement");
        add_type_fabric<Engine,CeInputText>        ("CeInputText");
        add_type_fabric<Engine,CeTextField>        ("CeTextField");
        add_type_fabric<Engine,CeGraphics>         ("CeGraphics");
    }
    ~Object_engine(){
        std::cout<<"dddddddddddddddddddddddd"<<std::endl;
    }

    std::unordered_map<std::string,Setting_Object> template_object;//подумать об обобщение


    CeObject_fabric<Obj_ptr,Key,void *,void *,nlohmann::json &,CePlagin_menedger *> fabric_inzilization_to_json;



    template<typename Engine,class J>
    inline void add_type_fabric(Key type){

        fabric_inzilization_to_json.template add_type(type,[](void * win_, void * list,nlohmann::json &json,CePlagin_menedger * plagin_engine) {
            std::shared_ptr<J> t=std::make_shared<J>(static_cast<Engine *>(win_),static_cast<typename Engine::List *>(list),typename J::Data_inzilization{static_cast<Engine *>(win_),json,plagin_engine});
            Base::processing_coll(static_cast<Engine *>(win_),*t);
            return t;
        });
    }


    template<typename Engine>
    Obj_ptr create_object_to_json(std::string name,Key type, Engine* win_,typename Engine::List * list,nlohmann::json &json,CePlagin_menedger * plagin_engine){
        //std::cout<<"|"<<type<<"|"<<win_<<std::endl;
        auto t=fabric_inzilization_to_json.call_fun(std::forward<Key>(type),win_,list,json,plagin_engine);
        objects.try_emplace(name,t);
        return t;
    }


    void set_template_object(std::filesystem::path path){
        template_object.merge(object_intilization(path));//подумать об обобще
    }

    template<typename Engine, typename ...Arg>
    std::shared_ptr<std::variant<CeCollected_in_parts<Arg...>, Arg...>> create_to_template_object(Engine* win_,typename Engine::List * list,std::string type, std::map<std::string,Sheme_data> data){
        using Colection = std::variant<CeCollected_in_parts<Arg...>, Arg...>;
        std::shared_ptr<Colection> object_var=nullptr;

        std::cout<<"name_primitive: "<<type<<std::endl;

        //((Arg<Win>::name_primitive==type? object_var=std::make_shared<Colection>(std::in_place_type<Arg<Win>>,std::move(Arg<Win>::get_any(win_,list,std::any{data}))):nullptr),...);
        ((Arg::name_primitive==type? object_var=std::make_shared<Colection>(std::in_place_type<Arg>,list,Arg::fun_data(data)):nullptr),...);
        std::cout<<"|"<<std::endl;

        if(object_var==nullptr){
            object_var = std::make_shared<Colection>(std::in_place_type<CeCollected_in_parts<Arg...>>,win_,list,template_object[type],data);
            std::cout<<"||7||"<<std::endl;
            Base::processing_coll(win_,std::get<CeCollected_in_parts<Arg...>>(*object_var));
            std::cout<<"||10||"<<std::endl;
        }

        return object_var;
    }

    template<class J,typename D,typename Engine,typename List>
    std::shared_ptr<J> create_object(std::string name, Engine* win_, List * list,D data){
        std::shared_ptr<J> t=Base::template create_object_no_base<J>(win_,list,data);
        objects.try_emplace(name,t);
        return t;
    }


    void delete_CeObject(std::string && name){
        objects.extract(std::forward<std::string>(name));
    }

    Obj_ptr get_CeObject(std::string name){
        return objects[std::forward<std::string>(name)];
    }
};



#endif // OBJECT_ENGINE_H
