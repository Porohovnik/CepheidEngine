#ifndef FONT_H
#define FONT_H
#include "texture.h"
#include "glifs.h"
#include "buffer_gl.h"
#include <functional>

void converting_unicode_to_glyph_number(std::vector<int> &unicod);

template <typename TEX>
class Font_controller{
    std::function<void()> deleter=nullptr;
    std::optional<TEX>   tex_controller;
public:
    template<typename D_glif>
    Font_controller(D_glif * data_st_glifs, int id_bd,Glifs glifs,
        std::function<void(std::optional<TEX> *,Image && )> to_texture=[](auto * t, auto && image){ t->emplace(move(image));}){

        auto atlas=create_atlas_image(glifs.glif_images,Map_atlas);
        for(std::size_t i=0;i<atlas.second.size();i++){
            glifs.glif_infos[i].position=                 {atlas.second[i][0],atlas.second[i][1]};
            glifs.glif_infos[i].coordinate_transformation={atlas.second[i][2],atlas.second[i][3]};
        }

        data_st_glifs->add_element(id_bd,std::move(glifs.glif_infos),glifs.glif_infos.size());
        std::cout<<"|<-font->|"<<std::endl;

        deleter= std::bind([](auto * bd,int id_bd_){
            if(bd!=nullptr){
                bd->delete_element(id_bd_);
            }},data_st_glifs,id_bd);

        to_texture(&tex_controller,std::move(atlas.first));
    }

    inline void Bind(int base,int id=-1){
        if constexpr(std::is_same_v<TEX,Texture<GL_TEXTURE_2D>>()){
            tex_controller->Bind(0);
        }
    }

    ~Font_controller(){
        if(deleter!=nullptr){
            std::cout<<"||||||||||||||||||||||Font_controller|||||||||||||:"<<std::endl;
            deleter();
        }
    }
};

class Font_controller_BINDLESS:public Font_controller<Texture_controller_BINDLESS<GL_TEXTURE_2D>>{
public:
    template<typename D_glif,typename D_font>
    Font_controller_BINDLESS(D_glif * data_st_glifs,D_font * data_st_texture,int id_bd,Glifs && glifs):
        Font_controller<Texture_controller_BINDLESS<GL_TEXTURE_2D>>(data_st_glifs,id_bd,std::move(glifs),
            std::bind([](D_font * data_st_texture,int id_bd,auto * t, auto && image){ t->emplace(data_st_texture,id_bd,std::move(image));}
               ,data_st_texture,id_bd,std::_Placeholder<1>{},std::_Placeholder<2>{})){}
};


void string_to_unicod(const char * text_in, std::vector<int> *simbols_out);

#endif // FONT_H
