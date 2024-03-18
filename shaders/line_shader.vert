#version 430 core
#extension GL_ARB_bindless_texture : enable
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


layout(std430, binding = 1) buffer dataPos{
   Pos position_[];
};

layout(std430, binding = 2) buffer dataMaterial{
   Mat material[];
};

layout(std430, binding = 3) buffer dataCmd{
   Command_draw_info cmd[];
};

layout(std430, binding = 0) buffer dataid{
   int id[];
};



void main(){
    //int j=id[i];
    //когда переёдт на gl_DrawID вернуть
    //int j=i;
    int j=id[gl_DrawIDARB];

    float x=position_x+gl_VertexID-cmd[material[j].map_mesh].first;
    gl_Position =position_[j].pos*vec4(x,position_y,0.0f,1.0f);
    mat_color=vec4(material[j].r,material[j].g,material[j].b,material[j].a);
}
