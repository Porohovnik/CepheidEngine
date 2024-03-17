#ifndef FONT_CONTROLLER_BINDLESS_H
#define FONT_CONTROLLER_BINDLESS_H
#include "font.h"
#include "Texture_controller_BINDLESS.h"

namespace CeEngine {
class Font_controller_BINDLESS:public Font_controller<Texture_controller_BINDLESS>{
public:
    template<typename D_glif,typename D_font,typename Data_font>
    Font_controller_BINDLESS(D_glif * data_st_glifs,D_font * data_st_texture,int id_bd,Data_font && data):
        Font_controller<Texture_controller_BINDLESS>(data_st_glifs,id_bd,std::move(data),data_st_texture,id_bd){}
};
}// namespace CeEngine
#endif // FONT_CONTROLLER_BINDLESS_H
