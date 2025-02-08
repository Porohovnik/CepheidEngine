#ifndef POSITION_H
#define POSITION_H

#include <glm/vec3.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

//enum POINT_ATTACH{TOP,BOTTOM,RIGHT,LEFT};

#include <iostream>
namespace CeEngine{
class  Position{

    using Weight=std::size_t;
    using Height=std::size_t;

    //размер объекта 8байт+8байт
    //размер области 8байт
    //объекты в области (8байт+8байт)*N
    //принадлежность к области
    //растояние между объектами, при соединение 16 байт
    //метод соединения первая вставка и следующие

    glm::vec3 position_world{0.0f};

    glm::vec3 scale_object{1.0f};
    glm::mat4 current_rote{1.0f};

    glm::mat4 * projection=nullptr;

    const Position * view=nullptr;
public:

    //в итоговый вектор приходит часть от двух, там где _b 0, там t1,  иначе t2
    inline static glm::vec3 to_vec(glm::bvec3 _b,glm::vec3 t1,glm::vec3 t2){
        return t1*(glm::vec3{_b^true})+t2*glm::vec3{_b};
    }



    Position()=default;

    Position(glm::vec3 position_world_,glm::vec3  scale_object_=glm::vec3{1.0f}): position_world(position_world_),scale_object(scale_object_){}
    Position(float x,float y,float z): position_world(x,y,z){}
    Position(float scale): scale_object(scale,scale,scale){}
    Position(float x,float y,float z,float scale_x,float scale_y,float scale_z=1.0f):position_world(x,y,z),scale_object(scale_x,scale_y,scale_z){}

    //премещения объекта

    inline void set_view( const Position * view_){
        view=view_;
    }
    inline void set_projection(glm::mat4 * projection_){
        projection=projection_;
    }

    //функции изменения
    inline void move(glm::vec3 position_world_){//неважен порядок
        position_world+=position_world_;
    }
    inline void new_move(glm::vec3 position_world_){//неважен порядок
        position_world=position_world_;
    }


    inline void scale_offset(glm::vec3 && scale, glm::vec3 && centre){
        this->scale(std::forward<glm::vec3>(scale));
        //смещение
        this->move({scale_object[0]*(scale[0]-1)*centre[0],
                    scale_object[1]*(scale[1]-1)*centre[1],
                    scale_object[2]*(scale[2]-1)*centre[2]});
    }

    inline void scale(glm::vec3 scale){
        scale_object[0]*=scale[0];
        scale_object[1]*=scale[1];
        scale_object[2]*=scale[2];
    }

    inline void scale_add_offset(glm::vec3 && scale, glm::vec3 && centre){
        this->scale_add(std::forward<glm::vec3>(scale));
        //смещение
        this->move({(scale[0])*centre[0],
                    (scale[1])*centre[1],
                    (scale[2])*centre[2]});
    }

    inline void scale_add(glm::vec3 scale){
        scale_object[0]+=scale[0];
        scale_object[1]+=scale[1];
        scale_object[2]+=scale[2];
    }

    inline void new_scale_offset(glm::vec3 && scale, glm::vec3 && centre){//неважен порядок
        this->new_scale(std::forward<glm::vec3>(scale));
        //смещение
        this->new_move({(scale[0]-1)*centre[0],
                    (scale[1]-1)*centre[1],
                    (scale[2]-1)*centre[2]});
    }

    inline void new_scale(glm::vec3 scale){//неважен порядок
        scale_object=scale;
    }

    inline void rotete(glm::vec3 && for_mom,float rad){//порядок очень очень важен
        current_rote=glm::rotate(current_rote,glm::radians(rad), for_mom);
    }

    inline glm::mat4 pos() const{
        glm::mat4 mat_{1.0f};

        mat_[0][0]=scale_object[0];
        mat_[1][1]=scale_object[1];
        mat_[2][2]=scale_object[2];

        mat_[3][0]=position_world[0];
        mat_[3][1]=position_world[1];
        mat_[3][2]=position_world[2];

        if(projection && view){
            return *projection*view->pos()*(mat_*current_rote);
        }
        if(view){
            return view->pos()*(mat_*current_rote);
        }
        if(projection){
            return *projection*(mat_*current_rote);
        }
        return mat_*current_rote;
    }


    inline glm::vec3 get_position_world()const noexcept{
        return position_world;
    }
    inline glm::vec3 get_scale_object() const noexcept{
        return scale_object;
    }

    inline const Position * get_position() const noexcept {
        return this;
    }

    void print() const{
        auto mat_=pos();
        std::cout<<"####################################"<<std::endl;
        std::cout<<"|"<<mat_[0][0]<<"|"<<mat_[1][0]<<"|"<<mat_[2][0]<<"|"<<mat_[3][0]<<"|"<<std::endl;
        std::cout<<"|"<<mat_[0][1]<<"|"<<mat_[1][1]<<"|"<<mat_[2][1]<<"|"<<mat_[3][1]<<"|"<<std::endl;
        std::cout<<"|"<<mat_[0][2]<<"|"<<mat_[1][2]<<"|"<<mat_[2][2]<<"|"<<mat_[3][2]<<"|"<<std::endl;
        std::cout<<"|"<<mat_[0][3]<<"|"<<mat_[1][3]<<"|"<<mat_[2][3]<<"|"<<mat_[3][3]<<"|"<<std::endl;
        std::cout<<"#####################################"<<std::endl;

    }

};
}// namespace CeEngine
#endif // POSITION_H
