#ifndef GLIFS_H
#define GLIFS_H

#include <glm/vec2.hpp>
#include "image.h"

struct Glif_info{
    glm::vec2 size;      // Размеры глифа
    glm::vec2 bearing;   // Смещение верхней левой точки глифа
    //информация об атласе текста
    glm::vec2 coordinate_transformation;
    glm::vec2 position;

    float     advance;   // Горизонтальное смещение до начала следующего глифа
    float    max_size;
};

class Glifs{  
public:
    Glifs()=default;
    std::vector<std::string>    locales;
    std::vector<Image> glif_images;
    std::vector<Glif_info> glif_infos;
    int size=0;
};


Glifs load_glif(std::filesystem::path path_font, std::size_t size);

#endif // GLIFS_H
