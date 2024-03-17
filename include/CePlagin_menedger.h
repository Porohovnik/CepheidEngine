#ifndef CEPLAGIN_MENEDGER_H
#define CEPLAGIN_MENEDGER_H
#include <unordered_map>
#include "ceplugin_engine.h"
namespace CeEngine {
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
}// namespace CeEngine
#endif // CEPLAGIN_MENEDGER_H
