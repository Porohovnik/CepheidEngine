 #ifndef BUFFERS_H
#define BUFFERS_H

#include <map>

#include <memory>
#include <optional>
#include <functional>
#include <iostream>

#include "celist.h"
#include "tuple_utl.h"

enum class TYPE_ORDER_UPDATE{FIRST,ANY};

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

template<typename K>
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


template<TYPE_ORDER_UPDATE order,typename K,typename T>
class Unit_res{
    std::map<K,std::shared_ptr<T>> data;
    std::map<std::shared_ptr<T>,Status_Section<K>> status_data;

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
            status_data.emplace(data.at(key),Status_Section<K>{key,Type_Status::CLEAR,0});
            //std::cout<<key <<"|:|"<<data.size()<<"|"<<data.at(key)<<"|"<<data.at(key).use_count()<<std::endl;
        }
        return data.at(key);
    }

    void process_task(const std::shared_ptr<T> & res,Status_count & status_cout){
        if(res==nullptr){
            return;
        }

        auto &current=status_data.at(res);
        std::cout<<res<<"|"<<res.use_count()<<"|"<<current.key <<std::endl;

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
            std::cout<<TT<<"tasks_size: "<<"|"<<current.key<<"|<"<<SS[0]<<"|"<<SS[1]<<"|"<<SS[2]<<"|"<<SS[3]<<">|"<<data.size()<<std::endl;

            process_task(TT,SS);
        }
        tasks.clear();
    }
};

template<typename J,typename T>
class Section{
protected:
    std::function<J()> dowm_func=nullptr;
    std::optional<J>   RAM;
    std::optional<T>   VRAM;
public:

//    template<typename JJ,typename ...Arg_>
//    static constexpr bool in_out_identically(){
//        return std::is_same_v<std::decay<J>,std::decay<JJ>>;
//    }

    template<typename ...Arg>
    Section(J(*f_)(Arg...),std::remove_reference_t<Arg> && ...arg){
        std::function<J(Arg...)> f=f_;
        dowm_func=std::bind(f,std::move(arg)...);
    }

    Section(std::nullptr_t,J arg){
        std::function<J(J)> f=[](J && arg_){
            return std::forward<J>(arg_);
        };
        dowm_func=std::bind(f,std::move(arg));
    }

//    Section()=delete;
//    Section(const Section& )=delete;
//    Section(Section&& )=default;
//    Section & operator=(Section &&)=default;
//    Section & operator=(const Section &)=delete;


    void to_RAM(){
        if(!RAM && !VRAM){
            RAM.emplace(dowm_func());
        }
    }
    void clear_RAM(){
        RAM.reset();
    }

    virtual void to_VRAM(){
        if(RAM && !VRAM){
            if constexpr(std::is_constructible_v<T,J>){
               VRAM.emplace(std::move(RAM.value()));
            }
            RAM.reset();
        }
    }

    void Bind(int i=-1){
        if(VRAM){
            VRAM->Bind(i);
        }
    }
    void Bind(int i,int base){
        if(VRAM){
            VRAM->Bind(i,base);
        }
    }

    void clear_VRAM(){
        VRAM.reset();
    }
//    auto & Get_VRAM_o(){ return VRAM;}
      T & Get_VRAM(){
        return VRAM.value();
      }
//    J & Get_RAM(){ return RAM.value();}
};

#include <any>

template<typename J,typename T>
class Section_static:public Section<J,T>{
    inline static int id=0;
    int id_current;
    std::function<void()> to_vram=nullptr;
public:
    template<typename T_st,typename ...Arg>//нужен массив бд
    Section_static(T_st  t_st ,Arg && ...arg):Section<J,T>(std::forward<Arg>(arg)...){
        id_current=id;
        auto l= std::tuple_cat(std::tuple<Section_static<J,T> *>(this),t_st);

        to_vram= std::bind([](auto m){
            std::apply([](auto ptr,auto ...t_st){
                ptr->VRAM.emplace(t_st...,ptr->id_current,std::move(ptr->RAM.value()));
             },m);},l);
        id++;
    }

    virtual void to_VRAM() override{
        if(this->RAM && !this->VRAM && to_vram){
            to_vram();//предпологается наличие статичных переменных
            this->RAM.reset();
        }
    }

     int get_id() const noexcept{
        return id_current;
    }
};

#endif // BUFFERS_H
