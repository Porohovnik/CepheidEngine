#ifndef IMAGE_ATLAS_H
#define IMAGE_ATLAS_H
#include "image.h"
#include "glm/vec4.hpp"
namespace CeEngine {
class Image_atlas:public Image{
public:
    std::vector<glm::vec4> data_fragment;

    Image_atlas()=default;
    Image_atlas(std::vector<Image> & images);
};
}// namespace CeEngine

#endif // IMAGE_ATLAS_H
