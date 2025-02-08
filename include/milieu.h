#ifndef MILIEU_H
#define MILIEU_H

#include <string>
#include <iostream>
#include <unordered_map>

#include "context.h"
#include "ceplagin_engine.h"

template<class Object_,
         typename Memory,
         template<typename K, uint  L> class Buffer,
         template<typename M,template<typename T__,uint n__> class B> class Context_storage,
         template<typename M,template<typename T_,uint n_> class B> class ...Storage>
class Milieu{
public:
    using Context=Context<Object_,Context_storage<Memory,Buffer>, Storage<Memory,Buffer>...>;
private:
    std::unordered_map<std::string,Context> contexts;
public:
    CePlagin_menedger plagin_menedger;

    Milieu(){
        std::system("chcp");//хак 1 - utf8
        // Init GLFW
        if(glfwInit()){
            std::cout << "Starting GLFW context, OpenGL "<< std::endl;
        };

//        if(sizeof (wchar_t)==2){
//            std::setlocale(LC_ALL,"Russian");
//            std::setlocale(LC_ALL,"ru_RU.UTF8");
//        }else{
//            std::setlocale(LC_ALL,"ru_RU.UTF8");
//        }
    }

     Context * get_context(std::string name_okno){
        if(contexts.find(name_okno)==contexts.end()){
            if(contexts.size()!=0){
                return &contexts.begin()->second;
            }else {
                return nullptr;
            }
        }else {
            return &contexts.find(name_okno)->second;
        }
    }

    Context * insert_context(std::string name, int WIDTH, int HEIGHT, glm::vec4 background_color){
        return &(contexts.try_emplace(name,name,WIDTH,HEIGHT,background_color).first->second);
    }

    Context * operator[](std::string name_okno){
        return get_context(name_okno);
    }

    void DISPLAY(){
        for (bool job=1;job;) {
            job=false;
            for(auto &[K,C]:contexts){
                if(C.isOpen()){
                   C.new_fraem();
                   job=true;
                }
            }
        }
        std::cout<<"|exit"<<std::endl;
    }
};

#endif // MILIEU_H
