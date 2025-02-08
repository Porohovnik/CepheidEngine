#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <string>

#include <cstdio>
#include <filesystem>
#include <array>
#include <optional>

#include "tuple_utl.h"
#include "interlayer_gl.h"




//разбить файл
#include <sstream>
#include <fstream>
#include <filesystem>
std::string load_file(std::filesystem::path path){
    std::cout<<":::" <<path.string() .c_str()<<std::endl;
    std::string data;
    std::ifstream file{path};
    file.exceptions(std::ifstream::badbit);
    try{
        std::stringstream file_stream;
        file_stream << file.rdbuf();

        data=file_stream.str();

        file.close();
    }
    catch (std::ifstream::failure e){
        std::cout<<data<<std::endl;
        std::cout<<"ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    return data;
}

template<std::size_t n>
std::array<std::string,n> load_files(std::array<std::filesystem::path,n> pachs){
    std::array<std::string,n> data;
    for(int i=0;i<n;i++){
        data[i]=load_file(pachs[i]);
    }
    return data;
}


uint load_shader_program(std::string data,uint type){
    if(data.size()==0){
        return 0;
    }

    GLchar infoLog[512];
    GLint success;
    const GLchar * code=data.c_str();

    GLuint program_sheder = glCreateShader(type);
    glShaderSource(program_sheder, 1,&code, nullptr);
    glCompileShader(program_sheder);

    // Print compile errors if any
    glGetShaderiv(program_sheder, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(program_sheder, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return program_sheder;
}

int check_program(GLuint Program){
    GLchar infoLog[512];
    GLint success;
    glGetProgramiv(Program, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(Program, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    return success;
}

uint load_shader_program(std::string data,uint type);
int check_program(GLuint Program);

template<int n>
inline GLuint create_shader(std::array<GLuint,n> program_sheder){
    GLuint Program = glCreateProgram();
    for(auto&T:program_sheder){
        if(T!=-1){
            glAttachShader(Program,T);
        }
    }
    glLinkProgram(Program);
    for(auto&T:program_sheder){
        if(T!=-1){
            glDeleteShader(T);
        }
    }
    check_program(Program);
    std::cout<<"^ "<<Program<<std::endl;

    return Program;
}

template<typename ...Arg>
inline auto preparatioon_shader_program(Arg...arg){
    std::array<GLuint,6> type_sheder={GL_VERTEX_SHADER,GL_FRAGMENT_SHADER,GL_GEOMETRY_SHADER,GL_TESS_CONTROL_SHADER,GL_TESS_EVALUATION_SHADER,GL_COMPUTE_SHADER};
    int i=0;
    std::array<GLuint,sizeof... (Arg)> array{load_shader_program(arg, type_sheder[i++])...};
    return create_shader<sizeof... (Arg)>(std::forward<std::array<GLuint,sizeof... (Arg)>>(array));
}


inline GLint Uniform_Location(GLuint Program,const char * name){
    return glGetUniformLocation(Program,name);
}

class Defolt_Uniform{
    GLint id_pos;
public:
    Defolt_Uniform(GLuint Program){
        id_pos=Uniform_Location(Program,"i");
    }
    inline void operator()(int base) const{

    }
    inline void operator()(int base,int id) const{
        GL_layer::Uniform(id_pos,id);
        //std::cout<<glGetError()<<"/-sheder-/"<<"///::"<<id<<":"<<std::endl;
    }
};

template<class Un_fun=Defolt_Uniform>
class Shader{
    GLuint Program;
    std::optional<Un_fun> uniform_fun;

public:
    template<typename T>
    Shader(T data){
        Program=tutl::ArrayArg(std::forward<T>(data),[](auto ...t){return  preparatioon_shader_program(t...);});
        use();
        uniform_fun.emplace(Program);
    }



    GLuint program()const {
        return this->Program;
    }

    void use() const{
        glUseProgram(this->Program);
    }
    inline void  Bind(int base=-1) const{
        use();
        (*uniform_fun)(base);
        //std::cout<<glGetError()<<"/-sheder-/"<<"///::"<<Program<<std::endl;
    }

    inline void  Bind(int base,int id) const{
        (*uniform_fun)(base,id);
    }

};



#endif // SHADER_H
