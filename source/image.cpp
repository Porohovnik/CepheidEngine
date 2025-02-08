#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC


#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <math.h>
#include <glm/vec4.hpp> // glm::vec3
#include <glm/vec2.hpp> // glm::vec3
#include <map>
#include <GL/gl.h>
#include <iostream>

void  to_png(std::filesystem::path path_image,Image & im){
    std::cout<<":::::::::::::::::::::;;;;"<<path_image.string().c_str()<<std::endl;
    std::vector<GLubyte> data_pixel;
    for(std::size_t f=im.heights;f>0;f--){
        for(std::size_t j=0;j<(im.widths*im.pixel_size);j++){
            data_pixel.emplace_back(im.image.get()[(f-1)*(im.widths*im.pixel_size)+j]);
        }
    }

    stbi_write_png(path_image.string().c_str(),im.widths,im.heights,STBI_rgb,data_pixel.data(),0);
};

Image load_image(std::filesystem::path path_image,TYPE_TEXTURE type){
    std::cout<<":::::::::::::::::::::;;;;"<<std::endl;
    Image im_resourse{};

    //предустановленно, можно поменять
    im_resourse.filter_atribute.set_data<GL_TEXTURE_WRAP_S>(GL_REPEAT);
    im_resourse.filter_atribute.set_data<GL_TEXTURE_WRAP_T>(GL_REPEAT);

    im_resourse.filter_atribute.set_data<GL_TEXTURE_MIN_FILTER>(GL_LINEAR);
    im_resourse.filter_atribute.set_data<GL_TEXTURE_MAG_FILTER>(GL_LINEAR);


    im_resourse.type=type;
    im_resourse.image=std::shared_ptr<unsigned char[]>(stbi_load(path_image.string().data(), &im_resourse.widths, &im_resourse.heights,&im_resourse.n,STBI_rgb_alpha));
    return im_resourse;
}

std::pair<Image, std::vector<glm::vec4>> create_atlas_image(std::vector<Image> & images, TYPE_TEXTURE type_map){
    std::pair<Image,std::vector<glm::vec4>> atlas_image;
    //настройка
    atlas_image.first.type=type_map;
    atlas_image.first.type_pixel_grid=GL_RED;
    atlas_image.first.filter_atribute.set_data<GL_TEXTURE_WRAP_S>(GL_REPEAT);
    atlas_image.first.filter_atribute.set_data<GL_TEXTURE_WRAP_T>(GL_REPEAT);

    atlas_image.first.filter_atribute.set_data<GL_TEXTURE_MIN_FILTER>(GL_LINEAR);
    atlas_image.first.filter_atribute.set_data<GL_TEXTURE_MAG_FILTER>(GL_LINEAR);


    std::multimap<int,std::pair<int,Image>,std::greater<int>> array;
    const int W=std::pow(2,10); 

    int pixel_size=1;

    struct Level{
        int height=0;
        int wight_fill=0;
        int wight_fill_ceiling=0;
        int y=0;
        std::vector<glm::vec2> floor;//widht,height
    };
    
    std::vector<Level> levels;

    std::vector<glm::vec2> poition_image(images.size());


    for(int i=0;i<images.size();i++){
        if(images[i].image==nullptr){
            std::cout<<"|error|"<<i<<":"<<images.size()<<std::endl;
            assert(true);
        }
        array.insert({images[i].heights,{i,images[i]}});
    }
    
    

    int viki=0;
    for(auto &[L,T]:array){
        int found = -1;
        for (int j = 0; j < levels.size(); j++) {
            if (T.second.widths<=(W-levels[j].wight_fill)) {//влезет ли прмоугольник слева
                    found = j;//установить уровень если проходит
                    continue;
            }
        }
        if (found > -1) { // floor-pack on existing level
            levels[found].floor.push_back({T.second.widths,T.second.heights});

            poition_image[T.first]={levels[found].wight_fill,levels[found].y};
            levels[found].wight_fill+=T.second.widths;
        } else {
            int found = -1;
            for (int j = 0; j < levels.size(); j++) {
                bool t=false;
                if(T.second.widths<=(W-levels[j].wight_fill_ceiling-levels[j].floor[0].x)){
                    int weight=0;
                    int i=levels[j].floor.size()-1;
                    for(;i>=0;i--){
                        weight+=levels[j].floor[i].x;

                        if(T.second.widths<=weight ){
                            if((T.second.heights+levels[j].floor[i].y)<=levels[j].height){
                                t=true;
                            }
                            continue;
                        }
                    }
                }
                if (t) {
                    found = j;
                    continue;
                }
            }
            if (found > -1) { // ceiling-pack on existing level
                levels[found].wight_fill_ceiling+=T.second.widths;
                poition_image[T.first]={W-levels[found].wight_fill_ceiling,levels[found].y+levels[found].height-T.second.heights};
            } else {
                levels.push_back({T.second.heights,T.second.widths});
                if(levels.size()>1){
                    levels.back().y=levels[levels.size()-1-1].height+levels[levels.size()-1-1].y;
                }
                levels.back().floor.push_back({T.second.widths,T.second.heights});
                poition_image[T.first]={0,levels.back().y};
            }
        }

       //std::cout<<L<<":"<<T.second.widths<<"|["<<poition_image[T.first].x<<";"<<poition_image[T.first].y<<"]"<<viki<<":"<<T.first<<std::endl;
       viki++;
    }

    atlas_image.first.widths=W;
    atlas_image.first.heights=(levels.back().y+levels.back().height);

    atlas_image.first.fill({0,0,0,0});

    for(int t=0;t<images.size();t++){
        int k=0;

        for(int i=0;i<images[t].heights;i++){
            for(int j=0;j<images[t].widths*pixel_size;j++){
                uint index=static_cast<uint>(poition_image[t].x*pixel_size)+j+(i+static_cast<uint>(poition_image[t].y))*W*pixel_size;

                atlas_image.first.image[index]=images[t].image[k];
                k++;
            }
        }
        atlas_image.second.push_back({(poition_image[t].x+1)/static_cast<float>(atlas_image.first.widths),
                           (poition_image[t].y+1)/static_cast<float>(atlas_image.first.heights),
                           (images[t].widths-2)/static_cast<float>(atlas_image.first.widths),
                           (images[t].heights-2)/static_cast<float>(atlas_image.first.heights)});
    }

    std::cout<<reinterpret_cast<long long>(atlas_image.first.image.get())<<std::endl;
    return atlas_image;


};
