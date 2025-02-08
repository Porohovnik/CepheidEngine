#ifndef TYPE_RESOURCE_H
#define TYPE_RESOURCE_H
#include "ceobject.h"
#include "Controller_Win_storage.h"
#include "Controller_Res_2d_form.h"
#include "Controller_Res_2d_text.h"
#include "Controller_Res_2d_line.h"

template<template<class Object, class Win_storage,class ... Arg> class T>
using Milieu_=T<CeEngine::CeObject,Controller_Win_storage,
                         Controller_Res_2d_form
                        ,Controller_Res_2d_text
                        ,Controller_Res_2d_line<false>
>;

#endif // TYPE_RESOURCE_H
