#ifndef BEZIER_CURVES_H
#define BEZIER_CURVES_H
#include <list>

#include <glm/vec3.hpp>
#include <iostream>
glm::vec3 distanse_midle_point(glm::vec3 p1, glm::vec3 p2){
    return glm::vec3{(p1.x+p2.x)*0.5f,(p1.y+p2.y)*0.5f,(p1.z+p2.z)*0.5f};
}

bool operator==(glm::vec3 p0, glm::vec3 p1){
    //std::cout<<std::abs(p0.x-p1.x)<<"|"<<std::abs(p0.y-p1.y)<<"|"<<std::abs(p0.z-p1.z)<<std::endl;
    return ((std::abs(p0.x-p1.x)<=0.001f) && (std::abs(p0.y-p1.y)<=0.001f) && (std::abs(p0.z-p1.z)<=0.001f));
}

bool isstraight_line(glm::vec3 & p_begin, glm::vec3 &p_end, glm::vec3 &p_test){
    return std::abs((p_test.x - p_begin.x) / (p_end.x - p_begin.x) - (p_test.y - p_begin.y) / (p_end.y - p_begin.y)) <= 0.0001f;
}

template <typename ...Arg>
bool isStraight_line(glm::vec3 & p_begin, glm::vec3 &p_end, Arg & ... p_test){
    return (isstraight_line(p_begin,p_end,p_test) &&...);
}

static void cubicBez(glm::vec3 p0, glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,float tol,std::list<glm::vec3> & mesh_data){

        //прямая линия значит, а финальную точку всё равно добавит внешний интерфейс
        if((p1==p2) &&(p2==p3) &&(p3==p1)){
            mesh_data.emplace_back(p0);
            return;
        }
        if(isStraight_line(p0,p3,p1,p2)){
            mesh_data.emplace_back(p0);
            return;
        }


        glm::vec3 a=3.0f*p1-2.0f*p0-p3;
        glm::vec3 b=3.0f*p2-p0-2.0f*p3;

        float t=std::max(a.x*a.x,b.x*b.x) + std::max(a.y*a.y,b.y*b.y)+std::max(a.z*a.z,b.z*b.z);

        //проверка на возможность апроксимации линеей
        if(t<=tol){
            mesh_data.emplace_back(p0);
            if(!(p0==p1)){
               mesh_data.emplace_back(p1);
            }
            if(!(p1==p2) && !(p2==p3)){
               mesh_data.emplace_back(p2);
            }
            return;
        }

        auto m_0=distanse_midle_point(p0,p1);
        auto m_1=distanse_midle_point(p1,p2);
        auto m_2=distanse_midle_point(p2,p3);

        auto q_0=distanse_midle_point(m_0,m_1);
        auto q_1=distanse_midle_point(m_1,m_2);

        auto B_midle_bez=distanse_midle_point(q_0,q_1);

        cubicBez(p0,    m_0,  q_0, B_midle_bez, tol, mesh_data);
        cubicBez(B_midle_bez, q_1, m_2,  p3, tol,  mesh_data);
}


void CubicBez(glm::vec3 p0, glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,float tol,std::list<glm::vec3> & mesh_data){
    cubicBez(p0,p1,p2,p3,tol,mesh_data);
    mesh_data.emplace_back(p3);
}





#endif // BEZIER_CURVES_H
