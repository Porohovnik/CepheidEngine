#ifndef STORAGE_DATA_H
#define STORAGE_DATA_H
#include <iostream>
#include <set>
#include <any>
#include <list>

#include "tuple_utl.h"

#include "Identification.h"
#include "CeEnumeration.h"
#include "GL_type_to_cpp.h"

namespace CeEngine {
template<GL_layer::TYPE_OBJECT_DRAW type_object_,int type_draw_,typename ...Arg>
class Storage_data{
    std::tuple<Arg...> data;
    std::set<std::size_t> list_to_delete;

    std::size_t size=0;
    std::size_t size_visible=0;

    std::size_t cout_call=0;
    bool change_size=false;
public:
    static constexpr GL_layer::TYPE_OBJECT_DRAW type_object(){return type_object_;}
    static constexpr int type_draw(){return type_draw_;}

    Storage_data():data(){
        std::cout<<"|:::"<<std::endl;
    }

    ~Storage_data(){
        std::size_t l=list_to_delete.size()+size;
        std::cout<<list_to_delete.size()+size<<"|"<<list_to_delete.size()+size_visible<<std::endl;
        for (std::size_t id=l+1;id>0;id--) {
            delete_element(id-1);//навсякий случай
        }

        std::cout<<list_to_delete.size()+size<<"0|"<<list_to_delete.size()+size_visible<<std::endl;

        tutl::TupleForeach(data,[](auto &t){
            if constexpr(std::remove_reference_t<decltype (t)>::type_container==TYPE_CONTAINER::ONE){
                if constexpr(std::remove_reference_t<decltype (t)>::trivial_delete==TRIVIAL_DELETE::YES){
                   t.earse();
                }
            }
        });
        std::cout<<list_to_delete.size()+size<<"1|"<<list_to_delete.size()+size_visible<<std::endl;
        tutl::TupleForeach(data,[](auto &t){
            if constexpr(std::remove_reference_t<decltype (t)>::type_container!=TYPE_CONTAINER::ONE){
                if constexpr(std::remove_reference_t<decltype (t)>::type_res_== TYPE_RES_STORAGE::GLOBAL){
                    t.new_frame();
                }
            }
        });
        std::cout<<list_to_delete.size()+size<<"|"<<"ssssssssssssssssssssssss"<<std::endl;
    }

    Storage_data(const Storage_data& )=delete;
    Storage_data(Storage_data&& )=delete;
    Storage_data & operator=(Storage_data &&)=delete;
    Storage_data & operator=(const Storage_data &)=delete;


    virtual void add_data_controller(std::size_t id,std::any &data){}
    size_t add_data(std::any data){
        change_size=true;
        std::size_t id=size;
        size++;
        size_visible++;

        if(list_to_delete.size()!=0){
            id=list_to_delete.extract(list_to_delete.begin()).value();
            std::cout<<size<<"|list_to_delete.size()!=0|"<<id<<std::endl;
        }else {

        }

        //std::cout<<size_visible<<"<<<<"<<id<<std::endl;
        add_data_controller(id,data);
        return id;
    }

    virtual void  delete_element_controller(std::size_t id){}
    bool delete_element(std::size_t id){
        change_size=true;

        delete_element_controller(id);

        if(size==0){
            size++;
            size_visible++;
        }

        list_to_delete.insert(id);
        size--;
        size_visible--;


        //std::cout<<size<<">>>>"<<id<<std::endl;
        tutl::TupleForeach(data,[id](auto &t){
            if constexpr(std::remove_reference_t<decltype (t)>::type_container!=TYPE_CONTAINER::ONE){
                if constexpr(std::remove_reference_t<decltype (t)>::trivial_delete==TRIVIAL_DELETE::YES){
                   t.delete_element(id);
                }
            }
        });
        return true;
    }

    virtual std::size_t visible_data_controller(std::list<std::list<Identification *> *> * ids,bool update=false){ std::cout<<"||5.2||"<<std::endl;return 0;}
    void visible_data(std::list<std::list<Identification *> *> * ids,bool update=false){
        cout_call+=visible_data_controller(ids,update);
    }


    virtual bool new_frame_controller(){return  false;}
    bool new_frame(){
        std::size_t b=false;
        cout_call=change_size;

        b+=new_frame_controller();
        tutl::TupleForeach(data,[&b](auto &t){
            if constexpr(std::remove_reference_t<decltype (t)>::type_container==TYPE_CONTAINER::ARRAY){
                b+=t.new_frame();
            }
        });

        cout_call+=b;
        return b;
    }

