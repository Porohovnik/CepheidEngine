#ifndef CREATE_SCREENSHOT_H
#define CREATE_SCREENSHOT_H
#include <filesystem>
#include "image.h"
#include "interlayer_gl.h"

#include <iostream>
namespace CeEngine {
void create_screenshot(std::size_t x,std::size_t size_x,std::size_t y,std::size_t size_y,std::filesystem::path path,GL_layer::GLenum type_scheme_pixel=GL_layer::get_value_GL<GL_layer::SHEME_COLOR_GL::RGB>()){
    std::cout<<path<<std::endl;
    std::size_t pixel_size=4;
    if (type_scheme_pixel==GL_layer::get_value_GL<GL_layer::SHEME_COLOR_GL::RGB>()){
        pixel_size=1;
    }
    Image image(size_x,size_y+1,pixel_size,Image::Setting_image{},nullptr);
    *image.get_data()=std::shared_ptr<unsigned char[]>(new unsigned char[image.get_width()*(image.get_height()+1)*image.get_pixel_size()]);

    GL_layer::read_pixel(x,y,size_x,size_y,type_scheme_pixel,GL_layer::get_value_GL<GL_layer::SHEME_TYPE_GL::UNSIGNED_INT>(),image.get_data()->get());

    image.to_png(path);
}
}// namespace CeEngine
#endif // CREATE_SCREENSHOT_H
