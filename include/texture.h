#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>

#include "Buffer_id_texture.h"
#include "interlayer_gl.h"

namespace CeEngine {
class Texture{
protected:
    GL_layer::Buffer_id_texture Textur;
public:
    template<typename Image>
    Texture(Image image):Textur(GL_layer::get_value_GL(image.get_POINT_TEX())){
        //std::cout<<image.width<<" : "<<image.heights<<"|"<<std::endl;

        if(image.get_setting().cout_image==1){
            Textur.TexImage2D(image);
        }

        if(image.get_setting().cout_image==6){
            Textur.TexImage2D_cube(image);
        }

        Textur.set_atribute_texture_buffer(image.get_setting().filter_atribute);
        if(image.get_setting().mipmap){
            Textur.generate_mipmap();
        }
    }

    void Bind(int base, int id=-1) const{
        Textur.active_texture(base);
        Textur.Bind_();
        //std::cout<<GL_layer::Get_Error()<<"/-Texture-/"<<id<<"///::"<<base<<std::endl;
    }
};
}// namespace CeEngine
#undef STB_IMAGE_IMPLEMENTATION
#endif // TEXTURE_H
