#ifndef CELIST_ENGINE_H
#define CELIST_ENGINE_H
#include "celist.h"
#include <memory>

template<typename Object,typename List>
class CeList_engine{
using Obj_ptr=std::shared_ptr<Object>;

    std::unordered_map<std::string, List> lists_storage;
public:
    CeList_engine();

    void new_frame(){

    }

    List * create_list(std::string name_list){
        lists_storage.try_emplace(name_list);
        return get_list(name_list);
    }

    List * get_list(std::string name_list){
        return &lists_storage[name_list];
    }
};

#endif // CELIST_ENGINE_H
