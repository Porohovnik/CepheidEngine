
#ifndef INFORMATION_RENDERING_H
#define INFORMATION_RENDERING_H

#include <vector>
#include <functional>
#include <queue>
#include <any>
#include <iostream>

#include "celist.h"
#include "tuple_utl.h"
#include "free_memory_controler.h"

using uint =unsigned int;



enum class TIME_BIND{NEVER=0,SHOT=1,      OBJECT=2,
                       FIRST_SHOT=3,FIRST_OBJECT=4,
                        LAST_SHOT=5, LAST_OBJECT=6,
                        FIRST_SHOT_LAST_OBJECT=7,
                        FIRST_SHOT_FIRST_OBJECT=8,
                        LAST_SHOT_LAST_OBJECT=9,
                        LAST_SHOT_FIRST_OBJECT=10,
                        SHOT_LAST_OBJECT=11,
                        SHOT_FIRST_OBJECT=12,
                        LAST_SHOT_OBJECT=13,
                        FIRST_SHOT_OBJECT=14,
                        SHOT_OBJECT=15
                    };
enum class TYPE_CONTAINER{ONE=0,ARRAY=1};
enum class TYPE_RES_STORAGE{LOCAL=0,GLOBAL=1};
enum class TRIVIAL_DELETE{YES,NO};
//template<typename T,typename ...Arg>
//class get_first_type{
//public:
//    static T value;
//};

template<typename Memory, typename Info_environment>
struct Data_herald{
    Memory *res;
    Info_environment info;
    std::size_t max_size=0;
};

template<typename Memory,int type_object_,int type_draw_,typename ...Arg>
class Storage_data{
    Memory res;
    std::tuple<Arg...> data;
    std::set<std::size_t> list_to_delete;

    std::size_t size=0;
    std::size_t size_visible=0;

    std::size_t cout_call=0;
    bool change_size=false;
public:
    template<typename Info_environment>
    using Data=Data_herald<Memory,Info_environment>;

    static constexpr int type_draw(){return type_draw_;}
    static constexpr int type_object(){return type_object_;}


    template<typename T,typename Info_environment>
    Data_herald<Memory,Info_environment> & fun_mem_set(Data_herald<Memory,Info_environment> &m){
        m.res=&res;
        return m;
    }


    template< typename Info_environment>
    Storage_data(Data_herald<Memory,Info_environment> herald):data(fun_mem_set<Arg>(herald)...){
        std::cout<<"|:::"<<herald.max_size<<std::endl;
    }

    Storage_data()=delete;
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
        res.new_frame();
        std::cout<<list_to_delete.size()+size<<"|"<<"ssssssssssssssssssssssss"<<std::endl;
    }

    Storage_data(const Storage_data& )=delete;
    Storage_data(Storage_data&& )=delete;
    Storage_data & operator=(Storage_data &&)=delete;
    Storage_data & operator=(const Storage_data &)=delete;

//    TYPE_INCLUDE is_get_job() const{
//        return include;
//    }

//    void on_job(){
//        include=TYPE_INCLUDE::ON;
//    }

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
        //std::cout<<"id <<|<<"<<id<<std::endl;
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
        res.new_frame();
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


//    template<typename Type>
//    inline auto * get_element_not_const(const std::size_t id){
//        cout_call++;
//        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
//        return (*get)->get_element_not_const(id);
//    }

//    template<typename Type>
//    constexpr auto * get_element_not_const() {
//        cout_call++;
//        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
//        return get->get_element_not_const();
//    }

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
    template<typename Type,typename ...Arg_>
    auto read_data(Arg_ ...arg_) const{
        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
        return get->read_data(std::forward<Arg_>(arg_)...);
    }
};



template<TIME_BIND bind_time,typename T,typename J=T>
class Data_ABC{
public:
    Data_ABC(){}

    Data_ABC(const Data_ABC& )=delete;
    Data_ABC(Data_ABC&& )=delete;
    Data_ABC & operator=(Data_ABC &&)=delete;
    Data_ABC & operator=(const Data_ABC &)=delete;

