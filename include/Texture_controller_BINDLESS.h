#ifndef TEXTURE_CONTROLLER_BINDLESS_H
#define TEXTURE_CONTROLLER_BINDLESS_H
#include "texture.h"

//нужен механизм выгрузки данных с С VRAM , без удаления ресурсов
namespace CeEngine {
class Texture_controller_BINDLESS:public Texture{
    typename GL_layer::Buffer_id_texture::Texture_handles handles;
public:
    template<typename D, typename  Image>
    Texture_controller_BINDLESS(D * data_st,int id_bd,Image image):Texture(image),handles(this->Textur.get_handles()){

        std::cout<<GL_layer::Get_Error()<<"/-Texture-/"<< handles.get_id()<<"///::"<<image.get_height() <<std::endl;
        data_st->add_element(id_bd,std::move(handles.get_id()));
    }
};
}// namespace CeEngine
#endif // TEXTURE_CONTROLLER_BINDLESS_H
