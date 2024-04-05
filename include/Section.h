#ifndef SECTION_H
#define SECTION_H
#include <optional>
#include <functional>
namespace CeEngine {
template<typename J,typename T>
class Section{
protected:
    std::function<J()> dowm_func=nullptr;
    std::optional<J>   RAM;
    std::optional<T>   VRAM;
public:

    template<typename ...Arg>
    Section(Arg && ...arg){
        dowm_func=[arg...](){return J(arg...);};
    }

    Section(J arg){
        dowm_func=[arg](){return arg;};
    }

    void to_RAM(){
        if(!RAM && !VRAM){
            RAM.emplace(dowm_func());
        }
    }
    void clear_RAM(){
        RAM.reset();
    }

    void to_VRAM(){
        if(RAM && !VRAM){
            VRAM.emplace(std::move(RAM.value()));
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

    T & Get_VRAM(){
        return VRAM.value();
    }
};
}// namespace CeEngine
#endif // SECTION_H
