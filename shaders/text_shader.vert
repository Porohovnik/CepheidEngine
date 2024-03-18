#version 430 core
#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_shader_draw_parameters : enable

layout (location = 0) in vec3 position;
layout (location = 4) in vec2 texCord;

struct Pos{
    mat4 pos;
};

struct Glif_info{
    float width,height;
    float bearing_left,bearing_top;

    float coordinate_transformation_x,coordinate_transformation_y;
    float x,y;

    float advance;
    float max_size;
};


struct ID{
    int begin;
    int size_max;
};

struct Buffer_text{
    int symbol;
};


layout(std430, binding = 1) buffer dataPos{
   Pos pos_symbol[];
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

struct Material{
    int id;
    int map_font;
    float r,g,b,a;
};


layout(std430, binding = 2) buffer dataMaterial{
   Material color_symbol[];
};


out flat int id_;

uniform int i;
layout(std430, binding = 0) buffer dataid{
   int id[];
};


out vec2 tex;

void main(){
        int j=id[gl_DrawIDARB];
        //когда перейдёт на gl_DrawID вернуть
        //int j=i;
        id_=j;

        mat4 model_=pos_symbol[j].pos;
        float size=0;
        for(int z = 0; z < gl_InstanceID; z++){
            int k=glif_id[color_symbol[j].map_font].begin+buffer_text[text[j].begin+z].symbol;
              size=size+(glif_info[k].width+glif_info[k].bearing_left)/glif_info[k].max_size*0.5f+0.035f;
            //size=size+glif_info[k].advance/glif_info[k].max_size*0.5f;
        }
        size*=pos_symbol[j].pos[1][1];

        int k=0;
        k=glif_id[color_symbol[j].map_font].begin+buffer_text[text[j].begin+gl_InstanceID].symbol;

        model_[1][1]=model_[1][1]*glif_info[k].height/glif_info[k].max_size;

        //float klk=pos_symbol[j].pos[1][1]*(1.0f-glif_info[k].height/glif_info[k].max_size)*0.5f;
        //float b=model_[1][1]*(glif_info[k].bearing_top-glif_info[k].height)/glif_info[k].height;

        float c=model_[1][1]*((0.5f*glif_info[k].max_size-glif_info[k].bearing_top)/glif_info[k].height+0.5f);
        //c=klk-b;

        model_[0][0]=glif_info[k].width/glif_info[k].height*model_[1][1]*0.5f;
        model_[3]=vec4(size*1.0f+pos_symbol[j].pos[3][0]+model_[0][0]*0.5f,pos_symbol[j].pos[3][1]-c,pos_symbol[j].pos[3][2],pos_symbol[j].pos[3][3]);



        gl_Position =model_*vec4(position, 1.0f);


        //float print=glif_info[k].widht/100.0f;


        float iks=texCord.x*glif_info[k].coordinate_transformation_x+glif_info[k].x;
        float yik=texCord.y*glif_info[k].coordinate_transformation_y+glif_info[k].y;
        //yik=1.0f-yik;


        tex=vec2(iks,yik);
}
