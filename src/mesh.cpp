#include "mesh.h"
#include <iostream>

#include <glm/vec2.hpp>
#include <glm/detail/func_geometric.inl>
namespace CeEngine {
    std::shared_ptr<Mesh_data> line_mesh(){
        std::vector<float> vertices{
                 0.0f,
                 0.0f,
        };
        std::shared_ptr<Mesh_data> data=std::make_shared<Mesh_data>(std::move(vertices));
        data->set_marking_vertices<GL_layer::TYPE_data::position>(1);

        return data;
    };

    struct Vec2_4{
        glm::vec2 k[4];

        auto & operator[](std::size_t i){
            return k[i];
        }
    };

    Vec2_4 get_point_quard(glm::vec2 P0, glm::vec2 P1,float width){
        Vec2_4 k;

        //получаем смещения
        float L =glm::distance(P0,P1);

        float K=width/L;

        float x_=-K*(P1.y-P0.y);
        float y_= K*(P1.x-P0.x);

        //вычесляем точки
        k[0]=glm::vec2( x_+P0.x, y_+P0.y);
        k[1]=glm::vec2(-x_+P0.x,-y_+P0.y);

        k[2]=glm::vec2( x_+P1.x, y_+P1.y);
        k[3]=glm::vec2(-x_+P1.x,-y_+P1.y);

        return k;
    }

    glm::vec2 get_point_corner(glm::vec2 P1,glm::vec2 P2,glm::vec2 P3,glm::vec2 P4){
        float t= (P1.x-P2.x)*(P3.y-P4.y) - (P3.x-P4.x)*(P1.y-P2.y);
        float a=P1.x*P2.y-P2.x*P1.y;
        float b=P3.x*P4.y-P4.x*P3.y;

        float x=(a*(P3.x-P4.x)-b*(P1.x-P2.x))/t;
        float y=(a*(P3.y-P4.y)-b*(P1.y-P2.y))/t;
        return glm::vec2(x,y);
    }

    inline void bild_vert(glm::vec2 point,std::vector<float> &vertices,unsigned int &id_current,std::vector<unsigned int> &idddd){
        vertices.emplace_back(point.x);
        vertices.emplace_back(point.y);
        vertices.emplace_back(0.0f);
        vertices.emplace_back(0.0f);
        vertices.emplace_back(0.0f);
        if(id_current>2){
            idddd.emplace_back(idddd[idddd.size()-2]);
            idddd.emplace_back(idddd[idddd.size()-2]);
        }
        idddd.emplace_back(id_current++);
    }


    void bild_quard(Vec2_4 points,std::vector<float> &vertices,unsigned int &id_current,std::vector<unsigned int> &idddd){
        std::size_t i=0;
        if(id_current!=0){
            i=2;
        }

        for (;i<4;i++) {
            bild_vert(points[i],vertices,id_current,idddd);
        }
    }

    void oblique_connection(glm::vec2 P0, glm::vec2 P1,std::vector<float> &vertices,unsigned int &id_current,std::vector<unsigned int> &idddd){
       bild_vert(P0,vertices,id_current,idddd);
       bild_vert(P1,vertices,id_current,idddd);
    }

    void direct_connection(glm::vec2 T_0, glm::vec2 T_1,glm::vec2 P0, glm::vec2 P1,std::vector<float> &vertices,unsigned int &id_current,std::vector<unsigned int> &idddd){
        bild_vert(T_0,vertices,id_current,idddd);
        bild_vert(T_1,vertices,id_current,idddd);
        bild_vert(P0,vertices,id_current,idddd);
        bild_vert(P1,vertices,id_current,idddd);
    }

    void bezier_connection(glm::vec2 T_0, glm::vec2 T_1,glm::vec2 P0, glm::vec2 P1,glm::vec2 P2, glm::vec2 P3,std::vector<float> &vertices,unsigned int &id_current,std::vector<unsigned int> &idddd){
        float t=0.0;
        int count=8;
        for(int z = 0; z < count+1; z++){
            glm::vec2 B_0=(1-t)*(1-t)*P0+2*t*(1-t)*T_0+t*t*P2;
            glm::vec2 B_1=(1-t)*(1-t)*P1+2*t*(1-t)*T_1+t*t*P3;
            bild_vert(B_0,vertices,id_current,idddd);
            bild_vert(B_1,vertices,id_current,idddd);

            t+=1.0/count;
        }
    }



