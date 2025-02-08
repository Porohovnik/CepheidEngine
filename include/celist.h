#ifndef CELIST_H
#define CELIST_H
#include "list"
#include "tuple_utl.h"

#include <Identification.h>
#include <CeEnumeration.h>
namespace CeEngine {
template <typename ...Storage>
class CeList{
    std::tuple<Storage *...> resourse_ptr;
    bool * change_status_vis;

    std::function<std::function<void()>(tutl::Type_to_data<tutl::TAAT<Storage, std::list<Identification *>>...> *)> add_vis;
    std::function<void()> delete_vis=nullptr;

    tutl::Type_to_data<tutl::TAAT<Storage, std::list<Identification *>>...>   data_to_list;
    Type_Status status=Type_Status::DISABLE;
public:
    CeList(bool * change_status_vis_, decltype (add_vis) add_vis_,Storage * ...  storage):
        change_status_vis(change_status_vis_),add_vis(add_vis_),resourse_ptr(storage...){
        tutl::TupleForeach(data_to_list.get_data(),[](auto &t){t.data.emplace();});
    }
    ~CeList(){
        if(delete_vis!=nullptr){
            delete_vis();
            delete_vis=nullptr;
        }
    }

    template<typename  Type>
    constexpr Type * get_element(){
        return *tutl::Get_tuple_type_element_t<Type *>(resourse_ptr);
    }

    template<typename  Type>
    auto attach_object(Identification * object){
         return data_to_list.template get_element_data<Type>().insert(data_to_list.template get_element_data<Type>().end(),object);
    }

    template<typename  Type, typename I>
    void reattach_object(I object){
         data_to_list.template get_element_data<Type>().erase(object);
    }

    void set_status(Type_Status new_status_){
        if(status==new_status_){
            return;
        }
        if(status==Type_Status::VISEBLE || new_status_==Type_Status::VISEBLE){
           *change_status_vis=true;
        }

        if(new_status_==Type_Status::VISEBLE && add_vis!=nullptr){
            delete_vis=add_vis(&data_to_list);
            //std::cout<<"|"<<std::endl;
        }

        if(status==Type_Status::VISEBLE && delete_vis!=nullptr){//раз бывший статус - отображение, значит следующий точно другой
            delete_vis();
            delete_vis=nullptr;
        }

        status=new_status_;
        tutl::TupleForeach(resourse_ptr,[this](auto t){
           t->new_status(status,&data_to_list.template get_element_data<decltype (*t)>());
        });
    }

    auto * get_data(){
        return &data_to_list;
    }

    Type_Status get_status() const{
        return status;
    }
};
}// namespace CeEngine
#endif // CELIST_H
