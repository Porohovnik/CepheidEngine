#ifndef DATA_RES_ONE_H
#define DATA_RES_ONE_H
#include <utility>
#include "Data_ABC.h"
namespace CeEngine {
template<TIME_BIND bind_time,typename S,typename T,int bind_base=-1>
class Data_res_one:public Data_ABC<bind_time,T>{
    T RES=nullptr;
    S * storage_res_;

public:
    inline constexpr static TYPE_RES_STORAGE type_res_=TYPE_RES_STORAGE::GLOBAL;
    inline static constexpr TYPE_CONTAINER type_container=TYPE_CONTAINER::ONE;

    template<typename Memory, typename Info_environment>
    Data_res_one(Data_herald<Memory,Info_environment> herald):storage_res_(herald.res){}

    template<typename ...Arg>
    bool emplace(Arg...arg){
        if(RES!=nullptr){
            storage_res_->add_task(RES,Type_Status::CLEAR);
        }
        RES=storage_res_->template add_data<T>(std::forward<Arg>(arg)...);
        storage_res_->add_task(RES,Type_Status::VISEBLE);
        return true;
    }

    bool earse(){
        if(RES!=nullptr){
            storage_res_->add_task(RES,Type_Status::CLEAR);
            RES=nullptr;
        }
        return true;
    }

    void bind() const{
        RES->Bind(bind_base);
    }

    void bind(std::size_t id) const{
        RES->Bind(bind_base,id);
    }

    constexpr auto * get_element() const{
        return &RES;
    }
};
}// namespace CeEngine
#endif // DATA_RES_ONE_H
