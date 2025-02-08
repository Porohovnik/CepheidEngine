#ifndef IMAGE_H
#define IMAGE_H
#include <memory>
#include <filesystem>

#include "GL_type_to_cpp.h"
#include "Atribute_texture.h"


namespace CeEngine {
class Image{
public:
    using Width=GL_layer::GLsizei;
    using Height=GL_layer::GLsizei;

    struct Setting_image{
        GL_layer::Type_filter_atribute_texture filter_atribute;

        std::size_t cout_image=1;
        bool mipmap=false;
        GL_layer::SHEME_COLOR_GL type_scheme_pixel=GL_layer::SHEME_COLOR_GL::RGBA;
        GL_layer::TYPE_TEXTURE POINT_TEX=GL_layer::TYPE_TEXTURE::TEXTURE_2D;
    };
protected:
    Width  width=1;
    Height height=1;
    std::size_t pixel_size=4;
    Setting_image setting;
    std::shared_ptr<unsigned char[]> data=std::shared_ptr<unsigned char[]>(new unsigned char[4]);
public:
    Image()=default;
    Image(std::filesystem::path path_image);
    Image( Width  width_,Height height_,std::size_t pixel_size_,Setting_image setting_,std::shared_ptr<unsigned char[]> data_):
        width(width_),height(height_),pixel_size(pixel_size_),setting(setting_),data(data_){}

    void to_png(std::filesystem::path path_image);

    GL_layer::TYPE_TEXTURE get_POINT_TEX() const{
        return setting.POINT_TEX;
    }

    Width get_width() const{
        return width;
    }
    Height get_height() const{
        return height;
    }
    std::size_t get_pixel_size() const{
        return pixel_size;
    }

    const Setting_image & get_setting() const{
        return setting;
    }

    auto * get_data() {
        return &data;
    }

    auto & operator [](std::size_t i){
        return data[i];
    }

};
}// namespace CeEngine
#endif // IMAGE_H
