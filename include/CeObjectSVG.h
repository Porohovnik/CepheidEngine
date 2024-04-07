#ifndef CEOBJECTSVG_H
#define CEOBJECTSVG_H
#include "ceobject.h"
#include "CePlate.h"

#include "SVG.h"
#include "bezier_curves.h"


#include "utiliti_json.h"
namespace CeEngine {
class CeObjectSVG: public CeObject{
    std::list<CePlate> shapes;

public:
    struct Data_initialization{
        std::filesystem::path path;
        Color color;
        Position pos{1.0f};
        Data_initialization(std::filesystem::path path_,Color color_,Position pos_):path(path_),color(color_),pos(pos_){}
        Data_initialization(std::filesystem::path path_):path(path_){}
    };


    struct DataObject_2d{
        inline static std::size_t id_global_mesh=0;

        Color color;
        std::shared_ptr<Mesh_data> data;
        glm::vec2 minmax_x;
        glm::vec2 minmax_y;
        Position pos{1.0f};

        operator CePlate::Data(){
            return CePlate::Data{data,std::to_string(id_global_mesh++)+"_SVG",color,pos};
        }
    };


    template<typename Engine>
    CeObjectSVG(Engine *win_,typename Engine::List * celist_,Data_initialization data){
        SVG svg{data.path};

        std::vector<DataObject_2d> data_2d;

        glm::vec2 minmax_x_all={100000000.0f,-100000000.0f};
        glm::vec2 minmax_y_all={100000000.0f,-100000000.0f};

        for (auto &SVG_shape:svg.objects) {
            std::cout<<"******************************************"<<std::endl;
            std::list<glm::vec3> data_point;
            std::vector<std::pair<uint,uint>> contour;

            uint begin_=0;
            for (auto &Data:SVG_shape.bezie_curves_data) {
                for (auto &Counter:Data) {
                    cubicBez(Counter.p0,Counter.p1,Counter.p2,Counter.p3,2.0f,data_point);
                    //std::cout<<Counter.p0[0]<<":"<<Counter.p0[1]<<"|"<<Counter.p1[0]<<":"<<Counter.p1[1]<<"|"<<Counter.p2[0]<<":"<<Counter.p2[1]<<"|"<<Counter.p3[0]<<":"<<Counter.p3[1]<<std::endl;
                }

                contour.emplace_back(begin_,data_point.size()-1);
                begin_=data_point.size();
                std::cout<<" =================================="<<std::endl;
            }

            std::vector<float> data_tex(data_point.size()*2,0.0f);
            for(auto &Data:data_point){
                Data.y=-Data.y;
                //data_mesh->vertex[i]=-data_mesh->vertex[i];
                //data_mesh->vertex[i+1]=-data_mesh->vertex[i+1];
            }


            auto data_mesh=std::make_shared<Mesh_data>(data_point,contour,data_tex,5);
            data_mesh->set_marking_vertices<GL_layer::TYPE_data::position>(3);            
            data_mesh->set_marking_texture <0>(2);

            auto minmax_array=data_mesh->normalaize<GL_layer::TYPE_data::position>();

            for(std::size_t i=0;i<data_mesh->vertex.size();i+=5){
                //data_mesh->vertex[i]=-data_mesh->vertex[i];
                //data_mesh->vertex[i+1]=-data_mesh->vertex[i+1];
            }
            minmax_x_all[0]=std::min(minmax_array[0][0],minmax_x_all[0]);
            minmax_y_all[0]=std::min(minmax_array[1][0],minmax_y_all[0]);

            minmax_x_all[1]=std::max(minmax_array[0][1],minmax_x_all[1]);
            minmax_y_all[1]=std::max(minmax_array[1][1],minmax_y_all[1]);

            std::cout<<"min : "<<minmax_array[0][0]<<"|"<<minmax_array[1][0]<<std::endl;
            std::cout<<"max : "<<minmax_array[0][1]<<"|"<<minmax_array[1][1]<<std::endl;
            data_2d.emplace_back(DataObject_2d{SVG_shape.color,data_mesh,minmax_array[0],minmax_array[1],Position{1.0f}});
        }
        std::cout<<"min_all : "<<minmax_x_all[0]<<"|"<<minmax_y_all[0]<<std::endl;
        std::cout<<"max_all : "<<minmax_x_all[1]<<"|"<<minmax_y_all[1]<<std::endl;

        float lenght_all_x=std::abs(minmax_x_all[1]-minmax_x_all[0]);
        float lenght_all_y=std::abs(minmax_y_all[1]-minmax_y_all[0]);

        for (auto &Data:data_2d) {

            Data.pos.new_scale(glm::vec3{(Data.minmax_x[1]-Data.minmax_x[0])/lenght_all_x,
                                         (Data.minmax_y[1]-Data.minmax_y[0])/lenght_all_y,
                                         1.0f});

            Data.pos.new_move(glm::vec3{(Data.minmax_x[0]-minmax_x_all[0])/(lenght_all_x)-0.5f,
                                        (Data.minmax_y[0]-minmax_y_all[0])/(lenght_all_y)-0.5f,
                                        0.0f});

            shapes.emplace_back(celist_,Data);
        }

    }

    void placement_of_elements(){}

    virtual void update_position(const Position *view){
        for(auto &TT:shapes){
            TT.template change <Position>().get_data()->set_view(view);
        }
    }

};
}// namespace CeEngine
#endif // CEOBJECTSVG_H
