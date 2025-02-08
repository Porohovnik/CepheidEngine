#include "node.h"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

using Graf_node= std::map<std::string,std::pair<std::pair<aiMesh*,aiMaterial*>,std::vector<std::string>>>;
Node node_load(aiMesh* mesh,aiMaterial* material);


auto load_3dm(std::filesystem::path patch){
    Assimp::Importer import;
    const aiScene * scene =import.ReadFile(patch, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    Graf_node graf_node;
    //lj,fdkztv

    auto & node=scene->mRootNode;
//    for(int i=0;i<node->;i++){

//    }


   // node_load(graf_node,,scene);
    return foreach_graf<Node>(graf_node,[](auto p){return node_load(p.first,p.second);});
};

//scene->mMeshes[node->mMeshes[i]];
//T[mesh->mName.C_Str()]=std::pair<Node,std::vector<std::string>>{{},{}};
//scene->mMaterials[mesh->mMaterialIndex];


Node node_load(aiMesh* mesh,aiMaterial* material){
    Node node;

    std::vector<float> vert;
    Mesh_info info;

    std::array<aiVector3t<float> *,COUT_TYPE> vertex_storage={
       mesh->mVertices,
       mesh->mNormals,
       mesh->mTangents,
       mesh->mBitangents
    };

    std::array<int,COUT_TYPE> type_vertex_storage={
       static_cast<int>(TYPE_data::position),
       static_cast<int>(TYPE_data::normal),
       static_cast<int>(TYPE_data::tangent),
       static_cast<int>(TYPE_data::bitangent)
    };

    for(int i=0;i<COUT_TYPE;i++){
        if(vertex_storage[i]!=nullptr){
            info.elements_cout[type_vertex_storage[i]]=3;
        }
    }
    for(int i=0;i<COUT_TYPE_TEX_COORDS;i++){
        if(mesh->mTextureCoords[i]!=nullptr){
            info.tex_cord_cout[i]=2;
        }
    }


    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
        for(int j=0;j<COUT_TYPE;j++){
            if(vertex_storage[j]!=nullptr){
                vert.push_back(std::move(vertex_storage[j][i].x));
                vert.push_back(std::move(vertex_storage[j][i].y));
                vert.push_back(std::move(vertex_storage[j][i].z));
            }
        }
        for(int j=0;j<COUT_TYPE_TEX_COORDS;j++){
            if(mesh->mTextureCoords[j]!=nullptr){
                vert.push_back(std::move(mesh->mTextureCoords[j][i].x));
                vert.push_back(std::move(mesh->mTextureCoords[j][i].y));
            }
        }
    };

    std::vector<unsigned int> indices;
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    auto data_mesh=std::make_shared<Mesh_data>(std::move(vert),std::move(indices));
    node.mesh.emplace(info,data_mesh);

    aiColor3D color;
    material->Get(AI_MATKEY_COLOR_DIFFUSE,color);
    node.material.diffuse.r=color.r;
    node.material.diffuse.g=color.g;
    node.material.diffuse.b=color.b;
    material->Get(AI_MATKEY_COLOR_AMBIENT,color);
    node.material.ambient.r=color.r;
    node.material.ambient.g=color.g;
    node.material.ambient.b=color.b;
    material->Get(AI_MATKEY_COLOR_SPECULAR,color);
    node.material.specular.r=color.r;
    node.material.specular.g=color.g;
    node.material.specular.b=color.b;
    float spec;
    material->Get(AI_MATKEY_SHININESS,spec);
    node.material.shininess=spec;


    aiTextureType type_ai[4]={
        aiTextureType_DIFFUSE,
        aiTextureType_NORMALS,
        aiTextureType_SPECULAR,
        aiTextureType_HEIGHT
    };

    TYPE_TEXTURE type_tx[4]={
            Map_diffusion,
            Map_normals,
            Map_reflection,
            Map_height
    };

    for (int j=0;j<4;j++) {
        for(uint k=0;k<material->GetTextureCount(type_ai[j]);k++){
            aiString str;
            material->GetTexture(type_ai[j], k, &str);
            int type=type_tx[j];
            node.image_info[type][k]={str.C_Str(),static_cast<TYPE_TEXTURE>(type)};
        }
    }

    return node;
};