    std::shared_ptr<Mesh_data> line_mesh(std::vector<float> &vertices,float width,TYPE_JOINT type){
        std::vector<float> vertices_line;
        std::vector<unsigned int> idddd;
        unsigned int id=0;
        std::size_t i=0;
        for (;i<vertices.size()-4;i+=2) {
            Vec2_4 quard_1=get_point_quard(glm::vec2(vertices[i],  vertices[i+1]),glm::vec2(vertices[i+2],vertices[i+3]),width);
            Vec2_4 quard_2=get_point_quard(glm::vec2(vertices[i+2],vertices[i+3]),glm::vec2(vertices[i+4],vertices[i+5]),width);
            bild_quard(quard_1,vertices_line,id,idddd);
            switch (type) {
                case TYPE_JOINT::TENSION :{};break;
                case TYPE_JOINT::OBLIQUE :{
                    oblique_connection(quard_2[0],quard_2[1],vertices_line,id,idddd);};break;
                case TYPE_JOINT::STRAIGHT:{
                    glm::vec2 t_= get_point_corner(quard_1[0],quard_1[2],quard_2[0],quard_2[2]);
                    glm::vec2 t_1=get_point_corner(quard_1[1],quard_1[3],quard_2[1],quard_2[3]);
                    direct_connection(t_,t_1,quard_2[0],quard_2[1],vertices_line,id,idddd);
                };break;
                case TYPE_JOINT::ROUNDED :{
                    glm::vec2 t_= get_point_corner(quard_1[0],quard_1[2],quard_2[0],quard_2[2]);
                    glm::vec2 t_1=get_point_corner(quard_1[1],quard_1[3],quard_2[1],quard_2[3]);
                    bezier_connection(t_,t_1,quard_1[2],quard_1[3],quard_2[0],quard_2[1],vertices_line,id,idddd);
                };break;
            }
        }
        Vec2_4 vec2=get_point_quard(glm::vec2(vertices[i],vertices[i+1]),glm::vec2(vertices[i+2],vertices[i+3]),width);
        bild_quard(vec2,vertices_line,id,idddd);

        std::shared_ptr<Mesh_data> data=std::make_shared<Mesh_data>(std::move(vertices_line),std::move(idddd));
        data->set_marking_vertices<GL_layer::TYPE_data::position>(3);
        data->set_marking_texture <0>(2);

        return data;
    };

    std::shared_ptr<Mesh_data> plate_mesh(){
        std::vector<float> vertices{
                // Позиции         // Текстурные координаты

            -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.0f,  0.0f, 1.0f

        };
        std::vector<unsigned int> idddd{0,1,2,2,3,0};

        std::shared_ptr<Mesh_data> data=std::make_shared<Mesh_data>(std::move(vertices),std::move(idddd));
        data->set_marking_vertices<GL_layer::TYPE_data::position>(3);
        data->set_marking_texture <0>(2);

        return data;
    };

    std::shared_ptr<Mesh_data> cube_mesh(){
        std::vector<float> vertices{
                // Позиции         // Текстурные координаты
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        std::shared_ptr<Mesh_data> data=std::make_shared<Mesh_data>(std::move(vertices));
        data->set_marking_vertices<GL_layer::TYPE_data::position>(3);
        data->set_marking_texture <0>(2);

        return data;
    }

    std::shared_ptr<Mesh_data> skybox_mesh(){
        std::vector<float> vertices{
                // Позиции         // Текстурные координаты
           -1.0f,  1.0f, -1.0f,
           -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
           -1.0f,  1.0f, -1.0f,

           -1.0f, -1.0f,  1.0f,
           -1.0f, -1.0f, -1.0f,
           -1.0f,  1.0f, -1.0f,
           -1.0f,  1.0f, -1.0f,
           -1.0f,  1.0f,  1.0f,
           -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

           -1.0f, -1.0f,  1.0f,
           -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
           -1.0f, -1.0f,  1.0f,

           -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
           -1.0f,  1.0f,  1.0f,
           -1.0f,  1.0f, -1.0f,

           -1.0f, -1.0f, -1.0f,
           -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
           -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };


        std::shared_ptr<Mesh_data> data=std::make_shared<Mesh_data>(std::move(vertices));
        data->set_marking_vertices<GL_layer::TYPE_data::position>(3);
        return data;
    };
}
