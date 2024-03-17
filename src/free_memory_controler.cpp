#include <free_memory_controler.h>
#include <vector>

//#include <iostream>
namespace CeEngine {
    Allocator_free_space::T Allocator_free_space::pull_space(Size size){
        T in;

        if(size==0){
            in.command |= NULLPTR_SIZE;
            return in;
        }

        if(size>max_size || free_size<size){
            in.command |= EXPANSION;
            return in;
        }

        auto begin_optional_size=free_memory_size_.lower_bound(size);

        if(begin_optional_size!=free_memory_size_.end()){
            //begin=end-size
            Begin b=*begin_optional_size->second.begin()-begin_optional_size->first;
            in.begin=static_cast<Begin>(b);
            //            begin,   end
            decrease_area(b+size, free_memory_end.find(*begin_optional_size->second.begin()));
        }else{
            in.command |=NORMALAIZ;
        }

        //std::cout<<free_size<<"|"<<size<<"<|>"<<in.begin<<std::endl;

        return in;
    }

    void Allocator_free_space::push(Begin begin, Size size){
        End end=begin+size;
        if(size==0){
            return;
        }

        auto cursor =free_memory_end.lower_bound(end);
        if(cursor==free_memory_end.end() && free_memory_end.size()){
            cursor--;
        }

        std::vector<decltype (cursor)> decreas_area_list;

        //проверка перекрытий,слияний и их учёт
        while (free_memory_end.size()) {
            End end_k=cursor->first;
            Begin begin_k=cursor->first-cursor->second;

            //std::cout<<end_k<<"|"<<begin_k<<"|"<<std::endl;

            //перекрытие справа или слияние с справа
            if(end<=end_k && end >= begin_k){
                end=end_k;
                decreas_area_list.emplace_back(cursor);
            }

            //перекрытие слева или слияние с слева
            if(begin <=end_k && begin>=begin_k){
                begin=begin_k;
                decreas_area_list.emplace_back(cursor);
            }

            //поглощение в центре
            if(begin <begin_k && end>end_k){
                decreas_area_list.emplace_back(cursor);
            }

            if(cursor==free_memory_end.begin()){
                break;
            }else {
               cursor--;
            }
        }
        size=end-begin;

        for (auto &I:decreas_area_list) {
           decrease_area(I->first,I);
        }

        //добавление пустого места
        free_memory_end[end]=size;
        free_memory_size_[size].insert(end);
        free_size+=size;

        //std::cout<<"push:"<<free_size<<"|"<<size<<"<|>"<<begin<<"|"<<end<<std::endl;
    }
}
