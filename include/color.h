#ifndef COLOR_H
#define COLOR_H
#include <cstdint>

namespace CeEngine {
union Convectr{
   uint32_t hex=0;
   uint8_t rgba[4];
};
class Color{
    inline static Convectr convector;
    enum  RGBA :int{R=0,G=1,B=2,A=3};
public:
    float r=0.0f;
    float g=0.0f;
    float b=0.0f;
    float a=0.0f;

    class Revers{};

    Color(uint32_t hex_,Revers){
        convector.hex=hex_;
        //std::cout<<hex_<<"COOOOOOOOOOOOOOLOR"<<static_cast<int>(convector.rgba[RGBA::R])/256.0f<<"|"<<static_cast<int>(convector.rgba[RGBA::G])/256.0f<<"|"<<static_cast<int>(convector.rgba[RGBA::B])/256.0f<<"|"<<static_cast<int>(convector.rgba[RGBA::A])/256.0f<<std::endl;
        r=static_cast<int>(convector.rgba[RGBA::R])/256.0f;
        g=static_cast<int>(convector.rgba[RGBA::G])/256.0f;
        b=static_cast<int>(convector.rgba[RGBA::B])/256.0f;
        a=static_cast<int>(convector.rgba[RGBA::A])/256.0f;
    }

    Color(uint32_t hex_){
        convector.hex=hex_;
        //std::cout<<hex_<<"COOOOOOOOOOOOOOLOR"<<static_cast<int>(convector.rgba[RGBA::R])/256.0f<<"|"<<static_cast<int>(convector.rgba[RGBA::G])/256.0f<<"|"<<static_cast<int>(convector.rgba[RGBA::B])/256.0f<<"|"<<static_cast<int>(convector.rgba[RGBA::A])/256.0f<<std::endl;
        r=static_cast<int>(convector.rgba[RGBA::A])/256.0f;
        g=static_cast<int>(convector.rgba[RGBA::B])/256.0f;
        b=static_cast<int>(convector.rgba[RGBA::G])/256.0f;
        a=static_cast<int>(convector.rgba[RGBA::R])/256.0f;
    }

    Color(float r_, float g_, float b_,float a_):r(r_),g(g_),b(b_),a(a_){}
    Color(){}
};
}// namespace CeEngine
#endif //