    static T Type();
    static J Type_gl();
    static constexpr TIME_BIND bind_time_(){return bind_time;}

    inline constexpr static TYPE_CONTAINER type_container=TYPE_CONTAINER::ARRAY;
    inline constexpr static TYPE_RES_STORAGE type_res_=TYPE_RES_STORAGE::LOCAL;
public:
    inline constexpr static TRIVIAL_DELETE trivial_delete=TRIVIAL_DELETE::YES;
    template<typename Memory, int type_object_,int type_draw_,typename ...Arg>
    friend class Storage_data;

//    virtual std::size_t add_element(T &&element, std::size_t id)=0;
//    virtual bool change(T&&element, std::size_t id)=0;
//    virtual bool delete_element(std::size_t id)=0;
//    virtual void bind(std::size_t id)=0;
    //virtual auto get_element(std::size_t id)=0;
};



template<typename T,typename D>
class Change_merge{
    T * controller;
    std::ptrdiff_t id=-1;
public:
    D * data;
    Change_merge(std::ptrdiff_t id_,T *t,D *data_):id(id_),controller(t),data(data_){}

    inline auto confirm(){
        return controller->merg.add(id);
    }
};

template<typename T,typename D>
class Change{
    T * controller;
    std::ptrdiff_t id=-1;
public:
    D * data;
    Change(std::ptrdiff_t id_,T *t,D *data_):id(id_),controller(t),data(data_){}

    inline auto confirm(){
        return controller->change_confirm(id);
    }
};

template<typename T,typename D>
class All_change{
    T * controller;
public:
    D * data;
    All_change(T *t,D *data_):controller(t),data(data_){}

    inline auto confirm(){
        return controller->change_confirm(0,data->size());
    }
};


template<typename T>
class Change_guard{
    T change;
public:
    Change_guard(T &&t):change(std::forward<T>(t)){}

    template<typename ...Arg>
    Change_guard(Arg...arg):change(arg...){}

    inline auto * get_data(){
        return change.data;
    }

    ~Change_guard(){
        change.confirm();
    }
};

template<TIME_BIND bind_time,int bind_base,int type,int type_buffer,template<typename K, uint  L> class Buffer,typename T,typename J=T,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
class Data_gl:public Data_ABC<bind_time,T,J>{
    std::vector<T> RAM;
    Buffer<J,type_buffer> SSBO_VRAM;
public:
    inline static constexpr TRIVIAL_DELETE trivial_delete=t_delete;
    Merge_controller merg;

    template<typename Memory, typename Info_environment>
    Data_gl(Data_herald<Memory,Info_environment> herald):SSBO_VRAM(bind_base,type,herald.max_size){}

    inline bool new_frame(){
        int b=false;
        auto t=merg.pop_data();
        for(auto &[ID,S]:t){
            change_confirm(ID,S);
            b+=true;
        }
        return b;
    }

    std::size_t add_element(std::size_t id,T &&element){
        if(RAM.size()<=id){
            RAM.resize(id+1);
        }

        RAM[id]=std::move(element);
        if constexpr(bind_time!=TIME_BIND::NEVER){
            if constexpr(fun!=nullptr){
                auto t=fun(RAM[id]);
                SSBO_VRAM.insert(id,&t);
            }else{
                SSBO_VRAM.insert(id,&RAM[id]);
            }
        }
        return id;
    }

    inline bool change(std::size_t id, T&&element){
        RAM[id]=element;
        return  change_confirm(id);
    }

    inline auto change_request(std::size_t id){
        return Change_guard<decltype (Change_merge(id,this,&RAM[id]))>(id,this,&RAM[id]);
    }

    inline auto change_request(){
        return Change_guard<decltype (All_change(this,&RAM))>(this,&RAM);
    }

    inline bool change_confirm(std::size_t id){
        if constexpr(bind_time!=TIME_BIND::NEVER){
            if constexpr(fun!=nullptr){
                auto t=fun(RAM[id]);
                SSBO_VRAM.changes(&t,id,id+1);
            }else{
                SSBO_VRAM.changes(&RAM[id],id,id+1);
            }
        }
        return true;
    }

