#ifndef CEOBJECT_FABRIC_H
#define CEOBJECT_FABRIC_H
#include <map>
#include <functional>
//В дополнения

template<typename T,typename Key,typename ...Arg>
class CeObject_fabric{
    using Fun_sign=std::function<T(Arg...)>;

    std::map<Key,Fun_sign> object_type_storage;
public:
    template<typename F>
    void add_type(Key key,F&&fun){
        object_type_storage[std::forward<Key>(key)]=fun;
    }
    T call_fun(Key && key,Arg...arg){
        return object_type_storage[key](arg...);
    }

    inline bool is_type(Key key){
        if(object_type_storage.find(key)==object_type_storage.end()){
            return false;
        }
        return true;
    }

    CeObject_fabric(){}
};


#endif // CEOBJECT_FABRIC_H
