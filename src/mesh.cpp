#include "mesh.h"
#include <iostream>
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

    std::shared_ptr<Mesh_data> sss_mesh(){
        std::vector<float> vertices{
                // Позиции         // Текстурные координаты

            -0.5, -0.5, 0, 0, 0,
            -0.496885, -0.168129, 0, 0, 0,
            -0.493769, 0.163743, 0, 0, 0,
            -0.490654, 0.495614, 0, 0, 0,
            -0.160436, 0.497076, 0, 0, 0,
            0.169782, 0.498538, 0, 0, 0,
            0.5, 0.5, 0, 0, 0,
            0.166667, 0.166667, 0, 0, 0,
            -0.166667, -0.166667, 0, 0, 0
        };
        std::vector<unsigned int> idddd{8, 1, 0, 2, 4, 3, 2, 7, 4, 5, 7, 6, 7, 2, 8, 1, 8, 2, 5, 4, 7};

        std::shared_ptr<Mesh_data> data=std::make_shared<Mesh_data>(std::move(vertices),std::move(idddd));
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