    inline bool change_confirm(std::size_t id, std::size_t size){
        if constexpr(bind_time!=TIME_BIND::NEVER){
            if constexpr(fun!=nullptr){
                std::vector<J> t;
                for(std::size_t i=0;i<size;i++){
                    t.emplace_back(fun(RAM[id+i]));
                }
                SSBO_VRAM.changes(t.data(),id,id+size);
            }else{
                SSBO_VRAM.changes(&RAM[id],id,id+size);
            }
        }
        return true;
    }


    inline bool delete_element(std::size_t id){


// так надо, оптимизация, при удаление нечего тут не делаем)
        return true;
    }

    inline void bind(int id) const{}

    inline void bind() const{
        if constexpr(bind_time!=TIME_BIND::NEVER){
            SSBO_VRAM.Bind(bind_base);
        }
    }

    const auto * get_element(std::size_t id) const{
        return &RAM[id];
    }

    auto * get_element_not_const(std::size_t id){
        return &RAM[id];
    }

    inline bool read_data(int begin,int end,std::vector<T> &data) const{
        SSBO_VRAM.read_data(begin,end,data);
        return true;
    }
};

template<TIME_BIND bind_time,int bind_base,int type,int type_buffer,template<typename K, uint  L> class Buffer,typename T,typename J=T,J fun(T&)=nullptr>
class Data_gl_one:public Data_ABC<bind_time,T,J>{
    T RAM;
    Buffer<J,type_buffer> SSBO_VRAM;
public:
    inline static constexpr TYPE_CONTAINER type_container=TYPE_CONTAINER::ONE;

    template<typename Memory, typename Info_environment>
    Data_gl_one(Data_herald<Memory,Info_environment> herald):SSBO_VRAM(bind_base,type,1){}


    bool emplace(T&&element){
        RAM=element;
        if constexpr(bind_time!=TIME_BIND::NEVER){
            if constexpr(fun!=nullptr){
                auto t=fun(RAM);
                SSBO_VRAM.changes(&t,0,1);
            }else{
                SSBO_VRAM.changes(&RAM,0,1);
            }
        }
        return true;
    }

    bool earse(){
//        RAM=nullptr;
//        if constexpr(bind_time!=NEVER){
//            SSBO_VRAM.excl0.ude(0,1);
//        }
        return true;
    }

    inline void bind(int id) const{}

    inline void bind() const{
        if constexpr(bind_time!=TIME_BIND::NEVER){
            SSBO_VRAM.Bind(bind_base);
        }
    }
    constexpr auto * get_element(int id=0) const{
        return RAM;
    }

    inline bool read_data(int begin,int end,std::vector<T> &data)const{
        SSBO_VRAM.read_data(begin,end,data);
        return true;
    }
};


struct Type_array{
    int begin=0;//из-за GLSL
    int max_size=0;//из-за GLSL
};

template<TIME_BIND bind_time,int bind_base_aloc,int bind_base,int type,int type_buffer,template<typename K, uint  L> class Buffer,typename T,typename J=typename T::value_type,J fun(T&)=nullptr,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
class Data_gl_array:public Data_ABC<bind_time,T,J>{
    std::vector<std::pair<Type_array,T>> RAM;
    Allocator_free_space memory_controller;

    Buffer<J, type_buffer> SSBO_VRAM;
    Buffer<Type_array, type_buffer> SSBO_VRAM_allocate;
public:
    inline static constexpr TRIVIAL_DELETE trivial_delete=t_delete;

    template<typename Memory, typename Info_environment>
    Data_gl_array(Data_herald<Memory,Info_environment> herald):
        SSBO_VRAM(bind_base,type,herald.max_size),SSBO_VRAM_allocate(bind_base_aloc,type,herald.max_size),memory_controller(herald.max_size){}

    inline bool new_frame(){return  false;}

