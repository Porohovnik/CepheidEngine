#ifndef MESH_H
#define MESH_H
#include <array>
#include <vector>
#include <memory>


typedef unsigned int uint;

enum class TYPE_data:uint{position=0,normal=1,tangent=2,bitangent=3,tex_coords=4,color=12};
const uint COUT_TYPE=4;
const uint COUT_TYPE_TEX_COORDS=8;
struct Mesh_info{
    std::array<uint,COUT_TYPE>            elements_cout{0};
    std::array<uint,COUT_TYPE_TEX_COORDS> tex_cord_cout{0};
    int sum=0;
    bool operator==(Mesh_info & info){
        for(uint i=0;i<COUT_TYPE;i++){
           sum+= this->elements_cout[i]==info.elements_cout[i];
        }
        for(uint i=0;i<COUT_TYPE_TEX_COORDS;i++){
            sum+= this->tex_cord_cout[i]==info.tex_cord_cout[i];
        }
        if(sum!=(COUT_TYPE+COUT_TYPE_TEX_COORDS)){
            return false;
        }
        return true;
    }
};
#include <iostream>
struct Mesh_data{
    std::vector<float>  vertex;
    std::vector<uint>   indices;
    Mesh_data(std::vector<float> &&vertex_,std::vector<uint> &&indices_):vertex(std::move(vertex_)),indices(std::move(indices_)){}
    Mesh_data(std::vector<float> &&vertex_):vertex(std::move(vertex_)){}
};
class Mesh{
    Mesh_info info;
    std::shared_ptr<Mesh_data> data=nullptr;
public:
    Mesh(Mesh_info info_, std::shared_ptr<Mesh_data> data_):info(info_),data(data_){}

    const Mesh_info & get_info() const{
        return info;
    }

    auto get_data(){
        return  data;
    }

    //vertex
    float * data_vertex(){
        return data->vertex.data();
    }
    std::size_t size_vertex(){
        return data->vertex.size();
    }

    //indeces
    uint * data_indeces(){
        return data->indices.data();
    }

    std::size_t size_indeces(){
        return data->indices.size();
    }


    const std::shared_ptr<Mesh_data>& get_data () const{
        return data;
    }

    std::size_t size_bate_indeces(){
        return data->indices.size()*sizeof(uint);
    }

};

Mesh   line_mesh();
Mesh  plate_mesh();
Mesh skybox_mesh();
Mesh    box_mesh();
#endif // MESH_H
