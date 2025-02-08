#ifndef CELINE_H
#define CELINE_H
#include "CePrimitiv.h"
#include "Controller_Res_2d_line.h"
#include "universal_object.h"
namespace CeEngine {
constexpr char name_Line[]="CeLine";
//class CeLine: public {
//    using CePrimitiv<name_Line,Material_L,Controller_Res_2d_line,nullptr>::CePrimitiv;
//};

using CeLine = CePrimitiv<name_Line,Material_L,Controller_Res_2d_line<false>,nullptr>;
//using CeLine_xy = CePrimitiv<name_Line,Material_L,Controller_Res_2d_line<true>,nullptr>;
}// namespace CeEngine

#endif // CELINE_H
