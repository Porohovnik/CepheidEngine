#ifndef RESOURCE_ENGINE_H
#define RESOURCE_ENGINE_H
#include <unordered_map>
#include <memory>

#include "celist.h"
namespace CeEngine {
template <typename Context_storage, typename ...Storage>
class Resource_engine{
public:
    using List=CeList<Storage...>;
private:
    std::tuple<Storage...> resourse;
    Context_storage win_storage;

    tutl::Type_to_data<tutl::TAAT<Storage,std::list<std::list<Identification *> *>>...> visible_object;
    bool change_status_visual=false;

    std::unordered_map<std::string,List> list_array;
public:

    auto & Get_Data() const{
        return resourse;
    }

    auto * Get_Win_storage() const{
        return &win_storage;
    }

    std::size_t Get_cout_call() {//господи, 4-8 сложения на цикл, можно особо и не экономить)
        std::size_t cout_call=win_storage.get_cout_call();
        //std::size_t cout_call=0;

        tutl::TupleForeach(resourse,[&cout_call](auto &t){
            cout_call+=t.get_cout_call();
        });

        return cout_call;
    }

    List * create_list(std::string name_list){
        //благодаря этой функции, включение-отключение листа происходят за О(1)
        auto m=[this](tutl::Type_to_data<tutl::TAAT<Storage, std::list<Identification *>>...> * T){
            std::shared_ptr<tutl::Type_to_data<tutl::TAAT<Storage,std::list<std::list<Identification *> * >::iterator>...>> it_all;
            it_all=std::make_shared<tutl::Type_to_data<tutl::TAAT<Storage,std::list<std::list<Identification *> * >::iterator>...>>();

            tutl::TupleForeach(resourse,[this,T,it_all](auto &t){
                auto &g=T->template get_element_data<decltype (t)>();
                //добавляем лист в отображение
                auto it = visible_object.template get_element_data<decltype (t)>().insert(visible_object.template get_element_data<decltype (t)>().end(),&g);
                //получаем интератор для его быстрого исключения
                it_all->template get_element<decltype (t)>()->data.emplace(it);
            });

            //создаём функцию, которая отключит лист из отображение
            return [this,it_all](){
                tutl::TupleForeach(resourse,[this,it_all](auto &t){
                    auto g=it_all->template get_element_data<decltype (t)>();
                    visible_object.template get_element_data<decltype (t)>().erase(g);
                });
            };
        };

        list_array.try_emplace(name_list,&change_status_visual,m,tutl::Get_tuple_type_element_t<Storage>(resourse)...);

        return get_list(name_list);
    }

    List * get_list(std::string name_list){
        if(list_array.find(name_list)==list_array.end()){
            return create_list(name_list);
        }
        return &list_array.find(name_list)->second;
    }

    void status_change_all(Type_Status new_status_){
        for (auto &[K,L]:list_array) {
            L.set_status(new_status_);
        }
    }


    void new_frame(){
        win_storage.new_frame();
        tutl::TupleForeach(resourse,[this](auto &t){
            //собирается все изменения за кадр и применяются, дабы не размусоливать
            t.new_frame();
            t.visible_data(&visible_object.template get_element_data<decltype (t)>(),t.isChange_size() || change_status_visual);
        });

        change_status_visual=false;
    }


    template<typename Color ,typename Info_environment>
    Resource_engine(Color background_color_,Info_environment * info_):
        resourse   (tutl::repeater_fun<Storage>(info_)...),win_storage(background_color_,info_){
        tutl::TupleForeach(visible_object.get_data(),[](auto &t){t.data.emplace();});
        //std::cout<<"||0||"<<std::endl;
    }

};
}// namespace CeEngine
#endif // RESOURCE_ENGINE_H
