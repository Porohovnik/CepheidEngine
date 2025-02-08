#ifndef DRAW_PROGRAM_H
#define DRAW_PROGRAM_H
#include<GL/glew.h>

inline constexpr int TYPE_RENDERING_ARRAY=0X1;
inline constexpr int TYPE_RENDERING_ELEMENT=0X4;
inline constexpr int TYPE_RENDERING_MYLTI=0X8;
inline constexpr int TYPE_RENDERING_INDERECT=0X10;
inline constexpr int TYPE_RENDERING_INSTANSE=0X20;
inline constexpr int TYPE_RENDERING_BASE_INSTANSE=0X40;
inline constexpr int TYPE_RENDERING_BASE_VERTEX=0X80;
inline constexpr int TYPE_RENDERING_BIND_BUFFER=0X100;

//GL_POINTS — каждая вершина задает точку
//GL_LINES — каждая отдельная пара вершин задает линию
//GL_LINE_STRIP — каждая пара вершин задает линию (т.е. конец предыдущей линии является началом следующей)
//GL_LINE_LOOP — аналогично предыдущему за исключением того, что последняя вершина соединяется с первой и получается замкнутая фигура
//GL_TRIANGLES — каждая отдельная тройка вершин задает треугольник
//GL_TRIANGLE_STRIP — каждая следующая вершина задает треугольник вместе с двумя предыдущими (получается лента из треугольников)
//GL_TRIANGLE_FAN — каждый треугольник задается первой вершиной и последующими парами (т.е. треугольники строятся вокруг первой вершины, образуя нечто похожее на диафрагму)
//GL_QUADS — каждые четыре вершины образуют четырехугольник
//GL_QUAD_STRIP — каждая следующая пара вершин образует четырехугольник вместе с парой предыдущих
//GL_POLYGON — задает многоугольник с количеством углов равным количеству заданных вершин

inline constexpr int TYPE_OBJECT_POINTS=GL_POINTS;

inline constexpr int TYPE_OBJECT_LINES=GL_LINES;
inline constexpr int TYPE_OBJECT_LINE_STRIP=GL_LINE_STRIP;
inline constexpr int TYPE_OBJECT_LINE_LOOP=GL_LINE_LOOP;

inline constexpr int TYPE_OBJECT_TRIANGLES=GL_TRIANGLES;
inline constexpr int TYPE_OBJECT_TRIANGLE_STRIP=GL_TRIANGLE_STRIP;
inline constexpr int TYPE_OBJECT_TRIANGLE_FAN=GL_TRIANGLE_FAN;

inline constexpr int TYPE_OBJECT_QUADS=GL_QUADS;
inline constexpr int TYPE_OBJECT_QUAD_STRIP=GL_QUAD_STRIP;
inline constexpr int TYPE_OBJECT_POLYGON=GL_POLYGON;


struct DrawArraysIndirectCommand{
    GLuint  count=6;
    GLuint  primCount=1;
    GLuint  first=0;
    GLuint  baseInstance=0;
} ;

struct DrawElementsIndirectCommand{
    GLuint  count=6;
    GLuint  instanceCount=1;
    GLuint  firstIndex=0;
    GLuint  baseVertex=0;
    GLuint  baseInstance=0;
};



template<int type_object,int type_draw,int type_byte=GL_UNSIGNED_INT>
static inline void Draw(const DrawArraysIndirectCommand * command, int count=1) noexcept{
    if constexpr(type_draw & TYPE_RENDERING_ARRAY){        
        if constexpr(type_draw & TYPE_RENDERING_INSTANSE){
            if constexpr(type_draw & TYPE_RENDERING_BASE_INSTANSE){
                glDrawArraysInstancedBaseInstance(type_object,command[0].first,command[0].count,command[0].primCount,command[0].baseInstance);
            }else{
                glDrawArraysInstanced(type_object,command[0].first,command[0].count,command[0].primCount);
            }
        }
        if constexpr(type_draw & TYPE_RENDERING_INDERECT){
            if constexpr(type_draw & TYPE_RENDERING_MYLTI){
                glMultiDrawArraysIndirect(type_object,command,count,0);
            }else{
                glDrawArraysIndirect(type_object,command);
            }
        }
        if constexpr(type_draw & TYPE_RENDERING_MYLTI){
            if constexpr(type_draw & TYPE_RENDERING_BASE_VERTEX){
                //glMultiDrawArraysBaseVertex(type_object);
            }else{
               // glMultiDrawArrays(type_object);
            }
        }else{
            if constexpr(type_draw & TYPE_RENDERING_BASE_VERTEX){
                //glMultiDrawArrays(type_object,command.first,command.count,command.instanceCount);
            }else{
                glDrawArrays(type_object,command[0].first,command[0].count);
            }
        }
    }
}


#include <iostream>
template<int type_object,int type_draw,int type_byte=GL_UNSIGNED_INT>
static inline void Draw(const DrawElementsIndirectCommand * command, int count=1){

    if constexpr(type_draw & TYPE_RENDERING_ELEMENT){
        if constexpr(type_draw & TYPE_RENDERING_INSTANSE){
            if constexpr(type_draw & TYPE_RENDERING_BASE_INSTANSE){
                if constexpr(type_draw & TYPE_RENDERING_BASE_VERTEX){
                    glDrawElementsInstancedBaseVertexBaseInstance(type_object,command[0].count,type_byte,nullptr,command[0].instanceCount,command[0].baseVertex,command[0].baseInstance);
                }else{
                    glDrawElementsInstancedBaseInstance(type_object,command[0].count,type_byte,nullptr,command[0].instanceCount,command[0].baseInstance);
                }
            }else{
                if constexpr(type_draw & TYPE_RENDERING_BASE_VERTEX){
                    glDrawElementsInstancedBaseVertex(type_object,command[0].count,type_byte,nullptr,command[0].instanceCount,command[0].baseVertex);
                }else{
                    glDrawElementsInstanced(type_object,command[0].count,type_byte,nullptr,command[0].instanceCount);
                    //std::cout<<glGetError()<<"/-draw-/"<<"///::"<<command[0].instanceCount<<std::endl;
                }
            }
        }
        if constexpr(type_draw & TYPE_RENDERING_INDERECT){
            if constexpr(type_draw & TYPE_RENDERING_MYLTI){
                //std::cout<<glGetError()<<"|"<<type_byte<<"|"<<"glMultiDrawElementsIndirect"<<count<<std::endl;
                glMultiDrawElementsIndirect(type_object,type_byte,command,count,0);
                //std::cout<<"|"<<glGetError()<<std::endl;
            }else{
                glDrawElementsIndirect(type_object,type_byte,command);
            }
        }
        if constexpr(type_draw & TYPE_RENDERING_MYLTI){
            if constexpr(type_draw & TYPE_RENDERING_BASE_VERTEX){
                //glMultiDrawElements(type_object, ,type_byte,nullptr,count,command[0].baseVertex);
            }else{
                //glMultiDrawElements(type_object, ,type_byte,nullptr,count);
            }
        }else{
            if constexpr(type_draw & TYPE_RENDERING_BASE_VERTEX){
                glDrawElementsBaseVertex(type_object,command[0].count,type_byte,nullptr,command[0].baseVertex);
            }else{
                glDrawElements(type_object,command[0].count,type_byte,nullptr);
            }
        }
    }
};


#endif // DRAW_PROGRAM_H
