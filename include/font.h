#ifndef FONT_H
#define FONT_H
#include <functional>
#include <iostream>

namespace CeEngine {
template <typename TEX>
class Font_controller:public TEX{
    std::function<void()> deleter=nullptr;
public:
    template<typename D_glif,typename Data_font,typename ...Arg>
    Font_controller(D_glif * data_st_glifs, int id_bd, Data_font && data_font,Arg...arg):TEX(arg...,data_font.atlas){
        data_st_glifs->add_element(id_bd,std::move(data_font.glif_infos),data_font.glif_infos.size());
        std::cout<<"|<-font->|"<<data_font.atlas.data_fragment.size()<<std::endl;

        deleter=[data_st_glifs,id_bd](){
            if(data_st_glifs!=nullptr){
                data_st_glifs->delete_element(id_bd);
            }
        };
    }

    ~Font_controller(){
        if(deleter!=nullptr){
            std::cout<<"||||||||||||||||||||||Font_controller|||||||||||||:"<<std::endl;
            deleter();
        }
    }

    inline void Bind(int base,int id=-1){
         this->TEX::Bind(0);
    }
};

void converting_unicode_to_glyph_number(std::vector<int> &unicod);
void string_to_unicod(const char * text_in, std::vector<int> *simbols_out);
}// namespace CeEngine
#endif // FONT_H
