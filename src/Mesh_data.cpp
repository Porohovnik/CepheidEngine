#include "Mesh_data.h"
#include <algorithm>
#include "CDT.h"

#include <Interator_different_steps.h>

#include <iostream>

namespace CeEngine {
std::vector<glm::vec2> Mesh_data::normalaize(unsigned int begin_offset,unsigned int end_offset){
    std::vector<glm::vec2> parametr_cord_min_max(end_offset-begin_offset);

    //    double diagonal_qrt=0;
    for (std::size_t i=begin_offset;i<end_offset;i++) {
        auto [min, max]=std::minmax_element(Interator_different_steps(vertex.begin()+i,  info.get_offset()),
                                            Interator_different_steps(vertex.end()+i,    info.get_offset()));

        parametr_cord_min_max[i][0]=*min;
        parametr_cord_min_max[i][1]=*max;

        if(std::abs(*max-*min)>0.00001f){
            std::for_each(Interator_different_steps(vertex.begin()+i,  info.get_offset()),
                          Interator_different_steps(vertex.end()+i,    info.get_offset()),
                          [min_=*min,max_=*max](auto &t){
                          t=(t-min_)/std::abs(max_-min_);//приведение к еденичному размеру и центровка
            });
        }
    }

    return parametr_cord_min_max;
};


Mesh_data::Mesh_data(std::list<glm::vec3> &point_data, std::vector<std::pair<uint,uint>> edges, std::vector<float> data_outher, int data_step){
    std::vector<CDT::Edge> edgs_;

    for(auto [begin,end]:edges){
        for(std::size_t i=begin;i<end;i++){
            edgs_.emplace_back(i,i+1);
        }
        edgs_.emplace_back(end,begin);
        std::cout<<begin<<":"<<end<<"|"<<end<<":"<<begin<<std::endl;
    }



    CDT::Triangulation<float> cdt;
    cdt.insertVertices(point_data.begin(),
                       point_data.end(),
                       [](auto &t){return  t.x;},
                       [](auto &t){return  t.y;}
                       );

    std::cout<<edgs_.size()<<" edgs_ "<<point_data.size()<<std::endl;


    cdt.insertEdges(edgs_);
    cdt.eraseOuterTrianglesAndHoles();

    std::cout<<point_data.size()<<">>>>>>>>>>>"<<cdt.triangles.size()<<std::endl;

    std::size_t n_=0;
    for(auto &L:point_data){
        vertex.emplace_back(L.x);
        vertex.emplace_back(L.y);
        vertex.emplace_back(0.0f);

        for (std::size_t j=0;j<(data_step-3);j++,n_++) {
            vertex.emplace_back(data_outher[n_]);
        }
    }

    for(auto &T:cdt.triangles){
        //std::cout<<T.vertices[0] <<"|"<<T.vertices[1]<<"|"<<T.vertices[2] <<std::endl;
        indexes.emplace_back(T.vertices[0]);
        indexes.emplace_back(T.vertices[1]);
        indexes.emplace_back(T.vertices[2]);

    }
}
}// namespace CeEngine
