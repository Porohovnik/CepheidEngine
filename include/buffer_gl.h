#ifndef BUFFER_GL_H
#define BUFFER_GL_H
#include<GL/glew.h>
#include <vector>
#include "interlayer_gl.h"

inline const int K=2;

template<class T,GLenum n>
class Buffer_GL{
    GL_layer::Buffer_id<n> buffer=0;
    std::size_t size=0;
    std::size_t max_size=1;
    int type=0;
    int bind_base=-1;
public:
    std::size_t get_max_size(){
        return max_size;
    }
    void expansion_buffer(std::size_t new_max_size);
    void expansion_not_save_data(std::size_t new_max_size){
        GL_layer::Bind_guard bind{buffer};
        GL_layer::initialization_buffer<n>(sizeof (T)*new_max_size, nullptr, type);
        max_size=new_max_size;
        glFinish();
    }
    void clear(){
         GL_layer::Bind_guard bind{buffer};
         GLuint i=0;
         GL_layer::clear_buffer<n>(&i);
    }
    Buffer_GL(){
        buffer=GL_layer::create_buffer<n>();
    }
    Buffer_GL(int bind_base_,int type_,std::size_t max_size_,T * array=nullptr):
    bind_base(bind_base_),type(type_),max_size(max_size_){
        buffer=GL_layer::create_buffer<n>();
        GL_layer::Bind_guard bind{buffer};
        GL_layer::initialization_buffer<n>(sizeof (T)*max_size, array, type);
    }
    ~Buffer_GL(){
        GL_layer::delete_buffer(buffer);
    }

    Buffer_GL(Buffer_GL&& buffer):buffer(buffer.buffer),bind_base(buffer.bind_base),type(buffer.type),max_size(buffer.max_size),size(buffer.size){}
    Buffer_GL(const Buffer_GL& buffer):buffer(buffer.buffer),bind_base(buffer.bind_base),type(buffer.type),max_size(buffer.max_size),size(buffer.size){}

    Buffer_GL & operator=(Buffer_GL && buffer_){
        return buffer_;
    }
    Buffer_GL & operator=(const Buffer_GL & buffer_){
        return buffer_;
    }

    inline int insert(std::size_t id,T * elements,int size_insert=1,int offset=0){
        changes(elements,id,id+size_insert,offset);
        return id;
    }
    inline bool changes(const T * changes,std::size_t begin,std::size_t end,std::size_t offset=0){
        if(end>size){
            size=end;
        }
        if(size>(max_size)){
            expansion_buffer(size+max_size*K);
        }
        GL_layer::Bind_guard bind(buffer);
        GL_layer::update_buffer<n>(begin,end,sizeof(T),changes,offset);
        return true;
    }
    void read_data(std::size_t begin,std::size_t end,std::vector<T> &data){//заменить на интераторы
        if(data.size()<(end-begin)){
            data.resize(end-begin);
        }
        GL_layer::read_buffer<n>(begin,end,sizeof (T),data.data());
    }
    T read_data(std::size_t pos){
        GL_layer::Bind_guard bind{buffer};
        T t=0;

        GL_layer::read_buffer<n>(pos,pos+1,sizeof (T),&t);
        return t;
    }
    inline void Bind(int bind_base_=-1) const{
        GL_layer::Bind_(buffer);
        GL_layer::BindBase(buffer,bind_base_);
    }
};
////////////////////
///
///
///
///
///////////////////
template<class T,GLenum n>
void  Buffer_GL<T,n>::expansion_buffer(std::size_t new_max_size){
    //std::cout<<"/-expansion-/"<<new_max_size<<std::endl;
    //создание буффера большего объёма
    auto temporary_buffer=GL_layer::create_buffer<n>();
    GL_layer::Bind_guard<n> bind_1{temporary_buffer};
    GL_layer::initialization_buffer<n>(sizeof (T)*new_max_size, nullptr, type);//выделение места для нового буффера
    GL_layer::Bind_guard bind_2{GL_layer::Buffer_id<GL_COPY_READ_BUFFER>{buffer}};//так надо
    //перенос данных из буфффера меньшего объма в новый
    GL_layer::copy_buffer<GL_COPY_READ_BUFFER,n>(max_size*sizeof (T));
    //удаления старого буффера
    GL_layer::delete_buffer(buffer);
    //новый буффер большего размера теперь главнный
    buffer=temporary_buffer;
    max_size=new_max_size;
    glFinish();
};
#endif // BUFFER_GL_H
