#ifndef CEFPS_H
#define CEFPS_H
#include "ceobject.h"
#include "CeText.h"

namespace CeEngine {
class CeFPS: public CeObject{
    int FPS=0;
    CeText text;
    char fps[9]={'F','P','S',':',' ','0','0','0'};
public:
    using Data_initialization = CeText::Data;

    template<typename Engine>
    CeFPS(Engine *win_,typename Engine::List * celist_,CeText::Data data):
        text(celist_,data){
        std::cout<<"CeFPS"<<std::endl;
    }

    void connect(Signal_engine<CeObject> * signal){
        signal->connect(this,typename CeObject::Infiniti{});
    }


    virtual void operator()(Win_layer::Incoming_signals_from_user * ISFU,typename CeObject::Infiniti){
        FPS=static_cast<int>(1/(ISFU->deltaTime));
        std::to_chars(&fps[5],&fps[7],FPS);

        //std::string s=std::string{"F"};
        text.change_text(fps);
        //std::cout<<FPS<<std::endl;
    }
};
}// namespace CeEngine
#endif // CEFPS_H
