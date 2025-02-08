#ifndef SIGNAL_ENGINE_H
#define SIGNAL_ENGINE_H

#include <map>
#include <set>
#include <vector>
#include <memory>
#include "incoming_signals_from_user.h" //сделать шаблонным)

class Setting_key;

template<typename Object>
class Signal_engine{
    std::set<Object *> infiniti_fun;
    std::set<Object *> input_code_fun;

    std::map<int,Object *> key_fun;

    std::map<int,std::shared_ptr<std::vector<Object *>>> id_fun;//расширить

    std::size_t  id_detector_before=0;
public:
    Signal_engine(){}

    int connect(Object * object,int min,int max=-1){
        if(max==-1){
            max=min;
        }

        for(int id=min;id<=max;id++){
            if(id_fun[id]==nullptr){
                id_fun[id]=std::make_shared<std::vector<Object *>>(0);
               // std::cout<<min<<":______:"<<max<<std::endl;
            }
            id_fun[id]->push_back(object);
        }
        return min;
    }


    void connect(Object * object,typename Object::Infiniti){
        infiniti_fun.insert(object);
    }
    void connect(Object * object,typename Object::Input_signal){
        input_code_fun.insert(object);
    }

    void connect(Object * object,int key,typename Object::Key_signal){
        key_fun[key]=object;
    }


    void dis_connect(Object * object,typename Object::Infiniti){
        auto t=infiniti_fun.extract(object);
    }
    void dis_connect(Object * object,typename Object::Input_signal){
        auto t=input_code_fun.extract(object);
    }

    void dis_connect(Object * object,int key,typename Object::Key_signal){
        key_fun[key]=nullptr;
    }
    //соединение с объектом на экране

    void new_frame(Incoming_signals_from_user * ISFU){
        for(auto &F:infiniti_fun){
            if(F!=nullptr){
                (*F)(ISFU,typename Object::Infiniti());
            }
        }


        for(auto &F:input_code_fun){
            if(F!=nullptr && ISFU->input_code_text!=-1){
                (*F)(ISFU,typename Object::Input_signal());
            }
        }


        auto t=[&](int id_detector,int id_detector_before,auto type){
            if(id_fun[ISFU,id_detector_before]!=nullptr){
                for(auto &T:*id_fun[id_detector_before]){
                    if(T!=nullptr){
                        int t=false;
                        if(id_fun[id_detector]!=nullptr){
                            for(auto &J:*id_fun[id_detector]){
                                t+=(J==T);
                            }
                        }
                        if(t){
                            continue;
                        }
                        (*T)(ISFU,type);
                    }
                }
            }
        };


        if(id_fun[ISFU->id_detector]!=nullptr){
            for(auto &T:*id_fun[ISFU->id_detector]){
                if(T!=nullptr){
                    (*T)(ISFU,typename Object::Interaction_Repetition_area());
                }
            }
        }

        if(ISFU->id_detector!=id_detector_before){
            t(ISFU->id_detector,id_detector_before,typename Object::Exit_area());
            t(id_detector_before,ISFU->id_detector,typename Object::Interaction_One_input_area());
        }
        id_detector_before=ISFU->id_detector;

        std::size_t i=0;
        for(auto &[K,T]:(ISFU->keys)){
            if(key_fun[K] !=nullptr){
                (*key_fun[K])(ISFU,typename Object::Key_signal());
                i+=1;
            }
        }
        if(i){
            ISFU->keys.clear();//в общем, проблема возникает из-за того что, кнопки сами по-себе могут нажиматься в функциях во время проверки...
        }
    }
};


#endif // SIGNAL_ENGINE_H
