#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include <memory>
#include <filesystem>
#include <vector>

#include "color.h"
#include "interlayer_gl.h"

enum TYPE_TEXTURE {
    Map_diffusion,
    Map_normals,
    Map_reflection,
    Map_height,
    Map_cub,
    Map_font,
    Map_atlas,
    No_map
};


class Image{
public:
    Image(){}
    GL_layer::Type_filter filter_atribute{};

    std::shared_ptr<unsigned char[]> image=nullptr;
    int  widths=0;
    int  heights=0;
    int  n=0;
    int  deep=0;

    TYPE_TEXTURE type=Map_diffusion;
    int pixel_size=1;
    int type_pixel_grid=GL_RGBA;
    bool mipmap=false;

    void fill(Color color_){//rgba
        auto color=color_.get_color()*256.0f;
        image=std::shared_ptr<unsigned char[]>(new unsigned char[widths*heights*pixel_size]);
        switch (pixel_size) {
            case 1:for(std::size_t  i=0;i<widths*heights*pixel_size;i+=pixel_size){image.get()[i]=color.r;};break;
            case 2:for(std::size_t  i=0;i<widths*heights*pixel_size;i+=pixel_size){image.get()[i]=color.r;image.get()[i+1]=color.g;};break;
            case 3:for(std::size_t  i=0;i<widths*heights*pixel_size;i+=pixel_size){image.get()[i]=color.r;image.get()[i+1]=color.g,image.get()[i+2]=color.b;};break;
            case 4:for(std::size_t  i=0;i<widths*heights*pixel_size;i+=pixel_size){image.get()[i]=color.r;image.get()[i+1]=color.g,image.get()[i+2]=color.b;image.get()[i+3]=color.a;};break;
        }
    }
};

void  to_png(std::filesystem::path path_image,Image & im);
Image load_image(std::filesystem::path path_image,TYPE_TEXTURE type=Map_diffusion);
std::pair<Image, std::vector<glm::vec4>> create_atlas_image(std::vector<Image> & images, TYPE_TEXTURE type_map);

#endif // IMAGE_H
