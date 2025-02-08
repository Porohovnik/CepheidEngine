#include "glifs.h"

//#define FT_ERROR_START_LIST
//#define FT_CONFIG_OPTION_ERROR_STRINGS


#include <ft2build.h>
#include FT_FREETYPE_H


#include <iostream>

std::vector<int> locale_to_symbols_code(){
    std::vector<int> symbols_code;

    for(uint symbol=0;symbol<128;symbol++){
        symbols_code.push_back(symbol);
    }

    //смещение  для поддержки русскго языка ,потом оптимизировать для других языков
    for(uint symbol=1020;symbol<1120;symbol++){
        symbols_code.push_back(symbol);
    }

    return symbols_code;
}



Glifs load_glif(std::filesystem::path path_font, std::size_t size){
    auto symbols_code=locale_to_symbols_code();

    Glifs glifs;

    FT_Library ft;
    if (FT_Init_FreeType(&ft)){
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    FT_Face face;
    FT_New_Face(ft,path_font.string().data(), 0, &face);

    FT_Set_Pixel_Sizes(face, 0, size);
    for(auto &S:symbols_code){
        FT_Load_Char(face,S, FT_LOAD_RENDER);
        glifs.glif_infos.push_back({});
        glifs.glif_images.push_back({});

        glifs.glif_infos.back().size={face->glyph->bitmap.width+2,face->glyph->bitmap.rows+2};
        glifs.glif_infos.back().bearing={face->glyph->bitmap_left,face->glyph->bitmap_top};

        glifs.glif_infos.back().advance=face->glyph->advance.x>>6;
        glifs.glif_infos.back().max_size=size;

        //std::cout<<S<<" : "<<(face->glyph->advance.x)<<std::endl;

        glifs.glif_images.back().widths =glifs.glif_infos.back().size.x;
        glifs.glif_images.back().heights=glifs.glif_infos.back().size.y;

        glifs.glif_images.back().pixel_size=1;
        glifs.glif_images.back().fill({0,0,0,0});

        int kk=0;
        for(int f=(glifs.glif_images.back().heights-1-1);f>=1;f--){
            for(int j=1;j<(glifs.glif_images.back().widths-1)*1;j++){
               glifs.glif_images.back().image.get()[j+f*glifs.glif_images.back().widths*1]=face->glyph->bitmap.buffer[kk];
               kk++;
            }
        }
        //std::cout<<": "<<glifs.glif_infos.size()<<std::endl;
    };

    //space-symbol
    glifs.glif_infos[32].size={size/2,size/2};
    glifs.glif_images[32].widths =glifs.glif_infos[32].size.x;
    glifs.glif_images[32].heights=glifs.glif_infos[32].size.y;
    glifs.glif_images[32].fill({0,0,0,0});

    FT_Done_Face(face);   // Завершение работы с шрифтом face
    FT_Done_FreeType(ft); // Завершение работы FreeType
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    return glifs;
};
