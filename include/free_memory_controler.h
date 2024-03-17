#ifndef FREE_MEMORY_CONTROLER_H
#define FREE_MEMORY_CONTROLER_H
#include <map>
#include <set>
#include <cstdint>

namespace CeEngine {
inline constexpr int32_t EXPANSION=0X1;
inline constexpr int32_t NORMALAIZ=0X4;
inline constexpr int32_t NULLPTR_SIZE=0X8;


class Allocator_free_space{
    std::size_t max_size=0;
    std::size_t free_size=0;

    using Begin=std::size_t;
    using End=std::size_t;

    using Size=std::size_t;

   // bool optimal_merge=true;

    std::map<Size,std::set<End>>  free_memory_size_;
    std::map<End,Size>  free_memory_end;

    template<typename T>
    void decrease_area(Begin begin, T interator){
        Size last_size=interator->second;
        Size size_new=interator->first-begin;

        //удаление пустого места
        if(size_new==0){
            free_memory_end.erase(interator);
        } else {
            interator->second=size_new;
            free_memory_size_[size_new].insert(interator->first);
        }

        free_memory_size_[last_size].erase(interator->first);
        if(free_memory_size_[last_size].size()==0){
            free_memory_size_.erase(last_size);
        }
        free_size-=(last_size-size_new);
    }
public:

    struct T{
        Begin begin=0;
        char command=0;
    };


    bool expansion(Size size=0){
        push(max_size,size);
        max_size+=size;
        return true;
    }

    Allocator_free_space(Size max_size_=0):max_size(max_size_){
        push(0,max_size_);
    }

    T pull_space(Size size);
    void push(Begin begin, Size size);
};
}// namespace CeEngine

#endif // FREE_MEMORY_CONTROLER_H
