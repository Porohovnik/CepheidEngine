#version 450 core

out vec4 color;
in vec2    tex;
struct Win_data{
    int widht;
};

layout(std430, binding = 10) buffer id_mat{
   uint hh[];
};
layout(std430, binding = 11) buffer dataWin{
   Win_data win_data[];
};


void main(){

    int k=int((gl_FragCoord.y-0.5)*win_data[0].widht+gl_FragCoord.x-0.5);
    float t=1.0f/(hh[46077]);
    color=vec4(1.0f,1.0f,t,1.0f);
};
