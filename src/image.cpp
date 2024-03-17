#include "image.h"


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <iostream>
#include <vector>
namespace CeEngine {
    void  Image::to_png(std::filesystem::path path_image){
        std::cout<<width<<"|"<<height<<":::::::::::::::::::::;;;;"<<path_image.string().c_str()<<std::endl;
        std::vector<GL_layer::GLubyte> data_pixel;
        //переворачивание изображения, вынести
        for(std::size_t f=height;f>0;f--){
            for(std::size_t j=0;j<(width*pixel_size);j++){
                data_pixel.emplace_back(data[(f-1)*(width*pixel_size)+j]);
            }
        }

        //stbi_set_flip_vertically_on_load(true);
        stbi_write_png(path_image.string().c_str(),width,height,STBI_grey,data_pixel.data(),0);
    };

    Image::Image(std::filesystem::path path_image){
        std::cout<<":::::::::::::::::::::;;;;"<<std::endl;
        int n=0;
        //предустановленно, можно поменять
        setting.filter_atribute.set_data<GL_layer::TYPE_SETTING_TEXTURE::TEXTURE_WRAP_S>(GL_layer::TYPE_ARGUMENT_SETTING_TEXTURE::REPEAT);
        setting.filter_atribute.set_data<GL_layer::TYPE_SETTING_TEXTURE::TEXTURE_WRAP_T>(GL_layer::TYPE_ARGUMENT_SETTING_TEXTURE::REPEAT);

        setting.filter_atribute.set_data<GL_layer::TYPE_SETTING_TEXTURE::TEXTURE_MIN_FILTER>(GL_layer::TYPE_ARGUMENT_SETTING_TEXTURE::LINEAR);
        setting.filter_atribute.set_data<GL_layer::TYPE_SETTING_TEXTURE::TEXTURE_MAG_FILTER>(GL_layer::TYPE_ARGUMENT_SETTING_TEXTURE::LINEAR);

        data=std::shared_ptr<unsigned char[]>{stbi_load(path_image.string().data(), &width, &height,&n,STBI_rgb_alpha)};
    }
}
