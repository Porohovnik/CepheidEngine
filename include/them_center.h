#ifndef THEM_CENTER_H
#define THEM_CENTER_H
#include <filesystem>
#include <string>
#include <unordered_map>

#include "color.h"
#include "tuple_utl.h"

namespace CeEngine {
template<typename Them_object_base>
class Sub_Theme{
    std::unordered_map<std::string,Them_object_base> setting;                                       //базовые(из примитивов) объекты
    std::unordered_map<std::string,std::unordered_map<std::string,Them_object_base>> setting_object;//объекты из базовых и примитивов
};

template<typename Them_object_base>
class Theme{
    std::unordered_map<std::string,Sub_Theme<Them_object_base>> subtheme;

};


template<typename ...Arg>
class Theme_center{
public:
    using Them_object_base_base=tutl::Type_to_data<Arg...>;
private:
    std::unordered_map<std::string,Theme<Them_object_base_base>> theme;
public:

};
namespace  Enum_them{

constexpr char color_background[]="color_background";
constexpr char color_text[]      ="color_text";
constexpr char color_line[]      ="color_line";

constexpr char color_border[]    ="color_border";
constexpr char color_border_l[]  ="color_border_l";
constexpr char color_border_r[]  ="color_border_r";
constexpr char color_border_t[]  ="color_border_t";
constexpr char color_border_b[]  ="color_border_b";

constexpr char font_name[]       ="font";
constexpr char image_background[]="image";

constexpr char them_name[]       ="them";

}

template<typename T>
struct Condition_argument{
    T normal;
    T hover;
    T visited;
    T action_right;
    T action_lift;
    Condition_argument(T normal_):normal(normal_),hover(normal_),visited(normal_),action_right(normal_),action_lift(normal_){}
};

using Them_centre_=Theme_center<
    tutl::KAAT<Enum_them::color_background,Condition_argument<Color>>,
    tutl::KAAT<Enum_them::color_text      ,Condition_argument<Color>>,
    tutl::KAAT<Enum_them::color_line      ,Condition_argument<Color>>,

    tutl::KAAT<Enum_them::color_border    ,Condition_argument<Color>>,
    tutl::KAAT<Enum_them::color_border_l  ,Condition_argument<Color>>,
    tutl::KAAT<Enum_them::color_border_r  ,Condition_argument<Color>>,
    tutl::KAAT<Enum_them::color_border_t  ,Condition_argument<Color>>,
    tutl::KAAT<Enum_them::color_border_b  ,Condition_argument<Color>>,

    tutl::KAAT<Enum_them::font_name       ,Condition_argument<std::filesystem::path>>,
    tutl::KAAT<Enum_them::image_background,Condition_argument<std::filesystem::path>>
>;
}// namespace CeEngine

#endif // THEM_CENTER_H