    std::size_t get_begin_of_controller(std::size_t size_max_text){
        auto b=memory_controller.pull_space(size_max_text);

        while(b.command!=0){
            auto k=SSBO_VRAM.get_max_size();
            SSBO_VRAM.expansion_buffer(k*1.5+size_max_text);
            memory_controller.expansion(k*0.5+size_max_text);
            b=memory_controller.pull_space(size_max_text);
        }
        return b.begin;
    }

    //element -array/vector
    auto add_element(std::size_t id,T &&element,std::size_t size_max_text_){
        if(RAM.size()<=id){
            RAM.resize(id+1);
        }

        std::size_t size_max_text=size_max_text_;
        //защита от странного ввода
        if(element.size()>size_max_text){
            size_max_text=element.size();
        }
        //расчёт оптимальное местоположения
        auto begin=get_begin_of_controller(size_max_text);
        //добавление элемента

        //сделать проверку на переполнение int или изменить тип на GPU
        RAM[id]={{static_cast<int>(begin),static_cast<int>(size_max_text)},element};

        if constexpr(bind_time!=TIME_BIND::NEVER){
            if constexpr(fun!=nullptr){
                auto t=fun(RAM[id].second);
                SSBO_VRAM.insert(begin,&t,t.size(),0);
            }else{
                SSBO_VRAM.insert(begin,RAM[id].second.data(),RAM[id].second.size());
            }

            SSBO_VRAM_allocate.insert(id,&RAM[id].first);
        }
        return begin;
    }


    inline auto change(std::size_t id, T&&element){
        RAM[id].second=element;
        return change_confirm(id);
    }

    auto change_request(std::size_t id){
        return Change_guard<decltype (Change(static_cast<std::ptrdiff_t>(id),this,&RAM[id].second))>(static_cast<std::ptrdiff_t>(id),this,&RAM[id].second);
    }

    auto change_confirm(std::size_t id){
        auto [begin,max_size]=RAM[id].first;
        std::size_t size_element=RAM[id].second.size();
        std::size_t size_max_text=max_size;

        if(size_element>max_size){
            size_max_text=size_element;
            memory_controller.push(begin,max_size);
            begin=get_begin_of_controller(size_max_text);

            //сделать проверку на переполнение int или изменить тип на GPU
            RAM[id].first={static_cast<int>(begin),static_cast<int>(size_max_text)};
        }
        //std::cout<<size_element<<"|"<<size_max_text<<std::endl;

        if constexpr(bind_time!=TIME_BIND::NEVER){
            if constexpr(fun!=nullptr){
                auto t=fun(RAM[id].second);
                SSBO_VRAM.changes(t.data(),begin,t.size()+begin);
            }else{
                SSBO_VRAM.changes(RAM[id].second.data(),begin,RAM[id].second.size()+begin);
            }

            SSBO_VRAM_allocate.changes(&RAM[id].first,id,id+1);
        }
        return true;
    }

    inline bool read_data(int begin,int end,std::vector<T> &data)const{
        SSBO_VRAM.read_data(begin,end,data);
        return true;
    }

    bool delete_element(std::size_t id){
        if constexpr(bind_time!=TIME_BIND::NEVER){
            if(RAM.size()<=id){
                //std::cout<<RAM.size()<<"text|"<<id<<std::endl;
                return true;
            }

            if(id>=1150){
                std::cout<<id<<"|>"<<RAM[id].first.begin<<"|"<<RAM[id].first.max_size<<std::endl;
            }

            memory_controller.push(RAM[id].first.begin,RAM[id].first.max_size);
            RAM[id].first.max_size=0;
        }
        return true;
    }

    inline void bind(int id) const{}
    inline void bind() const{
        if constexpr(bind_time!=TIME_BIND::NEVER){
            SSBO_VRAM.Bind(bind_base);
            SSBO_VRAM_allocate.Bind(bind_base_aloc);
        }
    }
    const auto * get_element(std::size_t id) const{
        return &RAM[id];
    }


