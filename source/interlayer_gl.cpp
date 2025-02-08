#include <interlayer_gl.h>

//var float
inline void   GL_layer::Uniform(GLint id_variable, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){
  glUniform4f(id_variable,v0,v1,v2,v3);
};
inline void   GL_layer::Uniform(GLint id_variable, GLfloat v0, GLfloat v1, GLfloat v2){
  glUniform3f(id_variable,v0,v1,v2);
};
inline void   GL_layer::Uniform(GLint id_variable, GLfloat v0, GLfloat v1){
  glUniform2f(id_variable,v0,v1);
};
inline void   GL_layer::Uniform(GLint id_variable, GLfloat v0){
  glUniform1f(id_variable,v0);
};

//array float
template<> inline void   GL_layer::Uniform<4>(GLint id_variable, GLsizei count, const GLfloat* value){
  glUniform4fv(id_variable,count,value);
};
template<> inline void   GL_layer::Uniform<3>(GLint id_variable, GLsizei count, const GLfloat* value){
  glUniform3fv(id_variable,count,value);
};
template<> inline void   GL_layer::Uniform<2>(GLint id_variable, GLsizei count, const GLfloat* value){
  glUniform2fv(id_variable,count,value);
};
template<> inline void   GL_layer::Uniform<1>(GLint id_variable, GLsizei count, const GLfloat* value){
  glUniform1fv(id_variable,count,value);
};


//mat float
template<> inline void  GL_layer::Uniform<4,4>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix4fv(id_variable, count, transpose,value);
};
template<> inline void  GL_layer::Uniform<4,3>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix4x3fv(id_variable, count, transpose,value);
};
template<> inline void  GL_layer::Uniform<4,2>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix4x2fv(id_variable, count, transpose,value);
};


template<> inline void  GL_layer::Uniform<3,4>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix3x4fv(id_variable, count, transpose,value);
};
template<> inline void  GL_layer::Uniform<3,3>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix3fv(id_variable, count, transpose,value);
};
template<> inline void  GL_layer::Uniform<3,2>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix3x2fv(id_variable, count, transpose,value);
};

template<> inline void  GL_layer::Uniform<2,4>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix2x4fv(id_variable, count, transpose,value);
};
template<> inline void  GL_layer::Uniform<2,3>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix2x3fv(id_variable, count, transpose,value);
};
template<> inline void  GL_layer::Uniform<2,2>(GLint id_variable, GLsizei count, GLboolean transpose, const GLfloat* value){
    glUniformMatrix2fv(id_variable, count, transpose,value);
};

//var int


//array int
template<> inline void   GL_layer::Uniform<4>(GLint id_variable, GLsizei count, const GLint* value){
    glUniform4iv(id_variable,count,value);
};
template<> inline void   GL_layer::Uniform<3>(GLint id_variable, GLsizei count, const GLint* value){
    glUniform3iv(id_variable,count,value);
};
template<> inline void   GL_layer::Uniform<2>(GLint id_variable, GLsizei count, const GLint* value){
    glUniform2iv(id_variable,count,value);
};
template<> inline void   GL_layer::Uniform<1>(GLint id_variable, GLsizei count, const GLint* value){
    glUniform1iv(id_variable,count,value);
};

//var double
inline void   GL_layer::Uniform(GLint id_variable, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3){
  glUniform4d(id_variable,v0,v1,v2,v3);
};
inline void   GL_layer::Uniform(GLint id_variable, GLdouble v0, GLdouble v1, GLdouble v2){
  glUniform3d(id_variable,v0,v1,v2);
};
inline void   GL_layer::Uniform(GLint id_variable, GLdouble v0, GLdouble v1){
  glUniform2d(id_variable,v0,v1);
};
inline void   GL_layer::Uniform(GLint id_variable, GLdouble v0){
  glUniform1d(id_variable,v0);
};

//array double
template<> inline void   GL_layer::Uniform<4>(GLint id_variable, GLsizei count, const GLdouble* value){
  glUniform4dv(id_variable,count,value);
};
template<> inline void   GL_layer::Uniform<3>(GLint id_variable, GLsizei count, const GLdouble* value){
  glUniform3dv(id_variable,count,value);
};
template<> inline void   GL_layer::Uniform<2>(GLint id_variable, GLsizei count, const GLdouble* value){
  glUniform2dv(id_variable,count,value);
};
template<> inline void   GL_layer::Uniform<1>(GLint id_variable, GLsizei count, const GLdouble* value){
  glUniform1dv(id_variable,count,value);
};


//mat double
template<> inline void  GL_layer::Uniform<4,4>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix4dv(id_variable, count, transpose,value);
};
template<> inline void  GL_layer::Uniform<4,3>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix4x3dv(id_variable, count, transpose,value);
};
template<> inline void  GL_layer::Uniform<4,2>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix4x2dv(id_variable, count, transpose,value);
};

template<> inline void  GL_layer::Uniform<3,4>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix3x4dv(id_variable, count, transpose,value);
};
template<> inline void  GL_layer::Uniform<3,3>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix3dv(id_variable, count, transpose,value);
};
template<> inline void  GL_layer::Uniform<3,2>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix3x2dv(id_variable, count, transpose,value);
};

template<> inline void  GL_layer::Uniform<2,4>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix2x4dv(id_variable, count, transpose,value);
};
template<> inline void  GL_layer::Uniform<2,3>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix2x3dv(id_variable, count, transpose,value);
};
template<> inline void  GL_layer::Uniform<2,2>(GLint id_variable, GLsizei count, GLboolean transpose, const GLdouble* value){
    glUniformMatrix2dv(id_variable, count, transpose,value);
};
