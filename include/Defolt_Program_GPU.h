#ifndef DEFOLT_PROGRAM_GPU_H
#define DEFOLT_PROGRAM_GPU_H
#include "Shader_program.h"
#include "interlayer_gl.h"
namespace CeEngine {
class Defolt_Program_GPU:public GL_layer::Shader_program{
    GL_layer::GLint id_pos;
public:
    template<typename Data_Program>
    Defolt_Program_GPU(Data_Program & data_program):Shader_program(data_program){
        id_pos=this->Uniform_Location("i");
    }

    inline void  Bind(int base=-1) const{
        this->Use();
        //std::cout<<glGetError()<<"/-sheder-/"<<"///::"<<Program<<std::endl;
    }

    inline void  Bind(int base,int id) const{
        GL_layer::Uniform(id_pos,id);
    }
};
}// namespace CeEngine
#endif // DEFOLT_PROGRAM_GPU_H
