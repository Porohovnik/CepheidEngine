#version 430 core
#extension GL_ARB_bindless_texture : enable

in vec2 tex;
in flat int  id_;
//uniform sampler2D ourTexture;

out vec4 color_;

struct Win_data{
    int widht;
};

struct Material{
    int id;
    int map_font;
    float r,g,b,a;
};

layout(std430, binding = 10) buffer id_mat{
   uint hh[];
};
layout(std430, binding = 11) buffer dataWin{
   Win_data win_data[];
};

layout(std430, binding = 5) buffer id_smpl{
   uvec2 sample_id[];
};

layout(std430, binding = 2) buffer dataMaterial{
   Material color_symbol[];
};

//uniform int i;
layout(std430, binding = 0) buffer dataid{
   int id[];
};

layout(early_fragment_tests) in;
void main(){

    int k=int((gl_FragCoord.y-0.5)*win_data[0].widht+gl_FragCoord.x-0.5);
    hh[k]=uint(color_symbol[id_].id);

    vec4 sampled = vec4(1.0, 1.0, 1.0,texture(sampler2D(sample_id[color_symbol[id_].map_font]),tex).r);
    color_ = sampled *vec4(color_symbol[id_].r,color_symbol[id_].g,color_symbol[id_].b,color_symbol[id_].a) ;
//    if(color_.a<0.01f)
//       discard;
//  color_= vec4(color.xyz, 1.0);

};
