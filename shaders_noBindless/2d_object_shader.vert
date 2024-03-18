#version 430 core



layout (location = 0) in vec3 position;
layout (location = 4) in vec2 texKord;

out      vec2 tex;
out      vec4 color_object;
out flat int  id_;

struct Pos{
    mat4 pos;
};

struct Mat{
    int id;
    int map_difusion;
    float r,g,b,a;
};

layout(std430, binding = 0) buffer dataid{
   int id[];
};

layout(std430, binding = 1) buffer dataPos{
   Pos position_[];
};

layout(std430, binding = 2) buffer dataMaterial{
   Mat material[];
};

uniform int i;
void main(){
    id_=i;

    tex=texKord;
    color_object=vec4(material[id_].r,material[id_].g,material[id_].b,material[id_].a);
    gl_Position =position_[id_].pos*vec4(position,1.0f);
}
