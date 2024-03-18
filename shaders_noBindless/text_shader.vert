#version 430 core



layout (location = 0) in vec3 position;
layout (location = 4) in vec2 texCord;

out vec2 tex;
out      vec4 color_object;
out flat int id_;

struct Pos{
    mat4 pos;
};

struct Material{
    int id;
    int map_font;
    float r,g,b,a;
};

struct ID{
    int begin;
    int size_max;
};

struct Buffer_text{
    int symbol;
};

struct Glif_info{
    float width,height;
    float bearing_left,bearing_top;

    float coordinate_transformation_x,coordinate_transformation_y;
    float x,y;

    float advance;
    float max_size;
};

layout(std430, binding = 0) buffer dataid{
   int id[];
};

layout(std430, binding = 1) buffer dataPos{
   Pos pos_symbol[];
};

layout(std430, binding = 2) buffer dataMaterial{
   Material color_symbol[];
};

layout(std430, binding = 3) buffer dataText{
   ID text[];
};

layout(std430, binding = 4) buffer dataBuffer_text{
   Buffer_text buffer_text[];
};

layout(std430, binding = 6) buffer dataGLif_id{
   ID glif_id[];
};

layout(std430,binding = 7) buffer dataGlif_info{
   Glif_info glif_info[];
};

uniform int i;
void main(){
    id_=i;

    float size=0;
    for(int z = 0; z < gl_InstanceID; z++){
        size+=glif_info[glif_id[color_symbol[id_].map_font].begin+buffer_text[text[id_].begin+z].symbol].advance;
    }

    int k=glif_id[color_symbol[id_].map_font].begin+buffer_text[text[id_].begin+gl_InstanceID].symbol;

    mat4 line_str_placement=mat4(1.0f);

    line_str_placement[0][0]=glif_info[k].width;
    line_str_placement[1][1]=glif_info[k].height*2.0f;

    line_str_placement[3][0]=(size+glif_info[k].bearing_left+glif_info[k].width*0.5f);
    line_str_placement[3][1]=-(glif_info[k].max_size-glif_info[k].bearing_top*2.0f+glif_info[k].height);


    //пока так, легаси(чтобы везде текст не поплыл к чёрту)
    mat4 line_str_scale=mat4(1.0f);
    line_str_scale[0][0]=1/glif_info[k].max_size*0.5f;
    line_str_scale[1][1]=1/glif_info[k].max_size*0.5f;

    mat4 line_str_model=pos_symbol[id_].pos;
    line_str_model[0][0]=line_str_model[1][1];

    float iks=texCord.x*glif_info[k].coordinate_transformation_x+glif_info[k].x;
    float yik=texCord.y*glif_info[k].coordinate_transformation_y+glif_info[k].y;//yik=1.0f-yik;

    tex=vec2(iks,yik);

    color_object=vec4(color_symbol[id_].r,color_symbol[id_].g,color_symbol[id_].b,color_symbol[id_].a);
    gl_Position =line_str_model*line_str_scale*line_str_placement*vec4(position, 1.0f);
}
