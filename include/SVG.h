#ifndef SVG_H
#define SVG_H
#include <filesystem>
#include <glm/vec3.hpp>
#include <color.h>

namespace CeEngine {
class SVG{
    float width=0.0f;
    float height=0.0f;

    struct CubicBez_points{
        glm::vec3 p0;
        glm::vec3 p1;
        glm::vec3 p2;
        glm::vec3 p3;
    };

    using  ContourSVG=std::vector<CubicBez_points>;

    struct Object_SVG{
        std::vector<ContourSVG> bezie_curves_data;
        Color  color;
        std::string id;

        Object_SVG(Color  color_, std::string id_):color(color_),id(id_){}
    };

public:
    std::vector<Object_SVG> objects;
    SVG(std::filesystem::path path);
};
}// namespace CeEngine
#endif // SVG_H
