#ifndef DATA_RES_H
#define DATA_RES_H
#include <vector>
#include <list>

#include "Data_ABC.h"
#include "Identification.h"
namespace CeEngine {
template<TIME_BIND bind_time,typename S,typename T,int bind_base=-1>
class Data_res:public Data_ABC<bind_time,T>{
    std::vector<T> RES{nullptr};//странная дичь
    S * storage_res_;
public:
    inline constexpr static TYPE_RES_STORAGE type_res_=TYPE_RES_STORAGE::GLOBAL;
    inline bool new_frame(){return  false;}

    template<typename Memory, typename Info_environment>
    Data_res(Data_herald<Memory,Info_environment> herald):storage_res_(herald.res){}

    bool isData_storage(decltype(std::remove_reference_t<typename S::template get_unit_type<T>>::get_key()) key){
        return storage_res_->template get_unit<T>()->isData(key);
    }

    template<typename ...Arg>
    std::size_t add_element(std::size_t id,decltype(std::remove_reference_t<typename S::template get_unit_type<T>>::get_key()) key, Arg...arg){
        if(RES.size()<=id){
            RES.resize(id+1,nullptr);
        }
        //std::cout<<id<<"|*********************|"<<RES.size()<<std::endl;

        RES[id]=storage_res_->template add_data<T>(key,std::forward<Arg>(arg)...);
        //хммммммм//storage_res_->add_task(RES[id],Type_Status::VISEBLE);
        return id;
    }

    std::size_t add_element(std::size_t id, decltype(std::remove_reference_t<typename S::template get_unit_type<T>>::get_key()) key){
        if(RES.size()<=id){
            RES.resize(id+1,nullptr);
        }

        RES[id]=storage_res_->template get_unit<T>()->get_data(key);
        return id;
    }

    bool change(std::size_t id, T&&element){

        //всё иначе
        storage_res_->add_task(RES[id],Type_Status::CLEAR);
        RES[id]=element;
        //auto get=storage_res_->template get_unit<T>()->clear_trash();
        return true;
    }

    bool delete_element(std::size_t id){
        if(RES.size()<=id){
            //std::cout<<RES.size()<<"font|"<<id<<std::endl;
            return true;
        }

        if(RES[id]!=nullptr){
            storage_res_->add_task(RES[id],Type_Status::CLEAR);

            RES[id]=nullptr;
        }
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
                storage_res_->add_task(RES[id->get_id()],new_status_);
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
