#version 430 core
#extension GL_ARB_shader_draw_parameters : enable

layout (location = 0) in float position_y;
layout (location = 1) in float position_x;

out vec4 mat_color;

struct Pos{
    mat4 pos;
};

struct Mat{
    int id;
    int map_mesh;
    float r,g,b,a;
};

struct Command_draw_info{
    int  first;
    int  count;
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

layout(std430, binding = 3) buffer dataCmd{
   Command_draw_info cmd[];
};

void main(){
    int id_=id[gl_DrawIDARB];

    mat_color=vec4(material[id_].r,material[id_].g,material[id_].b,material[id_].a);

    float x=position_x+gl_VertexID-cmd[material[id_].map_mesh].first;
    gl_Position =position_[id_].pos*vec4(x,position_y,0.0f,1.0f);
}
