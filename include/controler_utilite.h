#ifndef CONTROLER_UTILITE_H
#define CONTROLER_UTILITE_H
#include <list>

#include "position.h"
#include "Identification.h"

#define BINDLES_TEXTURE
#define GL_DRAW_LINE
using namespace  CeEngine;

inline static int id_global=1;

#ifdef BINDLES_TEXTURE
const std::string Directory_shaders="shaders/";
#else
#ifdef GL_DRAW_LINE
const std::string Directory_shaders="shaders_noBindless/";
#else
const std::string Directory_shaders="shaders_noBindless_noGL_DRAW/";
#endif
#endif

auto fooooo(Position &pos){
    return pos.pos();
}

template<typename CMD,typename CMD_m,typename T>
inline bool visible_data_controller_any(T * t,std::list<std::list<Identification *> *> * ids,bool update=false){
    //std::cout<<"||5.5||"<<std::endl;
    if(update){
        auto ch = t->template change_request<Identification>();
        auto cmd=t->template change_request<CMD>();
        auto cmd_manadger=t->template change_request<CMD_m>();

        std::size_t size=0;
        for(auto &I:*ids){
            size+=I->size();
        }

        t->set_visible_size(size);//хммм

        ch.get_data()->resize(t->get_element_size());
        cmd.get_data()->resize(t->get_element_size());

        auto l_b=t->get_delete_ids().begin();

        std::size_t k=0;
        for(auto &I:*ids){
            for(auto &ID:*I){
                int id=ID->get_id();
                (*ch.get_data())[k]=id;
                (*cmd.get_data())[k]=(*cmd_manadger.get_data())[id].cmd;
                (*cmd_manadger.get_data())[id].id=k;

                k++;
            }
        }
        std::cout<<k<<"|"<<t->get_element_size()<<"||||||||||||||||^^^^^^^^"<<t->get_delete_ids().size()<<std::endl;
        return true;
    }
    return false;
}

#endif // CONTROLER_UTILITE_H
