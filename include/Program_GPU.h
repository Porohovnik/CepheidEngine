#ifndef UNIFORM_PROGRAM_GPU_H
#define UNIFORM_PROGRAM_GPU_H
#include "Shader_program.h"
#include "interlayer_gl.h"

namespace CeEngine {
class Program_GPU:public GL_layer::Shader_program{
    GL_layer::GLint id_pos;
    GL_layer::GLint id_texture;
public:
    template<typename Data_Program>
    Program_GPU(Data_Program data_program):Shader_program(data_program){
        id_pos=this->Uniform_Location("i");
        id_texture=this->Uniform_Location("ourTexture");
        //std::cout<<glGetError()<<"/-sheder-/"<<"///id_texture:0:"<<id_texture<<":"<<id_pos<<std::endl;
    }

    inline void Bind(int base=-1) const{
        this->Use();
        //std::cout<<glGetError()<<"/-sheder-/"<<"///:1:"<<base<<":"<<std::endl;

        if(base!=-1){
            GL_layer::Uniform(id_texture,base);
            std::cout<<GL_layer::Get_Error()<<"/-sheder-/"<<"///:2:"<<base<<":"<<std::endl;
        }
    }
    inline void Bind(int base,int id) const{
        GL_layer::Uniform(id_pos,id);
        std::cout<<GL_layer::Get_Error()<<"/-sheder-/"<<"///:3:"<<base<<":"<<std::endl;
    }
};
}// namespace CeEngine
#endif // UNIFORM_PROGRAM_GPU_H
