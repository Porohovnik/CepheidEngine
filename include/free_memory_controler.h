#ifndef FREE_MEMORY_CONTROLER_H
#define FREE_MEMORY_CONTROLER_H
#include <map>
#include <set>
#include <cstdint>

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


class Allocator_id{//подумать об оптимизации места
    using End=std::size_t;
    using Size=std::size_t;

    std::map<End,Size> data;
    //Edn size
    //4     5=>0
    //10    4=>7
    //14    4=>11

    Size size=0;
public:
    void add(std::size_t id){//O(log(N)
        auto begin_id=data.extract(id-1);

        if(!begin_id.empty()){

            begin_id.mapped()++;
            begin_id.key()++;

            data.insert(std::move(begin_id));
        }else{
            data.insert({id,1});
        }
        size++;
    }

    inline std::map<End,Size> * get_data(){
        return &data;
    }

    inline Size get_size()const noexcept{
        return size;
    }

    inline void merge(Allocator_id * al){
        data.insert(al->get_data()->begin(),al->get_data()->end());
        size+=al->get_size();
    }

    void earse(std::size_t id){//O(log(N)
        auto begin_id=data.lower_bound(id);
        if(begin_id!=data.end()){
            auto t=data.extract(begin_id->first);
            if(begin_id->first==id){
                t.mapped()--;
                t.key()--;
                data.insert(std::move(t));
            }else {
                //при удаление просто разделяем диопазон на две части
                data.insert({t.key(),t.key()-id});
                data.insert({id-1,t.mapped()-1-(t.key()-id)});
            }
            size--;
        }
    }

    void clear(){
        data.clear();
    }

//    void print()const{
//        std::cout<<"|size:"<<size<<std::endl;
//        for(auto &[T,S]:data){
//            std::cout<<T<<":"<<S<<std::endl;
//        }
//    }
};

#include <vector>
class Merge_controller{
    std::set<std::size_t> data;

    using Begin=std::ptrdiff_t;
    using Size=std::size_t;

public:
    void add(std::size_t id){
        data.insert(id);
    }
    void merge(std::set<std::size_t> & set){
        data.merge(set);
    }
    std::vector<std::pair<Begin,Size>> pop_data(){
        std::vector<std::pair<Begin,Size>> t;
        for(auto T:data){
            if(t.size()==0){
                t.push_back(std::pair{T,1});
                continue;
            }
            if(t.back().first+t.back().second==T){
                t.back().second++;
            }else {
                t.push_back(std::pair{T,1});
            }
        }
        data.clear();
        return t;
    }
};

#endif // FREE_MEMORY_CONTROLER_H
