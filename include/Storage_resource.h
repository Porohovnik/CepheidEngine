#ifndef STORAGE_RESOURCE_H
#define STORAGE_RESOURCE_H
#include "tuple_utl.h"
#include "CeEnumeration.h"

namespace CeEngine {
template<typename ...Arg>
class Storage_resource{
    std::tuple<Arg...> storage_data{};
public:
    Storage_resource(){}

    Storage_resource(const Storage_resource& )=delete;
    Storage_resource(Storage_resource&& )=delete;
    Storage_resource & operator=(Storage_resource &&)=delete;
    Storage_resource & operator=(const Storage_resource &)=delete;


    void new_frame(){
        tutl::TupleForeach(storage_data,[](auto &t){
            if constexpr(std::remove_reference_t<decltype (t)>::type_order_update==TYPE_ORDER_UPDATE::FIRST){
               t.new_frame();
            }
        });
        tutl::TupleForeach(storage_data,[](auto &t){
            if constexpr(std::remove_reference_t<decltype (t)>::type_order_update==TYPE_ORDER_UPDATE::ANY){
               t.new_frame();
            }
        });
    }

    template<typename Type,typename ...Arg_>
    auto add_data(Arg_ && ...arg_){
        auto * get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(storage_data);
        return get->add_data(std::forward<Arg_>(arg_)...);
    }

    template<typename Type>
    auto * get_unit(){
        return tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(storage_data);
    }

    template<typename Type,typename ...Arg_>
    void add_task(Type res,Type_Status status){
        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(storage_data);
        get->add_task(res, status);
    }
};
}// namespace CeEngine
#endif // STORAGE_RESOURCE_H
