#ifndef COLOR_H
#define COLOR_H
#include <glm/vec4.hpp> // glm::vec4

enum  RGBA :int{R=0,G=1,B=2,A=3};

union Convectr{
   uint32_t hex=0;
   uint8_t rgba[4];
};
class Color{  
    inline static Convectr convector;

    glm::vec4 color_opengl;
public:
    Color(uint32_t hex_){
        convector.hex=hex_;
        //std::cout<<hex_<<"COOOOOOOOOOOOOOLOR"<<static_cast<int>(convector.rgba[RGBA::R])/256.0f<<"|"<<static_cast<int>(convector.rgba[RGBA::G])/256.0f<<"|"<<static_cast<int>(convector.rgba[RGBA::B])/256.0f<<"|"<<static_cast<int>(convector.rgba[RGBA::A])/256.0f<<std::endl;
        color_opengl={static_cast<int>(convector.rgba[RGBA::A])/256.0f,
                      static_cast<int>(convector.rgba[RGBA::B])/256.0f,
                      static_cast<int>(convector.rgba[RGBA::G])/256.0f,
                      static_cast<int>(convector.rgba[RGBA::R])/256.0f};
    }
    inline glm::vec4 get_color() const{
        return color_opengl;
    }
    Color(float r_, float g_, float b_,float a_):color_opengl(r_,g_,b_,a_){}
    Color(glm::vec4 color_opengl_):color_opengl(color_opengl_){}
    Color(){}

};

#endif //
