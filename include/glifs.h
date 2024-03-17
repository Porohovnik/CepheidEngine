#ifndef GLIFS_H
#define GLIFS_H

#include <glm/vec2.hpp>
#include "Image_atlas.h"
namespace CeEngine {
struct Glif_info{
    glm::vec2 size;      // Размеры глифа
    glm::vec2 bearing;   // Смещение верхней левой точки глифа
    //информация об атласе текста
    glm::vec2 coordinate_transformation;
    glm::vec2 position ;

    float     advance;   // Горизонтальное смещение до начала следующего глифа
    float    max_size;
};

class Data_font{
    std::vector<std::string>    locales;
public:
    std::vector<Glif_info> glif_infos;
    Image_atlas atlas;
    Data_font(std::filesystem::path path_font, std::size_t size);
};
}// namespace CeEngine
#endif // GLIFS_H
