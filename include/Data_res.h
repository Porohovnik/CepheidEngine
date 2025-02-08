#ifndef DATA_RES_H
#define DATA_RES_H
#include <vector>
#include <list>
#include <memory>

#include "Data_ABC.h"
#include "Identification.h"

namespace CeEngine {
template<TIME_BIND bind_time,template<typename K,typename T> class Unit,typename K,typename T,int bind_base=-1>
class Data_res:public Data_ABC<bind_time,T>{
    std::vector<std::shared_ptr<T>> RES{nullptr};//странная дичь
    Unit<K,T> unit;
public:
    inline constexpr static TYPE_RES_STORAGE type_res_=TYPE_RES_STORAGE::GLOBAL;
    inline bool new_frame(){return  unit.new_frame();}

    Data_res(){}
    bool isData_storage(K key){
        return unit.isData(key);
    }

    template<typename ...Arg>
    std::size_t add_element(std::size_t id, K key, Arg...arg){
        if(RES.size()<=id){
            RES.resize(id+1,nullptr);
        }
        //std::cout<<id<<"|*********************|"<<RES.size()<<std::endl;

        RES[id]=unit.add_data(key,std::forward<Arg>(arg)...);
        //хммммммм//storage_res_->add_task(RES[id],Type_Status::VISEBLE);
        return id;
    }

    std::size_t add_element(std::size_t id, K key){
        if(RES.size()<=id){
            RES.resize(id+1,nullptr);
        }

        RES[id]=unit.get_data(key);
        return id;
    }

    bool change(std::size_t id, T&&element){

        //всё иначе
        unit.add_task(RES[id],Type_Status::CLEAR);
        RES[id]=element;
        //auto get=storage_res_->template get_unit<T>()->clear_trash();
        return true;
    }

    bool delete_element(std::size_t id){
        if(RES.size()<=id){
            //std::cout<<RES.size()<<"font|"<<id<<std::endl;
            return true;
        }
        unit.add_task(RES[id],Type_Status::CLEAR);
        RES[id]=nullptr;

        return true;
    }

    void bind(std::size_t id) const{
        if(RES.size()>id){
            if(RES[id]!=nullptr){
                if constexpr(bind_base==-1){
                    RES[id]->Bind(0,id);
                }else{
                    RES[id]->Bind(bind_base,id);
                }
            }
        }else{
//            if(RES.size()){
//                std::cout<<id<<"::"<<RES.size()<<":"<<(RES[id]==nullptr)<<std::endl;
//            }
        }
    }

    const auto * get_element(std::size_t id) const{
        return &RES[id];
    }

    void new_status(Type_Status new_status_,Identification *id){
        if(RES.size()>id->get_id()){
            if(RES[id->get_id()]!=nullptr ){
                unit.add_task(RES[id->get_id()],new_status_);
            }
        }
    }

    void new_status(Type_Status new_status_,std::list<Identification *> * ids){
        for(auto &ID:*ids){
            new_status(new_status_,ID);
        }
    }
};
}// namespace CeEngine
#endif // DATA_RES_H
