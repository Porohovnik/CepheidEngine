#version 430 core
#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_shader_draw_parameters : enable

layout (location = 0) in vec3 position;
layout (location = 4) in vec2 texKord;

out vec2   tex;
out vec4 mat_color;
out flat int t;


struct Pos{
    mat4 pos;
};

layout(std430, binding = 1) buffer dataPos{
   Pos position_[];
};

//uniform int i;
layout(std430, binding = 0) buffer dataid{
   int id[];
};


void main(){
    int j=id[gl_DrawIDARB];
    t=j;
    //когда переёдт на gl_DrawID вернуть
    //int j=i;

    tex=texKord;
    gl_Position =position_[j].pos*vec4(position,1.0f);
}
