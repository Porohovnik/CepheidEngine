#version 450 core
layout (location = 0) in vec3 position;
layout (location = 4) in vec2 texKord;

out vec2   tex;

void main(){
    tex=texKord;
    gl_Position =vec4(position,1.0f);
}
