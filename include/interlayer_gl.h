#ifndef INTERLAYER_GL_H
#define INTERLAYER_GL_H
// GLEW нужно подключать до GLFW.

#define GLFW_INCLUDE_NONE
#include<GL/glew.h>
//#define STB_IMAGE_IMPLEMENTATION

#include <GL/gl.h>
// GLFW
#include <GLFW/glfw3.h>

#include <array>
#include <iostream>
#include <algorithm>
#include <numeric>
#include "tuple_utl.h"
#include <vector>

using uint =unsigned int;
namespace GL_layer{

template<GLenum n>
struct Buffer_id{
    GLenum buffer=0;
    Buffer_id(GLenum buffer_):buffer(buffer_){}
    inline operator const unsigned int() const noexcept{
        return buffer;
    }
};

template<GLenum n>
inline void Bind_(const Buffer_id<n> buffer) noexcept{
    if constexpr(n==GL_SHADER_STORAGE_BUFFER){
        glBindBuffer(n, buffer);
    }
    if constexpr(n==GL_DRAW_INDIRECT_BUFFER ||
                 n==GL_COPY_READ_BUFFER     ||
                 n==GL_COPY_WRITE_BUFFER    ||
                 n==GL_ARRAY_BUFFER         ||
                 n==GL_ELEMENT_ARRAY_BUFFER
                 ){
        glBindBuffer(n, buffer);

    }
    if constexpr(n==GL_FRAMEBUFFER){
        glBindFramebuffer(n, buffer);
    }
    if constexpr(n==GL_RENDERBUFFER){
        glBindRenderbuffer(n, buffer);
    }

    if constexpr(n==GL_TEXTURE_1D ||
                 n==GL_TEXTURE_2D ||
                 n==GL_TEXTURE_3D ||
                 n==GL_TEXTURE_2D_ARRAY ||
                 n==GL_TEXTURE_CUBE_MAP) {
        glBindTexture(n, buffer);
    }
    if constexpr(n==GL_VERTEX_ARRAY){
        glBindVertexArray(buffer);
    }
}

template<GLenum n>
inline void BindBase(const Buffer_id<n> buffer, GLenum bind_base){
    if constexpr(n==GL_SHADER_STORAGE_BUFFER){
        glBindBufferBase(n, bind_base, buffer);
    }
}

template<GLenum n>
class Bind_guard{
public:
    inline explicit Bind_guard(const Buffer_id<n> buffer) noexcept{
        Bind_<n>(buffer);
    }
    inline ~Bind_guard() noexcept{
        Bind_<n>(0);
    }
    Bind_guard(const Bind_guard&) = delete;
    Bind_guard& operator=(const Bind_guard&) = delete;
};

template<GLenum n,typename T>
class Map_buffer_guard{
    void * ptr=nullptr;
public:
    inline explicit Map_buffer_guard(GLenum type) noexcept{
       ptr=glMapBuffer(n, type);
    }
    inline ~Map_buffer_guard() noexcept{
        glUnmapBuffer(n);
    }

    inline T * data() const{
        return static_cast<T*>(ptr);
    }

    inline T& operator [](std::size_t index){
        return static_cast<T*>(ptr)[index];
    }

