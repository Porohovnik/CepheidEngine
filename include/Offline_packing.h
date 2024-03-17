#ifndef OFFLINE_PACKING_H
#define OFFLINE_PACKING_H
#include <utility>
#include <vector>
#include <tuple>
#include <map>

namespace CeEngine {
struct Level{
    using Width=std::size_t;
    using Height=std::size_t;

    Width wight_fill=0;
    Width wight_fill_ceiling=0;
    Width object_available_width=0;
    std::ptrdiff_t object_overlap=-1;

    Height height=0;
    Height y=0;

    std::vector<std::pair<Width,Height>> objects_level;//widht,height

    bool is_empty_place_below(Width width_object,Width width_atlas);
    bool is_empty_place_above(Width width_object,Height height_object,Width width_atlas);
public:
    static std::pair<Height,std::vector<std::pair<Width,Height>>> offline_packing(Width width_atlas,std::multimap<Height,Width,std::greater<int>> & array);
};
}// namespace CeEngine
#endif // OFFLINE_PACKING_H
