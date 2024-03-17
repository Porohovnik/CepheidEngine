#include "Image_atlas.h"
#include <cmath>
#include "Offline_packing.h"
namespace CeEngine {
inline static std::size_t iiiiii=0;
Image_atlas::Image_atlas(std::vector<Image> & images){

    width=std::pow(2,10);
    height=0;
    pixel_size=1;

    //настройка изображения
    setting.type_scheme_pixel=GL_layer::SHEME_COLOR_GL::RED;
    setting.filter_atribute.set_data<GL_layer::TYPE_SETTING_TEXTURE::TEXTURE_WRAP_S>(GL_layer::TYPE_ARGUMENT_SETTING_TEXTURE::REPEAT);
    setting.filter_atribute.set_data<GL_layer::TYPE_SETTING_TEXTURE::TEXTURE_WRAP_T>(GL_layer::TYPE_ARGUMENT_SETTING_TEXTURE::REPEAT);

    setting.filter_atribute.set_data<GL_layer::TYPE_SETTING_TEXTURE::TEXTURE_MIN_FILTER>(GL_layer::TYPE_ARGUMENT_SETTING_TEXTURE::LINEAR);
    setting.filter_atribute.set_data<GL_layer::TYPE_SETTING_TEXTURE::TEXTURE_MAG_FILTER>(GL_layer::TYPE_ARGUMENT_SETTING_TEXTURE::LINEAR);


    //подготавлием данные для алгоритма упаковки
    std::multimap<std::size_t,std::size_t,std::greater<int>> array_id;
    std::multimap<std::size_t,std::size_t,std::greater<int>> array;

    for(std::size_t  i=0;i<images.size();i++){
        array_id.emplace(images[i].get_height(), i);
        array.emplace(images[i].get_height(),images[i].get_width());
    }

    //получаем данные с алгоритма упоковки
    std::pair<std::size_t,std::vector<std::pair<std::size_t,std::size_t>>> position_data=Level::offline_packing(width,array);
    height=position_data.first;

    //перетасовываем данные в правильном порядке
    std::vector<std::pair<std::size_t,std::size_t>> position_image(images.size());
    position_image.resize(images.size());
    std::size_t i=0;
    for(auto &[Height,ID]:array_id){
        position_image[ID].first=position_data.second[i].first;
        position_image[ID].second=position_data.second[i].second;

        i++;
    }

    //приведение данных позииции в удобный формат
    for(std::size_t i=0;i<images.size();i++){
       data_fragment.emplace_back( (position_image[i].first+1)/static_cast<float>(width),//приведение к ширене
                                   (position_image[i].second+1)/static_cast<float>(height),//приведение к высоте
                                   (images[i].get_width()-2)/static_cast<float>(width),//приведение к ширене
                                   (images[i].get_height()-2)/static_cast<float>(height));//приведение к высоте
    }

    //создаём изображение
    data=std::shared_ptr<unsigned char[]>(new unsigned char[width*height*pixel_size]);
    for(std::size_t i=0;i<width*height*pixel_size;i++){
        data[i]=0;
    }

    //соединение изображений в атлас
    for(std::size_t t=0;t<images.size();t++){
        std::size_t k=0;
        for(std::size_t i=0;i<images[t].get_height();i++){
            for(std::size_t j=0;j<(images[t].get_width())*pixel_size;j++){
                std::size_t index=static_cast<uint>(position_image[t].first)*pixel_size+j+(i+static_cast<uint>(position_image[t].second))*width*pixel_size;

                data[index]=images[t][k];
                k++;
            }
        }
    }

    iiiiii++;
    to_png(std::to_string(iiiiii)+".png");
}
}
