#include "font.h"
namespace CeEngine {
    void converting_unicode_to_glyph_number(std::vector<int> &unicod){//временно,потом переписать для поддержки всех языков

        int offset=0;

        offset=1020-128;

        for(auto &U:unicod){
            if(U>=1020 && 1120>=U){
                U-=offset;
            }
        }
    };

    void string_to_unicod(const char * text_in, std::vector<int> * simbols_out){

        //std::cout<<": "<<text_in<<std::endl;

        std::mbstate_t state = std::mbstate_t();
        std::size_t len =std::mbsrtowcs(nullptr,&text_in, 0, &state);
        for(std::size_t i=simbols_out->size();i<len;i++){
            simbols_out->push_back({});
        }
        //std::cout<<"len::: "<<len<<std::endl;

        if(sizeof (wchar_t)==2){//хак-1 сp1251 =>utf-8 для винды
            auto namber_symbol=[](char symbol_1,char symbol_2){
                int symbol1=static_cast<int>(symbol_1);
                int symbol2=static_cast<int>(symbol_2);

                int out=0;

                if(symbol1==-70 && symbol2==91){out= 186;};
                if(symbol1==-47){out=  1120-(-symbol2-96);};
                if(symbol1==-48){out=  1120-(16+(-symbol2-48));};


                //std::cout<<symbol1<<" | "<<symbol2<<" ="<<out<<std::endl;
                return out;
            };
            std::string current_characters=text_in;
            std::size_t k = 0;
            for(std::size_t i=0;i< current_characters.size();i++){
                if(static_cast<int>(current_characters[i])<0){
                    (*simbols_out)[k]=namber_symbol( current_characters[i],current_characters[i+1]);
                     i++;
                }else{
                    if(current_characters[i]!=0){
                        (*simbols_out)[k]=current_characters[i];
                    }

                }
                k++;
            };
            simbols_out->resize(k);
        }

        if(sizeof (wchar_t)==4){//для линя
            std::mbsrtowcs(reinterpret_cast<wchar_t *>(simbols_out->data()),&text_in,len, &state);
        }

    //    for(auto &T:*simbols_out){
    //        //std::cout<<T<<std::endl;
    //    }
    };
}
