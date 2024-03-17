#ifndef MERGE_CONTROLLER_H
#define MERGE_CONTROLLER_H
#include <map>
namespace CeEngine {
class Merge_controller{
    using Begin=std::ptrdiff_t;
    using End=std::ptrdiff_t;
    using Size=std::size_t;

    std::map<End,Size> t;
public:
    void add(std::size_t id){
        End end =id+1;
        Begin begin=id;

        auto cursor=t.lower_bound(end);
        if(cursor==t.end() && t.size()){
            cursor--;
        }

        int k=2;
        while (!(!t.size())*k) {
            End end_k=cursor->first;
            Begin begin_k=cursor->first-cursor->second;
            bool isdelete=false;

            //перекрытие справа или слияние с справа
            if(end<=end_k && end >= begin_k){
                end=end_k;
                isdelete=true;
            }

            //перекрытие слево или слияние с слево
            if(begin <=end_k && begin>=begin_k){
                begin=begin_k;
                isdelete=true;
            }

            //поглощение в центре
            if(begin <begin_k && end>end_k){
                isdelete=true;
            }

            if(isdelete){
                cursor=t.erase(cursor);

                if(cursor!=t.begin() && cursor==t.end()){
                    cursor--;
                }
            }else {
                if(cursor==t.begin()){
                    break;
                }else {
                   cursor--;
                }
            }

            k--;
        }

        t[end]=end-begin;
    }

    std::map<End,Size> pop_data(){
        return std::move(t);
    }
};
}// namespace CeEngine
#endif // MERGE_CONTROLLER_H