    auto * get_element_not_const(std::size_t id) const{
        return &RAM[id];
    }
};

template<TIME_BIND bind_time,int bind_base,int type,int type_buffer,template<typename K, uint  L> class Buffer,typename T,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
class Data_gl_array_no_ram_no_allocate:public Data_ABC<bind_time,T,typename T::value_type>{
    Allocator_free_space memory_controller;
    Buffer<typename T::value_type, type_buffer> SSBO_VRAM;

    std::vector<Type_array> vram_position;
public:
    inline static constexpr TRIVIAL_DELETE trivial_delete=t_delete;

    template<typename Memory, typename Info_environment>
    Data_gl_array_no_ram_no_allocate(Data_herald<Memory,Info_environment> herald):
        SSBO_VRAM(bind_base,type,herald.max_size),memory_controller(herald.max_size){}

    inline bool new_frame(){return  false;}

    std::size_t get_begin_of_controller(std::size_t size_max_text){
        auto b=memory_controller.pull_space(size_max_text);

        while(b.command!=0){
            auto k=SSBO_VRAM.get_max_size();
            SSBO_VRAM.expansion_buffer(k*1.5+size_max_text);
            memory_controller.expansion(k*0.5+size_max_text);
            b=memory_controller.pull_space(size_max_text);
        }
        return b.begin;
    }

    //element -array/vector
    auto add_element(std::size_t id,T &&element){
        if(vram_position.size()<=id){
            vram_position.resize(id+1);
        }
        //расчёт оптимальное местоположения
        auto begin=get_begin_of_controller(element.size());

        vram_position[id]={static_cast<int>(begin),static_cast<int>(element.size())};
        //добавление элемента

        SSBO_VRAM.insert(begin,element.data(),element.size());

        return begin;
    }

    inline auto change(std::size_t id, T&&element){
        auto [begin,max_size]=vram_position[id];

        memory_controller.push(begin,max_size);
        begin=get_begin_of_controller(element.size());
        vram_position[id]={begin,element.size()};


        SSBO_VRAM.changes(element.data(),begin,element.size()+begin);

        return true;

    }

    inline bool read_data(int begin,int end,std::vector<T> &data)const{
        SSBO_VRAM.read_data(begin,end,data);
        return true;
    }

    bool delete_element(std::size_t id){
        if(vram_position.size()<=id){
            return true;
        }
        std::cout<<"||||||||||||||||||||||delete_element|||||||||||||:"<<std::endl;
        auto [begin,max_size]=vram_position[id];
        memory_controller.push(begin,max_size);

        return true;
    }

    inline void bind(int id) const{}
    inline void bind() const{
        if constexpr(bind_time!=TIME_BIND::NEVER){
            SSBO_VRAM.Bind(bind_base);
        }
    }
    inline void Bind() const{//более приорететный вызов для удобства API
        SSBO_VRAM.Bind(bind_base);
    }

};


template<TIME_BIND bind_time,typename S,typename T,int bind_base=-1>
class Data_res:public Data_ABC<bind_time,T>{
    std::vector<T> RES{nullptr};//странная дичь
    S * storage_res_;
public:
    inline constexpr static TYPE_RES_STORAGE type_res_=TYPE_RES_STORAGE::GLOBAL;
    inline bool new_frame(){return  false;}

    template<typename Memory, typename Info_environment>
    Data_res(Data_herald<Memory,Info_environment> herald):storage_res_(herald.res){}