    Map_buffer_guard(const Map_buffer_guard&) = delete;
    Map_buffer_guard& operator=(const Map_buffer_guard&) = delete;
};


template<GLenum n>
inline void attach_to_freambuffer_(Buffer_id<n> buffer,GLuint bind_frame,GLuint bind_point) noexcept{
    if constexpr(n==GL_TEXTURE_1D){
       glFramebufferTexture1D(bind_frame, bind_point, n, buffer, 0);
    }
    if constexpr(n==GL_TEXTURE_2D){
       glFramebufferTexture2D(bind_frame, bind_point, n, buffer, 0);
    }
    if constexpr(n==GL_TEXTURE_3D){
       //glFramebufferTexture3D(bind_frame, bind_point, POINT_TEX, Textur,0);
    }
    if constexpr(n==GL_RENDERBUFFER){
       glFramebufferRenderbuffer(bind_frame, bind_point, n, buffer);
    }
}

template<GLenum n>
inline Buffer_id<n> create_buffer(){
    GLenum buffer=0;
    if constexpr(n==GL_SHADER_STORAGE_BUFFER ||
                 n==GL_DRAW_INDIRECT_BUFFER  ||
                 n==GL_COPY_READ_BUFFER     ||
                 n==GL_COPY_WRITE_BUFFER    ||
                 n==GL_ARRAY_BUFFER          ||
                 n==GL_ELEMENT_ARRAY_BUFFER){
        glGenBuffers(1, &buffer);
    }
    if constexpr(n==GL_VERTEX_ARRAY){
        glGenVertexArrays(1,&buffer);
    }
    if constexpr(n==GL_FRAMEBUFFER){
        glGenFramebuffers(1,&buffer);
    }
    if constexpr(n==GL_RENDERBUFFER){
        glGenRenderbuffers(1,&buffer);
    }

    return buffer;
}

template<GLenum n>
inline void delete_buffer(const Buffer_id<n> buffer_){
    glGetError();
    GLenum buffer=buffer_;
    if constexpr(n==GL_SHADER_STORAGE_BUFFER ||
                 n==GL_DRAW_INDIRECT_BUFFER  ||
                 n==GL_COPY_READ_BUFFER     ||
                 n==GL_COPY_WRITE_BUFFER    ||
                 n==GL_ARRAY_BUFFER          ||
                 n==GL_ELEMENT_ARRAY_BUFFER){
        glDeleteBuffers(1, &buffer);
   // std::cout<<glGetError()<<"/-delete-/"<<"///::"<<buffer<<std::endl;
    }
    if constexpr(n==GL_VERTEX_ARRAY){
        glDeleteVertexArrays(1,&buffer);
    }
    if constexpr(n==GL_FRAMEBUFFER){
        glDeleteFramebuffers(1,&buffer);
    }
    if constexpr(n==GL_RENDERBUFFER){
        glDeleteRenderbuffers(1,&buffer);
    }
};




inline void Finish(){
      glFinish();
}

template<GLenum n>
inline void clear_buffer(const void *data=nullptr,GLenum internalformat=GL_R32UI, GLenum format=GL_RED_INTEGER, GLenum type=GL_UNSIGNED_INT){
      glClearBufferData(n,internalformat,format,type,data);
}

template<GLenum n>
inline void initialization_buffer(GLuint size_bate,void * data,GLuint type_update=GL_STATIC_DRAW){
    glBufferData(n,size_bate,data,type_update);
}

template<GLenum n>
inline void update_buffer(GLuint begin,GLuint end,GLuint size_byte,const void * data,GLuint offset=0){
    glBufferSubData(n,offset+(size_byte)*begin,offset+(size_byte)*(end-begin),data);
}

template<GLenum readtarget,GLenum writetarget>
inline void copy_buffer(GLuint size_byte,GLuint read_offset=0, GLuint write_offset=0){
    glCopyBufferSubData(readtarget, writetarget, read_offset, write_offset, size_byte);
}
template<GLenum n>
inline void read_buffer(std::size_t begin,std::size_t end,GLuint size_byte,void * data){
    glGetBufferSubData(n,begin*size_byte,(end-begin)*size_byte,data);
}

//Texture


using Type_filter= tutl::Type_to_data<
    tutl::KAAT<GL_TEXTURE_WRAP_R,int>,
    tutl::KAAT<GL_TEXTURE_WRAP_T,int>,
    tutl::KAAT<GL_TEXTURE_WRAP_S,int>,
    tutl::KAAT<GL_TEXTURE_SWIZZLE_RGBA,int>,
    tutl::KAAT<GL_TEXTURE_SWIZZLE_A,int>,
    tutl::KAAT<GL_TEXTURE_SWIZZLE_B,int>,
    tutl::KAAT<GL_TEXTURE_SWIZZLE_G,int>,
    tutl::KAAT<GL_TEXTURE_SWIZZLE_R,int>,
    tutl::KAAT<GL_TEXTURE_MAX_LEVEL,int>,
    tutl::KAAT<GL_TEXTURE_MAX_LOD,int>,
    tutl::KAAT<GL_TEXTURE_MIN_LOD,int>,
    tutl::KAAT<GL_TEXTURE_MAG_FILTER,int>,
    tutl::KAAT<GL_TEXTURE_MIN_FILTER,int>,
    tutl::KAAT<GL_TEXTURE_LOD_BIAS,int>,
    tutl::KAAT<GL_TEXTURE_COMPARE_MODE,int>,
    tutl::KAAT<GL_TEXTURE_COMPARE_FUNC,int>,
    tutl::KAAT<GL_TEXTURE_BORDER_COLOR,int>,
    tutl::KAAT<GL_TEXTURE_BASE_LEVEL,int>,
    tutl::KAAT<GL_DEPTH_STENCIL_TEXTURE_MODE,int>
>;



template<GLenum n>
inline Buffer_id<n> create_texture_buffer(){
    GLuint textur=0;
    glGenTextures(1,&textur);
    return textur;
}

template<GLenum n, typename Image>
inline void TexImage2D(Image & image, int level=0,int board=0){
    glTexImage2D(n, level, image.type_pixel_grid , image.widths, image.heights, board, image.type_pixel_grid, GL_UNSIGNED_BYTE, image.image.get());
}



template<GLenum n>
inline void delete_texture_buffer(Buffer_id<n> textur){
    glDeleteTextures(1,&textur.buffer);
}

const uint COUT_TYPE=4;
const uint COUT_TYPE_TEX_COORDS=8;

template<typename T>
inline void set_atribute_Mesh_buffer(const T &info){
    auto & info_mesh = info.elements_cout;
    auto & info_mesh_text_cord=info.tex_cord_cout;

    uint cout_offset=0;
    uint offset=std::accumulate(info_mesh.begin(),info_mesh.end(),0) +
                std::accumulate(info_mesh_text_cord.begin(),info_mesh_text_cord.end(),0);


    for(uint i=0;i<COUT_TYPE;i++){
        if(info_mesh[i]){
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i,info_mesh[i], GL_FLOAT, GL_FALSE, offset*sizeof(float),reinterpret_cast<void*>(cout_offset*sizeof(float)));
            cout_offset+=info_mesh[i];
        }
    }

