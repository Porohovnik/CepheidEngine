#version 430 core
#extension GL_ARB_bindless_texture : enable

in      vec2 tex;
in      vec4 color_object;
in flat int  id_;

out vec4 color;

struct Mat{
    int id;
    int map_difusion;
    float r,g,b,a;
};

struct Win_data{
    int widht;
};

layout(std430, binding = 0) buffer dataid{
   int id[];
};

layout(std430, binding = 2) buffer dataMaterial{
   Mat material[];
};

layout(std430, binding = 4) buffer id_smpl{
   uvec2 sample_id[];
};

layout(std430, binding = 10) buffer id_mat{
   uint hh[];
};

layout(std430, binding = 11) buffer dataWin{
   Win_data win_data[];
};

layout(early_fragment_tests) in;

void main(){
    int k=int((gl_FragCoord.y-0.5)*win_data[0].widht+gl_FragCoord.x-0.5);
    hh[k]=uint(material[id_].id);

    if(material[id_].map_difusion!=-1){
        color=texture2D(sampler2D(sample_id[material[id_].map_difusion]),tex);
    }else{
        color=color_object;
    }
};
