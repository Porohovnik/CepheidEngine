#ifndef CESCROLLBAR_H
#define CESCROLLBAR_H
#include "CeButtom.h"
namespace CeEngine {
class CeScrollbar: public CeObject{
    Position pos_elements=glm::vec3{0.0f,0.0f,0.0f};

    CePlate Scrollbar;
    CeButtom Scrollbar_joustick;
    CeButtom Scrollbar_plus;
    CeButtom Scrollbar_minus;
    float pos=0.0f;//relevation size page,max=1

public:
    template<typename Engine>
    CeScrollbar(Engine *win_,typename Engine::List * celist_,typename Engine::Data data){
        std::cout<<"CeScrollbar"<<std::endl;

    }
    //

    void connect(Signal_engine<CeObject> * signal){
        signal->connect(this,Scrollbar.id_global,-1);
    }

    virtual void operator()(int id,typename CeObject::Interaction_Repetition_area){

    }
};
}// namespace CeEngine
#endif // CESCROLLBAR_H