    for(uint i=0;i<COUT_TYPE_TEX_COORDS;i++){
        if(info_mesh_text_cord[i]){
            glEnableVertexAttribArray(i+COUT_TYPE);
            glVertexAttribPointer(i+COUT_TYPE,info_mesh_text_cord[i], GL_FLOAT, GL_FALSE, offset*sizeof(float),reinterpret_cast<void*>(cout_offset*sizeof(float)));
            cout_offset+=info_mesh_text_cord[i];
        }
    }
}

template<int POINT_TEX=GL_TEXTURE_2D>
inline void set_atribute_texture_buffer(Type_filter & filter_atribute){
    auto data=filter_atribute.get_data();
    tutl::TupleForeach(data,[](auto &t){
        if(t.data.has_value()){
            glTexParameteri(POINT_TEX,t.type,*t.data );
        }
    });
}

inline void active_texture(int base=0){
    glActiveTexture(static_cast<unsigned int>(GL_TEXTURE0+base));
}


template<int POINT_TEX=GL_TEXTURE_2D>
inline void generate_mipmap(bool mipmap=false){
    if(mipmap){
        glGenerateMipmap(POINT_TEX);
    }
}

template<GLenum type_sheme_pixel=GL_RGB,GLenum type_byte=GL_UNSIGNED_BYTE>
inline void read_pixel(std::size_t x,std::size_t y,std::size_t weight,std::size_t height,std::vector<GLubyte> & data){
    std::size_t bayt_pixel=1;
    if constexpr(type_sheme_pixel==GL_RGB){
        bayt_pixel=3;
    }
    data.resize((weight)*(height+1)*bayt_pixel);

    read_pixel<type_sheme_pixel,type_byte>(x,y,weight,height,data.data());
}
template<GLenum type_sheme_pixel=GL_RGB,GLenum type_byte=GL_UNSIGNED_BYTE>
inline void read_pixel(std::size_t x,std::size_t y,std::size_t weight,std::size_t height,GLubyte *  data){
    glReadPixels(x,y,weight,height,type_sheme_pixel,type_byte,data);
    std::cout<<glGetError<<std::endl;
}


//Sheder

//var float
inline void   Uniform(GLint id_variable, GLfloat v0);
inline void   Uniform(GLint id_variable, GLfloat v0, GLfloat v1);
inline void   Uniform(GLint id_variable, GLfloat v0, GLfloat v1, GLfloat v2);
inline void   Uniform(GLint id_variable, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
//array float
template<int i>
inline void   Uniform(GLint id_variable, GLsizei count, const GLfloat* value);
//mat float
template<int i,int j>
inline void  Uniform(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value);

//var int
inline void   Uniform(GLint id_variable, GLint v0, GLint v1, GLint v2, GLint v3){
  glUniform4i(id_variable,v0,v1,v2,v3);
}
inline void   Uniform(GLint id_variable, GLint v0, GLint v1, GLint v2){
  glUniform3i(id_variable,v0,v1,v2);
}
inline void   Uniform(GLint id_variable, GLint v0, GLint v1){
  glUniform2i(id_variable,v0,v1);
}
inline void   Uniform(GLint id_variable, GLint v0){
 glUniform1i(id_variable,v0);
}

//array int
template<int i>
inline void  Uniform(GLint id_variable, GLsizei count, const GLint* value);

//var double
inline void   Uniform(GLint id_variable, GLdouble x);
inline void   Uniform(GLint id_variable, GLdouble x, GLdouble y);
inline void   Uniform(GLint id_variable, GLdouble x, GLdouble y, GLdouble z);
inline void   Uniform(GLint id_variable, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
//array double
template<int i>
inline void  Uniform(GLint id_variable, GLsizei count, const GLdouble* value);
//mat double
template<int i,int j>
inline void  Uniform(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value);


}
#endif // INTERLAYER_GL_H
