#ifndef DATA_RES_ONE_H
#define DATA_RES_ONE_H
#include <utility>
#include <memory>

#include "Data_ABC.h"
namespace CeEngine {
template<TIME_BIND bind_time,typename T,int bind_base=-1>
class Data_res_one:public Data_ABC<bind_time,T>{
    std::shared_ptr<T> RES=nullptr;
public:
    inline constexpr static TYPE_RES_STORAGE type_res_=TYPE_RES_STORAGE::GLOBAL;
    inline static constexpr TYPE_CONTAINER type_container=TYPE_CONTAINER::ONE;

    Data_res_one(){}

    template<typename ...Arg>
    bool emplace(Arg &&...arg){
        RES=std::make_shared<T>(std::forward<Arg>(arg)...);
        RES->to_RAM();
        RES->to_VRAM();
        return true;
    }

    bool earse(){
        RES=nullptr;
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
