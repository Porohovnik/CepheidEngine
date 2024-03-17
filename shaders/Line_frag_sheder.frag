#version 430 core
#extension GL_ARB_bindless_texture : enable

out vec4 color;
in  vec4 mat_color;

void main(){
    color=mat_color;
};

