#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>
#include <string>
#include <vector>

#include "image.h"
#include "interlayer_gl.h"

template<int POINT_TEX=GL_TEXTURE_2D>
class Texture{
protected:
    GL_layer::Buffer_id<POINT_TEX> Textur{0};

public:

    int widths=0;
    int heights=0;

    const GLuint & texture(){return Textur;}

    Texture(std::array<Image,6> images){
        for (int i=0;i<6;i++){
            assert(images[i].image==nullptr);
        }
        Textur=GL_layer::create_texture_buffer<POINT_TEX> ();
        GL_layer::Bind_guard bind{Textur};

        widths=images[0].widths;
        heights=images[0].heights;


        GL_layer::TexImage2D<GL_TEXTURE_CUBE_MAP_POSITIVE_X>(images[0]);
        GL_layer::TexImage2D<GL_TEXTURE_CUBE_MAP_NEGATIVE_X>(images[1]);

        GL_layer::TexImage2D<GL_TEXTURE_CUBE_MAP_POSITIVE_Y>(images[2]);
        GL_layer::TexImage2D<GL_TEXTURE_CUBE_MAP_NEGATIVE_Y>(images[3]);

        GL_layer::TexImage2D<GL_TEXTURE_CUBE_MAP_POSITIVE_Z>(images[4]);
        GL_layer::TexImage2D<GL_TEXTURE_CUBE_MAP_NEGATIVE_Z>(images[5]);

        GL_layer::set_atribute_texture_buffer<POINT_TEX>(images[0].filter_atribute);
    }

    Texture(Image image):Textur(GL_layer::create_texture_buffer<POINT_TEX> ()){
        widths=image.widths;
        heights=image.heights;

        GL_layer::Bind_guard bind{Textur};
        GL_layer::set_atribute_texture_buffer<POINT_TEX>(image.filter_atribute);

        std::cout<<image.widths<<" : "<<image.heights<<"|"<<std::endl;

        GL_layer::TexImage2D<POINT_TEX>(image);
        GL_layer::generate_mipmap(image.mipmap);
    }

    ~Texture(){
        GL_layer::delete_texture_buffer(Textur);
    }


    void Bind(int base, int id=-1) const{
        GL_layer::active_texture(base);
        GL_layer::Bind_(Textur);
        std::cout<<glGetError()<<"/-Texture-/"<<id<<"///::"<<base<<std::endl;
    }

    void attach_to_freambuffer(GLuint bind_frame,GLuint bind_point){
        GL_layer::Bind_guard bind{Textur};//?
        GL_layer::attach_to_freambuffer_<POINT_TEX>(Textur,bind_frame,bind_point);
    }
    //в деструкторе удаление
};


//нужен механизм выгрузки данных с С VRAM , без удаления ресурсов
template<int POINT_TEX=GL_TEXTURE_2D>
class Texture_controller_BINDLESS:public Texture<POINT_TEX>{
    uint64_t id_texture=0;
public:
    template<typename D>
    Texture_controller_BINDLESS(D * data_st,int id_bd,Image && image):Texture<POINT_TEX>(image){
        id_texture=glGetTextureHandleARB(this->Textur.buffer);
        glMakeTextureHandleResidentARB(id_texture);
        assert(id_texture==0);
        std::cout<<glGetError()<<"/-Texture-/"<<id_texture<<"///::"<<std::endl;
        assert(glGetError());
        data_st->add_element(id_bd,std::move(id_texture));
    }

    ~Texture_controller_BINDLESS(){
        //std::cout<<"||||||||||||||||||||||Texture_controller_BINDLESS|||||||||||||:"<<std::endl;
        glMakeTextureHandleNonResidentARB(id_texture);
    }
};

#undef STB_IMAGE_IMPLEMENTATION
#endif // TEXTURE_H

