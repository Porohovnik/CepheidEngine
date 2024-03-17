#ifndef CETEXT_H
#define CETEXT_H
#include <map>
#include <string>
#include <filesystem>

#include "CePrimitiv.h"
#include "Controller_Res_2d_text.h"
#include "universal_object.h"
namespace CeEngine {
Controller_Res_2d_text::Data F_Text(std::any  data_){
    auto data=std::any_cast<std::map<std::string,Sheme_data>>(data_);
    if(data["font"].data==""){
        std::cout<<"Problema, Font not set";
        assert(true);
    }

    return Controller_Res_2d_text::Data(data["font"].data,data["text"].data);
};


constexpr char name_CeText[]="CeText";
class CeText: public CePrimitiv<name_CeText,Material_T,Controller_Res_2d_text,F_Text>{
public:
    using CePrimitiv<name_CeText,Material_T,Controller_Res_2d_text,F_Text>::CePrimitiv;

    inline void change_text(char * ch){
        this->controller->template change_text<TYPE_TEXT::UNICOD>(this->id,ch);
    }

    inline void change_text_ansi(char * ch,int start,int size){
        this->controller->template change_text<TYPE_TEXT::ANSI>(this->id,ch,start,size);
    }
};
}// namespace CeEngine
#endif // CETEXT_H
