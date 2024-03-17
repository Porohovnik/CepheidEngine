#ifndef IDENTIFICATION_H
#define IDENTIFICATION_H
namespace CeEngine {
struct Identification{
    int id=0;
    template<typename  I>
    Identification(I id_):id(id_){}
    Identification(){}

    template<typename  I>
    operator I(){
        return id;
    }
    template<typename  I>
    bool operator ==(I i_){
        return i_==id;
    }
    bool operator ==(Identification i_){
        return i_.id==id;
    }

    inline int get_id() const{
        return id;
    }
};
}// namespace CeEngine
#endif // IDENTIFICATION_H
