#ifndef CEPLATE_H
#define CEPLATE_H
#include <map>
#include <string>
#include <filesystem>

#include "CePrimitiv.h"
#include "Controller_Res_2d_form.h"
#include "universal_object.h"
namespace CeEngine {
Controller_Res_2d_form::Data F_plate( std::any data_){
    auto data=std::any_cast<std::map<std::string,Sheme_data>>(data_);
    return Controller_Res_2d_form::Data(data["pach_image"].data);
};

constexpr char name_CePlate[]="CePlate";
//class CePlate:public {
//    using CePrimitiv<name_CePlate,Material_RI,Controller_Res_2d_form,F_plate>::CePrimitiv;
//};

using CePlate=CePrimitiv<name_CePlate,Material_RI,Controller_Res_2d_form,F_plate>;
}// namespace CeEngine
#endif // CEPLATE_H