    void new_status(Type_Status status,Identification * id){
        tutl::TupleForeach(data,[status,&id](auto &t){
            if constexpr(std::remove_reference_t<decltype (t)>::type_container!=TYPE_CONTAINER::ONE){
                if constexpr(std::remove_reference_t<decltype (t)>::type_res_== TYPE_RES_STORAGE::GLOBAL){
                    t.new_status(status,id);
                }
            }
        });
    }

    void new_status(Type_Status status,std::list<Identification *>  * ids){
        tutl::TupleForeach(data,[status,&ids](auto &t){
            if constexpr(std::remove_reference_t<decltype (t)>::type_container!=TYPE_CONTAINER::ONE){
                if constexpr(std::remove_reference_t<decltype (t)>::type_res_== TYPE_RES_STORAGE::GLOBAL){
                    t.new_status(status,ids);
                }
            }
        });
    }

    template<TIME_BIND time_bind>
    static constexpr bool check_bind_time__(){
        std::array<TIME_BIND,sizeof... (Arg)> points_draw{{{Arg::bind_time_()}...}};
        return (tutl::ArraySearch<time_bind>(points_draw)+1);
    }

    template<TIME_BIND...time_bind>
    static constexpr bool check_bind_time(){
        return (check_bind_time__<time_bind>()+...);
    }


    template<TIME_BIND time_bind,typename T,typename ...Arg__>
    inline static bool bind_l(T &t,Arg__...base){
        if constexpr(std::remove_reference_t<T>::bind_time_()==time_bind){
           t.bind(base...);
           return true;
        }
        return false;
    }

    template<TIME_BIND time_bind,typename ...Arg__>
    inline void bind_(Arg__...base)const {
        if constexpr(check_bind_time<time_bind>()){
            std::size_t i=0;
            tutl::TupleForeach(data,[&i,base...](auto &t){
                bind_l<time_bind>(t,base...);
            });
        }
    }

    template<TIME_BIND...time_bind>
    inline void bind() const{
        ((bind_<time_bind>()),...);
    }

    template<TIME_BIND...time_bind>
    inline void bind(std::size_t id) const{
        ((bind_<time_bind>(id)),...);
    }

    inline void set_visible_size(std::size_t size) noexcept{
        size_visible=size;
    }

    inline std::size_t get_element_size() const noexcept{
        return size_visible;
    }

    inline std::size_t get_cout_call(){
        return cout_call;
    }

    inline std::size_t isChange_size(){
        std::size_t change_size_=change_size;
        change_size=0;
        return change_size_;
    }

    inline const std::set<std::size_t> & get_delete_ids() const{
        return list_to_delete;
    }


    template<typename Type>
    constexpr inline auto * get_element_data_GL(){
        return std::remove_reference_t<decltype (tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data))>(tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data));
    }

    template<typename Type>
    inline auto * get_element(const std::size_t id) const{
        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
        return get->get_element(id);
    }

    template<typename Type>
    inline constexpr auto * get_element() {
        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
        return get->get_element();
    }

    template<typename Type>
    static constexpr bool check_type() {
        return tutl::isGet_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>();
    }
    template<typename Type,TIME_BIND time_bind>
    static constexpr bool check_bind_time_type() {
        using k=decltype (*tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data));
        if constexpr(std::remove_reference_t<k>::value_type::bind_time_()==time_bind){
            return true;
        }
        return false;
    }

    template<typename Type>
    auto change_request(std::size_t id){
        cout_call++;
        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
        return get->change_request(id);
    }

    template<typename Type>
    auto change_request(){
        cout_call++;
        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
        return (*get).change_request();
    }

    template<typename Type,typename T_T,typename ...Arg_>
    auto change(std::size_t id, T_T &&change, Arg_ ...arg_){
        cout_call++;
        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
        return get->change(id,std::forward<T_T>(change),std::forward<Arg_>(arg_)...);
    }



    template<typename Type,typename ...Arg_>
    auto add_element(std::size_t id,Arg_ ...arg_){
        cout_call++;
        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
        return get->add_element(id,std::forward<Arg_>(arg_)...);
    }

    template<typename Type,typename ...Arg_>
    std::size_t emplace(Arg_ ...arg_){
        cout_call++;
        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
        get->emplace(std::forward<Arg_>(arg_)...);
        return 0;
    }

    template<typename Type,typename K>
    bool isData_storage(K key){
        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
        return get->isData_storage(key);
    }

    template<typename Type,typename ...Arg_>
    auto read_data(Arg_ ...arg_) const{
        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
        return get->read_data(std::forward<Arg_>(arg_)...);
    }
};
}// namespace CeEngine
#endif // STORAGE_DATA_H
