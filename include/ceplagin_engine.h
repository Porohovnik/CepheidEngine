#ifndef CEPLAGIN_ENGINE_H
#define CEPLAGIN_ENGINE_H

#ifdef WIN32
    #include <windows.h>
    #include "winsock2.h"
#else
#include <dlfcn.h>
#endif


#include <cassert>
#include <iostream>
#include <filesystem>

class CePlagin_engine{
    void * lib=nullptr;

public:
    CePlagin_engine(std::filesystem::path path){
        #ifdef WIN32
           lib = LoadLibrary(path.string().c_str());
           std::cout<<path.string().c_str()<<"||||||:"<<lib<<std::endl;
        #else
           lib = dlopen(path.string().c_str(), RTLD_LAZY);
        #endif
    }

    template<typename Return,typename ...Arg>
    Return method(std::string name_method,Arg...arg){
        if(lib==nullptr){
            std::cout<<"|assert(lib!=nullptr);"<<std::endl;
        }
        assert(lib!=nullptr);
    #ifdef WIN32
        return reinterpret_cast<Return (*)(Arg...)>(GetProcAddress(reinterpret_cast<HINSTANCE>(lib),name_method.data()))(arg...);
    #else
       return reinterpret_cast<Return (*)(Arg...)>(dlsym(lib,name_method.data()))(arg...);
    #endif
    }
    template<typename Return,typename ...Arg>
    auto get_fun(std::string name_method){
        #ifdef WIN32
            return std::function<Return(Arg...)>{reinterpret_cast<Return (*)(Arg...)>(GetProcAddress(reinterpret_cast<HINSTANCE>(lib),name_method.data()))};
        #else
            return std::function<Return(Arg...)>{reinterpret_cast<Return (*)(Arg...)>(dlsym(lib,name_method.data()))};
        #endif
    }

    ~CePlagin_engine(){
        std::cout<<"aaaaaaaaaaaaaaaaaaaaaaaa"<<std::endl;
    #ifdef WIN32
       FreeLibrary(reinterpret_cast<HINSTANCE>(lib));
    #else
       dlclose(lib);
    #endif
    }

};

class CePlagin_menedger{
    std::unordered_map<std::string,CePlagin_engine> shared_modul;
public:
    CePlagin_engine * get_Shared_modul(std::string name){
       if(shared_modul.find(name)==shared_modul.end()){
               return nullptr;
       }else {
           return &shared_modul.find(name)->second;
       }
    }

    CePlagin_engine * add_Shared_modul(std::filesystem::path path,std::string name=""){
         if(name==""){
            name=path.string();
         }

         shared_modul.try_emplace(name,path);
         return get_Shared_modul(name);
    }
};

#endif // CEPLAGIN_ENGINE_H
