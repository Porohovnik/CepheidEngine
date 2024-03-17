#include "Offline_packing.h"

namespace CeEngine {
using Height = Level::Height;
using Width = Level::Height;


bool Level::is_empty_place_below(Width width_object,Width width_atlas){
    return  width_object<=width_atlas-wight_fill;//влезет ли прмоугольник слева
}

bool Level::is_empty_place_above(Width width_object,Height height_object,Width width_atlas){
    if (width_object<=width_atlas-wight_fill_ceiling) {//влезет ли прмоугольник справа

        std::size_t weight=object_available_width;//доступная ширина c учётом перекрытия
        std::ptrdiff_t i=object_overlap;

        if(object_overlap==-1){
            weight=width_atlas-wight_fill;//доступная ширина
            i=objects_level.size()-1;
        }

        for(;i>=0;i--){
            //считаем доступную ширину справа налево(зависит от имеющихся объектов)
            if(object_overlap!=i){
                weight+=objects_level[i].first;
            }
            //если ширины хватает, проверяем, влезет ли объект
            if(width_object<=weight ){//если ширины достаточно проверям, влезит ли по высоте
                if((height_object+objects_level[i].second)<=height){//(так как они отсортированны по высоте, достаточно проверить последний
                    object_overlap=i;
                    object_available_width=weight-width_object;//лишнию ширину объекта сохраним
                    return true;
                }else {
                    return false;
                }
            }
        }
    }
    return false;
}

std::pair<Height,std::vector<std::pair<Width,Height>>> Level::offline_packing(Width width_atlas,std::multimap<Height,Width,std::greater<int>> & array){
    std::vector<Level> levels;
    std::vector<std::pair<Width,Height>> position_image;

    int viki=0;//отсылка
    for(auto &[Height,Weight]:array){
        bool isLevel=false;
        for(auto &L:levels){
            isLevel=L.is_empty_place_below(Weight,width_atlas);
            if(isLevel){
                position_image.emplace_back(L.wight_fill,L.y);

                L.objects_level.emplace_back(Weight,Height);//добавляем данные объекта
                L.wight_fill+=Weight;
                break;
            }

            isLevel=L.is_empty_place_above(Weight,Height,width_atlas);
            if(isLevel){
                L.wight_fill_ceiling+=Weight;
                position_image.emplace_back(width_atlas-L.wight_fill_ceiling,L.y+L.height-Height);
                break;
            }
        }

        if(isLevel==false){
            //добавляем новый уровень
            levels.emplace_back();
            levels.back().height=Height;
            if(levels.size()>1){
                levels.back().y=levels[levels.size()-1-1].height+levels[levels.size()-1-1].y;
            }
            //добавляем на него объект
            position_image.emplace_back(0,levels.back().y);

            levels.back().objects_level.emplace_back(Weight,Height);
            levels.back().wight_fill+=Weight;
        }

    }

    return std::pair<Height,std::vector<std::pair<Width,Height>>>{levels.back().y+levels.back().height,position_image};
}
}
