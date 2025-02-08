#ifndef NODE_H
#define NODE_H

#include <mesh.h>
#include <texture.h>

#include <array>
#include <glm/vec3.hpp> // glm::vec3
#include <optional>

#include <map>
#include <vector>
#include <tuple>
constexpr inline int cout_type_image=4;

struct Material_3d_M{
  int map_difusion=-1;
  int map_normals=-1;
  int map_reflection=-1;
  int map_height=-1;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess=-1;
};



class Node{
public:
    Node(){};
    std::array<std::array<std::tuple<std::filesystem::path,TYPE_TEXTURE>,COUT_TYPE_TEX_COORDS>,cout_type_image> image_info;
    std::optional<Mesh> mesh;
    Material_3d_M material;
};

template<typename T__,typename N,typename Key,typename F>
auto foreach_graf(std::map<Key,std::pair<N,std::vector<Key>>> graf,F&& fun){
    std::map<Key,std::pair<T__,std::vector<Key>>> graf_new;
    for(auto &[K,T]:graf){
        graf_new[K]={fun(T.first),T.second};
    }
    return graf_new;
};

#endif // NODE_H
