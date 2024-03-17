#ifndef CEENUMERATION_H
#define CEENUMERATION_H
namespace CeEngine {
enum Type_Status:int {CLEAR=3,DISABLE=2,NOVISEBLE=1,VISEBLE=0};
using uint =unsigned int;

enum class TIME_BIND{NEVER=0,SHOT=1,      OBJECT=2,
                       FIRST_SHOT=3,FIRST_OBJECT=4,
                        LAST_SHOT=5, LAST_OBJECT=6,
                        FIRST_SHOT_LAST_OBJECT=7,
                        FIRST_SHOT_FIRST_OBJECT=8,
                        LAST_SHOT_LAST_OBJECT=9,
                        LAST_SHOT_FIRST_OBJECT=10,
                        SHOT_LAST_OBJECT=11,
                        SHOT_FIRST_OBJECT=12,
                        LAST_SHOT_OBJECT=13,
                        FIRST_SHOT_OBJECT=14,
                        SHOT_OBJECT=15
                    };

enum class TYPE_CONTAINER{ONE=0,ARRAY=1};
enum class TYPE_RES_STORAGE{LOCAL=0,GLOBAL=1};
enum class TRIVIAL_DELETE{YES,NO};

enum class TYPE_ORDER_UPDATE{FIRST,ANY};


enum TYPE_TEXTURE {
    Map_diffusion,
    Map_normals,
    Map_reflection,
    Map_height,
    Map_cub,
    Map_font,
    Map_atlas,
    No_map
};

enum KEY {
    MOUSE_BUTTON_LEFT=0,
    MOUSE_BUTTON_RIGHT=1,
    KEY_ENTER=257,
    KEY_BACKSPACE=259
};


}// namespace CeEngine


#endif // CEENUMERATION_H
