#ifndef TEXTURE_CONTROLLER_BINDLESS_H
#define TEXTURE_CONTROLLER_BINDLESS_H
#include "texture.h"

#include<memory>
//нужен механизм выгрузки данных с С VRAM , без удаления ресурсов
namespace CeEngine {
class Texture_controller_BINDLESS:public Texture{
    inline static std::unique_ptr<Texture_controller_BINDLESS> tex_default=nullptr;
public:
    typename GL_layer::Buffer_id_texture::Texture_handles handles;//пока так

    template<typename D, typename  Image>
    Texture_controller_BINDLESS(D * data_st,int id_bd,Image image):Texture(image),handles(this->Textur.get_handles()){
        std::cout<<GL_layer::Get_Error()<<"/-Texture-/"<< handles.get_id()<<"///::"<<image.get_height() <<std::endl;
        if(data_st!=nullptr){
            data_st->add_element(id_bd,std::move(handles.get_id()));
        }
    }

    template<typename D, typename  Image>
    static void default_value(D data_st,int id_bd){
        if(tex_default==nullptr){
            tex_default=std::make_unique<Texture_controller_BINDLESS>( static_cast<std::remove_reference_t<D>>(nullptr) ,id_bd,Image());
        }
        data_st->add_element(id_bd,std::move(tex_default->handles.get_id()));
        std::cout<<"lll_|_lll: "<<id_bd<<std::endl;
    }
};
}// namespace CeEngine
#endif // TEXTURE_CONTROLLER_BINDLESS_H
