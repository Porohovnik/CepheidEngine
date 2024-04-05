#include "glifs.h"

//#define FT_ERROR_START_LIST
//#define FT_CONFIG_OPTION_ERROR_STRINGS


#include <ft2build.h>
#include FT_FREETYPE_H


#include <iostream>
namespace CeEngine {
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

    std::vector<Image> load_data_glifs(std::filesystem::path path_font, std::size_t size,std::vector<int> symbols_code,std::vector<Glif_info> &glif_infos){
        std::vector<Image> glif_images;
        FT_Library ft;
        if (FT_Init_FreeType(&ft)){
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }

        FT_Face face;
        FT_New_Face(ft,path_font.string().data(), 0, &face);
        FT_Set_Pixel_Sizes(face, 0, size);
        for(auto &S:symbols_code){
            FT_Load_Char(face,S, FT_LOAD_RENDER);

            //загрузка данных о глифе
            glif_infos.emplace_back();

            glif_infos.back().size={face->glyph->bitmap.width+2,face->glyph->bitmap.rows+2};
            glif_infos.back().bearing={face->glyph->bitmap_left,face->glyph->bitmap_top};

            glif_infos.back().advance=face->glyph->advance.x>>6;
            glif_infos.back().max_size=size;

            //std::cout<<S<<" : "<<"|"<<face->glyph->bitmap_left<<"|"<<face->glyph->bitmap.width<<"|"<<(face->glyph->advance.x)<<"|"<<glif_infos.back().advance<<std::endl;

            //загрузка самого глифа
            std::shared_ptr<unsigned char[]> image_data=std::shared_ptr<unsigned char[]>(new unsigned char[(face->glyph->bitmap.width+2)*(face->glyph->bitmap.rows+2)]);
            for(std::size_t i=0;i<(face->glyph->bitmap.width+2)*(face->glyph->bitmap.rows+2);i++){
                image_data[i]=0;
            }

            int kk=0;
            for(int f=(glif_infos.back().size.y-1-1);f>=1;f--){
                for(int j=1;j<(glif_infos.back().size.x-1)*1;j++){
                   image_data[j+f*glif_infos.back().size.x*1]=face->glyph->bitmap.buffer[kk];
                   kk++;
                }
            }

            glif_images.emplace_back(glif_infos.back().size.x,glif_infos.back().size.y,1,Image::Setting_image{},image_data);
            //std::cout<<": "<<glifs.glif_infos.size()<<std::endl;
        };

        FT_Done_Face(face);   // Завершение работы с шрифтом face
        FT_Done_FreeType(ft); // Завершение работы FreeType
        return glif_images;
    }

    //отделить загрузку
    Data_font::Data_font(std::filesystem::path path_font, std::size_t size){
        std::vector<Image> glif_images=load_data_glifs(path_font,size,locale_to_symbols_code(),glif_infos);

        //space-symbol
        glif_infos[32].size={size/2,size/2};
        std::shared_ptr<unsigned char[]> image_data_32=std::shared_ptr<unsigned char[]>(new unsigned char[size/2*size/2]);
        for(std::size_t i=0;i<size/2*size/2;i++){
            image_data_32[i]=0;
        }
        glif_images[32]=Image(size/2,size/2,1,Image::Setting_image{},image_data_32);


        //cоздание атласа из глифов, вынести тут всё и разобрать))
        atlas= Image_atlas(glif_images);
        for(std::size_t i=0;i<atlas.data_fragment.size();i++){
            glif_infos[i].position=                 {atlas.data_fragment[i][0],atlas.data_fragment[i][1]};
            glif_infos[i].coordinate_transformation={atlas.data_fragment[i][2],atlas.data_fragment[i][3]};
        }
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    };
}
