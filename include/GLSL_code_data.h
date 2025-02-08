#ifndef GLSL_CODE_DATA_H
#define GLSL_CODE_DATA_H
#include <string>
#include <map>
#include "load_file.h"
#include "GL_type_to_cpp.h"

//.vert - a vertex shader
//.tesc - a tessellation control shader
//.tese - a tessellation evaluation shader
//.geom - a geometry shader
//.frag - a fragment shader
//.comp - a compute shader

namespace GL_layer {
class GLSL_code_data{
    std::map<int,std::string> data;
public:
    template<TYPE_SHADER type>
    void set_glsl_code(std::filesystem::path path){
        data[get_value_GL<type>()]=CeEngine::load_file(path);
    }

    std::map<int,std::string> & get_data(){
        return data;
    }
};
}// namespace CeEngine

#endif // GLSL_CODE_DATA_H
