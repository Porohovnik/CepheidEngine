#ifndef BUFFER_GL_H
#define BUFFER_GL_H
#include "Buffer_id.h"
#include "GL_type_to_cpp.h"
namespace  CeEngine{
template<class T,GL_layer::BUFFER_SETTING type,GL_layer::TYPE_BUFFER n>
class Buffer_GL{
    inline static const GL_layer::GLintptr K=2;

    GL_layer::Buffer_id<n> buffer;
    GL_layer::GLsizeiptr size=0;
    GL_layer::GLsizeiptr max_size=1;
public:
    inline std::size_t get_max_size()const noexcept{
        return max_size;
    }
    void expansion_buffer(GL_layer::GLsizeiptr new_max_size);
    void expansion_not_save_data(GL_layer::GLsizeiptr new_max_size){
        buffer.initialization_buffer_noBind(sizeof (T)*new_max_size, nullptr, type);
        max_size=new_max_size;
        //GL_layer::Finish();
    }
    inline void clear() {
         buffer.clear_buffer_noBind();
    }

    Buffer_GL(const Buffer_GL& )=delete;
    Buffer_GL(Buffer_GL&& )=delete;
    Buffer_GL & operator=(Buffer_GL &&)=delete;
    Buffer_GL & operator=(const Buffer_GL &)=delete;

    Buffer_GL(const T * array=nullptr,GL_layer::GLsizeiptr max_size_=0):buffer(sizeof (T)*max_size_, array, type),max_size(max_size_){}

    template<typename Array>
    Buffer_GL(const Array &array):buffer(sizeof (T)*array.size(), array.data(), type),max_size(array.size()){}//для последоовательных контейнеров

    inline bool insert(GL_layer::GLintptr id,const T * elements,GL_layer::GLsizeiptr size_insert=1,GL_layer::GLintptr offset=0){
        return  changes(elements,id,id+size_insert,offset);
    }

    inline bool changes(const T * changes,GL_layer::GLintptr begin,GL_layer::GLintptr end,GL_layer::GLintptr offset=0){
        if(end>size){
            size=end;
        }
        if(size>(max_size)){
            expansion_buffer(size+max_size*K);
        }
        buffer.update_buffer_noBind(begin,end,sizeof(T),changes,offset);
        return true;
    }

    void read_data(GL_layer::GLintptr begin,GL_layer::GLintptr end,T * elements) const{
        buffer.read_buffer_noBind(begin,end,sizeof (T),elements);
    }

    T read_data(GL_layer::GLintptr pos) const{
        T t=0;
        buffer.read_buffer_noBind(pos,pos+1,sizeof (T),&t);
        return t;
    }

    inline void Bind() const noexcept{
        buffer.Bind_();
    }

    inline void Bind(GL_layer::GLuint bind_base_) const noexcept{
        buffer.Bind_();
        buffer.BindBase(bind_base_);
    }

};
////////////////////
///
///
///
///
///////////////////
template<class T,GL_layer::BUFFER_SETTING type,GL_layer::TYPE_BUFFER n>
void  Buffer_GL<T,type,n>::expansion_buffer(GL_layer::GLsizeiptr new_max_size){
    //std::cout<<"/-expansion-/"<<new_max_size<<std::endl;
    //создание буффера большего объёма
    GL_layer::Buffer_id<n> temporary_buffer{static_cast<GL_layer::GLsizeiptr>(sizeof (T)) *new_max_size, nullptr, type};//выделение места для нового буффера

    buffer          .template Bind_set_point<GL_layer::TYPE_BUFFER::COPY_READ_BUFFER>(); //отсюда читаем
    temporary_buffer.template Bind_set_point<GL_layer::TYPE_BUFFER::COPY_WRITE_BUFFER>();//сюда записываем

    //перенос данных из буффера меньшего обёъма в новый
    GL_layer::copy_buffer<GL_layer::TYPE_BUFFER::COPY_READ_BUFFER,GL_layer::TYPE_BUFFER::COPY_WRITE_BUFFER>(max_size*sizeof (T));

    // std::cout<<GL_layer::Get_Error()<<"/-Buffer_GL-/"<< max_size<<"///::"<<new_max_size <<std::endl;
    //новый  буффер большего размера теперь главный
    //старый буффер должен сам умереть
    buffer.swap(temporary_buffer);
    max_size=new_max_size;
    GL_layer::Finish();
};
}// namespace CeEngine
#endif // BUFFER_GL_H
