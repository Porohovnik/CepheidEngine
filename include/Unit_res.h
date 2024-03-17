#ifndef UNIT_RES_H
#define UNIT_RES_H
#include <map>
#include <memory>
#include <array>

#include "CeEnumeration.h"
namespace CeEngine {
template<TYPE_ORDER_UPDATE order,typename K,typename T>
class Unit_res{
    struct Status_Section{
        K key;
        Type_Status status=Type_Status::CLEAR;
        std::size_t no_use_VRAM=0;
    };

    struct Status_count{
        std::array<std::size_t,4> count={0,0,0,0};
        Type_Status last_status=Type_Status::CLEAR;

        std::size_t & operator[](Type_Status status){
            return count[status];
        }

        std::size_t & operator[](std::size_t i){
            return count[i];
        }
    };

    std::map<K,std::shared_ptr<T>> data;
    std::map<std::shared_ptr<T>,Status_Section> status_data;

    std::map<std::shared_ptr<T>,Status_count> tasks;
public:
    inline constexpr static TYPE_ORDER_UPDATE type_order_update = order;

    static std::shared_ptr<T> Type();
    Unit_res(){}

    Unit_res(const Unit_res& )=delete;
    Unit_res(Unit_res&& )=default;
    Unit_res & operator=(Unit_res &&)=delete;
    Unit_res & operator=(const Unit_res &)=delete;

    template<typename ...Arg>
    decltype (auto)  add_data(K key,Arg &&...arg){
        if(data.find(key)==data.end()){
            data.emplace(key,std::make_shared<T>(std::forward<Arg>(arg)...));
            status_data.emplace(data.at(key),Status_Section{key,Type_Status::CLEAR,0});
            //std::cout<<key <<"|:|"<<data.size()<<"|"<<data.at(key)<<"|"<<data.at(key).use_count()<<std::endl;
        }
        return data.at(key);
    }

    void process_task(const std::shared_ptr<T> & res,Status_count & status_cout){
        if(res==nullptr){
            return;
        }

        auto &current=status_data.at(res);
        //std::cout<<res<<"|"<<res.use_count()<<"|"<<current.key <<std::endl;

        current.no_use_VRAM+=status_cout[Type_Status::DISABLE];
        if(status_cout[Type_Status::CLEAR] && status_cout.last_status==Type_Status::CLEAR && res.use_count()<=3){
            data.erase(current.key);
            status_data.erase(res);
            return;
        }

        if(status_cout[Type_Status::DISABLE] && current.no_use_VRAM+4>=res.use_count()){
            current.status=Type_Status::DISABLE;
            res->clear_VRAM();
            res->to_RAM();
        }

        if(current.status==Type_Status::DISABLE && current.no_use_VRAM!=0){
            current.no_use_VRAM-=status_cout[Type_Status::VISEBLE]-status_cout[Type_Status::NOVISEBLE];
        }

        if(status_cout[Type_Status::VISEBLE] || status_cout[Type_Status::NOVISEBLE]){
            current.status=Type_Status::VISEBLE;
            res->to_RAM();
            res->to_VRAM();
        }

        //std::cout<<"#"<<res<<"|"<<res.use_count()<<"|"<<status <<std::endl;
    }

    void add_task(std::shared_ptr<T> res,Type_Status status){//сделать это ассинхронным
        if(tasks.find(res)==tasks.end()){
            tasks.try_emplace(res,Status_count{});
        }
        Status_count &t= tasks[res];
        t.count[status]++;
        t.last_status=status;;
    }


    void new_frame(){
        for (auto &[TT,SS]:tasks) {//для ассинхронности разные о
            auto &current=status_data[TT];
            //std::cout<<TT<<"tasks_size: "<<"|"<<current.key<<"|<"<<SS[0]<<"|"<<SS[1]<<"|"<<SS[2]<<"|"<<SS[3]<<">|"<<data.size()<<std::endl;

            process_task(TT,SS);
        }
        tasks.clear();
    }
};
}// namespace CeEngine
#endif // UNIT_RES_H
