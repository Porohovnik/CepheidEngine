#ifndef CEOBJECT_H
#define CEOBJECT_H

#include <memory>

#include "position.h"
#include "them_center.h"

#include "incoming_signals_from_user.h"
#include "signal_engine.h"
namespace CeEngine {
class CeObject{
protected:
    Position position_object;

    CeObject(){}
public:
    class Change_Position{
        CeObject * controller;
    public:
        Position * position;
        Change_Position(CeObject * t,Position * position_):controller(t),position(position_){}

        ~Change_Position(){
            controller->update_position(position->get_position());
        }
    };

    inline static std::wstring current_path___=std::filesystem::current_path().wstring();

    void connect([[maybe_unused]] Signal_engine<CeObject> * signal){}
    void placement_of_elements(){}
    void installing_the_theme(){}

    class Interaction_One_input_area{};
    class Interaction_Repetition_area{};
    class Exit_area{};

    class Infiniti{};
    class Key_signal{};
    class Input_signal{};

    virtual void operator()([[maybe_unused]] Win_layer::Incoming_signals_from_user * ISFU,  Interaction_One_input_area){}
    virtual void operator()([[maybe_unused]] Win_layer::Incoming_signals_from_user * ISFU,  Interaction_Repetition_area){}
    virtual void operator()([[maybe_unused]] Win_layer::Incoming_signals_from_user * ISFU,  Exit_area){}
    virtual void operator()([[maybe_unused]] Win_layer::Incoming_signals_from_user * ISFU,  Key_signal){}
    virtual void operator()([[maybe_unused]] Win_layer::Incoming_signals_from_user * ISFU,  Input_signal){}
    virtual void operator()([[maybe_unused]] Win_layer::Incoming_signals_from_user * ISFU,  Infiniti){}

    virtual void update_position([[maybe_unused]] const Position *view){}
    virtual void update_theme([[maybe_unused]] Them_centre_::Them_object_base_base * theme){}

    ///the original position every template from: 0;0 - 1;1;
    virtual void add_element([[maybe_unused]] std::shared_ptr<CeObject> element){}

    inline auto change_position(){
        return Change_Position(this,&position_object);
    }

    inline const Position * get_position() const{
        return &position_object;
    }


    template<class J,typename D,typename Engine>
    inline static std::shared_ptr<J> create_object_no_base(Engine * win_,typename Engine::List * list,D data){
        std::shared_ptr<J> t=std::make_shared<J>(win_,list,data);
        processing_coll(win_,*t);
        return t;
    }

    template<typename T,typename Engine>
    inline static void processing_coll(Engine * win_,T & coll){
        coll.T::connect(&static_cast<Engine * >(win_)->signal);//раз
        coll.T::placement_of_elements();//двас
        coll.T::installing_the_theme();//Трис
    }
};
}// namespace CeEngine


#endif // CEOBJECT_H
