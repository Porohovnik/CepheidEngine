#ifndef STROKE_OF_FATE_H
#define STROKE_OF_FATE_H
#include "ceobject.h"
#include "CeText.h"
namespace CeEngine {
class Stroke_of_fate: public CeObject{
    CeText text_1;
    CeText text_2;
    bool m=false;
public:
    template <typename Engine>
    Stroke_of_fate(Engine *win_,typename Engine::List * celist_,typename Engine::Data data):
        text_1(win_,celist_,CeText::Data{Directory_fonts+"SourceCodePro-Regular.ttf","Создатель: "}),
        text_2(win_,celist_,CeText::Data{Directory_fonts+"SourceCodePro-Regular.ttf","Кузнецов Даниил Андреевич"}){
        text_1.template change <Material_T>().get_data()->color=glm::vec4{65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};
        text_2.template change <Material_T>().get_data()->color=glm::vec4{65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};


    }

    void connect(Signal_engine<CeObject> * signal){
        signal->connect(this,GLFW_KEY_L,typename CeObject::Key_signal{});
        signal->connect(this,GLFW_KEY_H,typename CeObject::Key_signal{});///
        signal->connect(this,typename CeObject::Infiniti{});
    }

    virtual void operator()(Win_layer::Incoming_signals_from_user * ISFU, typename CeObject::Key_signal){;
        if(ISFU->keys[GLFW_KEY_L].action && ISFU->keys[GLFW_KEY_H].action){
            if(m){
                text_1.template change <Position>().get_data()->new_move(glm::vec3{-1000.9f,0.22f,-0.9f});
                text_2.template change <Position>().get_data()->new_move(glm::vec3{-1000.9f,-0.25f,-0.9f});
                m=false;
            }else {
                text_1.template change <Position>().get_data()->new_move(glm::vec3{-0.9f,0.22f,-0.9f});
                text_2.template change <Position>().get_data()->new_move(glm::vec3{-0.9f,-0.25f,-0.9f});
                m=true;
            }
        }
    }

    void placement_of_elements(){
        text_1.template change <Position>().get_data()->scale(glm::vec3{0.22f,0.22f,1.0f});
        text_1.template change <Position>().get_data()->new_move(glm::vec3{-1000.9f,0.22f,-0.9f});
        text_2.template change <Position>().get_data()->scale(glm::vec3{0.22f,0.22f,1.0f});
        text_2.template change <Position>().get_data()->new_move(glm::vec3{-1000.9f,-0.25f,-0.9f});
    }

};
}// namespace CeEngine
#endif // STROKE_OF_FATE_H
