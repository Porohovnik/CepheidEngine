#include "SVG.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#define NANOSVG_ALL_COLOR_KEYWORDS // Include full list of color keywords.
#define NANOSVG_IMPLEMENTATION // Expands implementation
#include "nanosvg.h"

#include <iostream>
namespace CeEngine {
SVG::SVG(std::filesystem::path path){
    NSVGimage* image=nsvgParseFromFile(path.string().c_str(), "px", 96);
    width=image->width;
    height=image->height;

    printf("size: %f x %f\n", width, height);

    //загрузка информации
    for (auto shape = image->shapes; shape != nullptr; shape = shape->next) {
        objects.emplace_back(Color(shape->fill.color,Color::Revers{}),shape->id);
        for (auto path = shape->paths; path != nullptr; path = path->next) {
            objects.back().bezie_curves_data.emplace_back();
            objects.back().bezie_curves_data.back().color=Color(shape->stroke.color,Color::Revers{});
            objects.back().bezie_curves_data.back().width=shape->strokeWidth;
            objects.back().bezie_curves_data.back().type=shape->stroke.type;

            for (std::size_t i = 0; i < path->npts-1; i += 3) {
                float* p = &path->pts[i*2];
                float sum = std::abs(p[0]-p[2])+std::abs(p[2]-p[4])+std::abs(p[4]-p[6])+std::abs(p[6]-p[0])+
                            std::abs(p[1]-p[3])+std::abs(p[3]-p[5])+std::abs(p[5]-p[7])+std::abs(p[7]-p[1]);

                if(sum<0.0001f){
                    std::cout<<sum<<std::endl;
                }else {
                    objects.back().bezie_curves_data.back().contour_data.emplace_back(CubicBez_points{glm::vec3{p[0],p[1],0.0f},
                                                                                         glm::vec3{p[2],p[3],0.0f},
                                                                                         glm::vec3{p[4],p[5],0.0f},
                                                                                         glm::vec3{p[6],p[7],0.0f}});
                }
            }
        }
    }

    nsvgDelete(image);
}
}// namespace CeEngine
