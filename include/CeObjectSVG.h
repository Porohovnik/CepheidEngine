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
    inline static std::size_t id_global_mesh=0;
public:
    struct Data_initialization{
        std::filesystem::path path;
        Color color;
        Position pos{1.0f};
        Data_initialization(std::filesystem::path path_,Color color_,Position pos_):path(path_),color(color_),pos(pos_){}
        Data_initialization(std::filesystem::path path_):path(path_){}
    };


    template<typename Engine>
    CeObjectSVG(Engine *win_,typename Engine::List * celist_,Data_initialization data){
        SVG svg{data.path};

        for (auto &T:svg.objects) {
            std::cout<<"******************************************"<<std::endl;
            std::list<glm::vec3> data_point;
            std::vector<std::pair<uint,uint>> contour;

            glm::vec3 begin;
            bool t=true;
            uint begin_=0;
            for (auto &J:T.bezie_curves_data) {
                cubicBez(J.p0,J.p1,J.p2,J.p3,0.1f,data_point);
                std::cout<<J.p0[0]<<":"<<J.p0[1]<<"|"<<J.p1[0]<<":"<<J.p1[1]<<"|"<<J.p2[0]<<":"<<J.p2[1]<<"|"<<J.p3[0]<<":"<<J.p3[1]<<std::endl;
                if(t){
                    begin=J.p0;
                    t=false;
                }
                if(( std::abs(begin.x-J.p3.x )+std::abs(begin.y-J.p3.y ))<=0.001f){//исправить равно
                    t=true;
                    contour.emplace_back(begin_,data_point.size()-1);
                    begin_=data_point.size();
                    std::cout<<" =================================="<<std::endl;
                }
            }


            for(auto &T:data_point){
                std::cout<<T.x<<"|"<<T.y<<std::endl;
            }


            std::vector<float> data_tex(data_point.size()*2,0.0f);

            auto data_mesh=std::make_shared<Mesh_data>(data_point,contour,data_tex,5);
            data_mesh->set_marking_vertices<GL_layer::TYPE_data::position>(3);            
            data_mesh->set_marking_texture <0>(2);

            //std::cout<<" =================================="<<std::endl;
            for(std::size_t i=0;i<data_mesh->indexes.size();i+=3){
                //std::cout<<data_mesh->indexes[i]<<", "<<data_mesh->indexes[i+1]<<", "<<data_mesh->indexes[i+2]<<", ";
            }
           // std::cout<<std::endl;
            auto minmax_array=data_mesh->normalaize<GL_layer::TYPE_data::position>();

           // std::cout<<" =================================="<<std::endl;
            for(std::size_t i=0;i<data_mesh->vertex.size();i+=5){
                data_mesh->vertex[i]=-data_mesh->vertex[i];
                //std::cout<<data_mesh->vertex[i]<<", "<<data_mesh->vertex[i+1]<<", "<<data_mesh->vertex[i+2]<<", "<<data_mesh->vertex[i+3]<<", "<<data_mesh->vertex[i+4]<<", "<<std::endl;
            }
            shapes.emplace_back(celist_,CePlate::Data{data_mesh,std::to_string(id_global_mesh++)+"_SVG",Color{0.0f, 0.0f, 0.0f, 1.0f},Position{1.0f}});
            //std::to_string(id_global_mesh++)+"_SVG"
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