    template<typename ...Arg>
    std::size_t add_element(std::size_t id, Arg...arg){
        if(RES.size()<=id){
            RES.resize(id+1,nullptr);
        }
        //std::cout<<id<<"|*********************|"<<RES.size()<<std::endl;

        RES[id]=storage_res_->template add_data<T>(std::forward<Arg>(arg)...);
        //хммммммм//storage_res_->add_task(RES[id],Type_Status::VISEBLE);
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


template<TIME_BIND bind_time,int bind_base,int type,int type_buffer,template<typename K, uint  L> class Buffer,typename T,TRIVIAL_DELETE t_delete=TRIVIAL_DELETE::YES>
class Data_gl_read_no_ram:public Data_ABC<bind_time,T,T>{
    mutable Buffer<T,type_buffer> SSBO_VRAM;
public:
    inline static constexpr TRIVIAL_DELETE trivial_delete=t_delete;

    inline bool new_frame(){return  false;}

    template<typename Memory, typename Info_environment>
    Data_gl_read_no_ram(Data_herald<Memory,Info_environment> herald):SSBO_VRAM(bind_base,type,0){}

    inline void bind(int id) const{}

    inline void bind() const{
        SSBO_VRAM.Bind(bind_base);
    }

    inline bool read_data(int begin,int end,std::vector<T> &data) const{
        SSBO_VRAM.read_data(begin,end,data);
        return true;
    }


    auto change(int begin, T &&element){
        SSBO_VRAM.changes(&element,begin,begin+1);
        return true;
    }

    auto add_element(std::size_t id,std::vector<T> &&element){
        SSBO_VRAM.changes(element.data(),id,element.size());
    }

    bool delete_element(std::size_t id){
        return true;
    }
    inline T read_data(int pos) const{
        return SSBO_VRAM.read_data(pos);
    }
    constexpr auto * get_element() {
        return &SSBO_VRAM;
    }
};



//template<typename T,typename ...Arg>
//class Data_tuple{
//    T controller;
//    std::tuple<Arg...> data;
//public:
//    //static decltype (decltype (get_first_type<Arg...>::value)::Type()) Type();
//    static T Type();
//    Data_tuple(std::size_t max_size):data(max_size,max_size){};

//    template<typename ...Arg_>
//    std::size_t add_element(std::size_t id,Arg_...arg_){
//        controller.add_data(this,id,arg_...);
//        return id;
//    };
//    void bind(std::size_t id=-1){
//         tutl::TupleForeach(data,[](auto &t){t.bind();});
//    }

//    bool delete_element(std::size_t id){
//        controller.delete_element(this,id);
//        tutl::TupleForeach(data,[](auto &t){t.delete_element();});
//    }

//    template<typename ...Arg_>
//    bool change(std::size_t id,Arg_ ...arg_){
//        controller.change(this,id,arg_...);
//    }

//    template<typename Type,typename ...Arg_>
//    auto & get_element(){
//        return tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
//    }

//};



///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///
///
///
///
///
///
///
///
///
///
///
///
///
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//template<typename Fream,typename ...Arg>
//class Fream_data{
//    Fream fream;
//    std::tuple<Arg...> data;

//    int * weight_=0;
//    int * height_=0;

//public:

//    Fream_data(int *weight,int *height):weight_(weight),height_(height){
//        tutl::TupleForeach(data,[this](auto &t){t={*weight_,*height_};});
//        bind_layer();
//    }


//    template<TIME_BIND time_bind>
//     static constexpr bool check_bind_time(){
//        int i=0;
//        (((Arg::bind_time_==time_bind)?i++:false),...);
//        return bool(i);
//    };


//    template<TIME_BIND time_bind>
//    void bind_layer(std::size_t id) const{
//        if constexpr(check_bind_time<time_bind>()){
//            tutl::TupleForeach(data,[id](auto &t){
//                if constexpr(t.bind_time_==time_bind){
//                   t.attach(id);
//                }
//            });
//        }
//    }

//    template<typename Type>
//    constexpr Type * get_element() const{
//        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
//        return get->get_element();
//    }

//    template<typename Type>
//    static constexpr bool check_type() {
//        return tutl::isGet_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>();
//    }

//    template<typename Type,typename T_T,typename ...Arg_>
//    bool change(std::size_t id, T_T &&change, Arg_ ...arg_){
//        auto get=tutl::Get_tuple_type_element<Type,std::tuple<decltype (Arg::Type())...>>(data);
//        return get->change(id,std::forward<T_T>(change),std::forward<Arg_>(arg_)...);
//    }


//};

#endif // INFORMATION_RENDERING_H
