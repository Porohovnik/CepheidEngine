#ifndef SIGNAL_ENGINE_H
#define SIGNAL_ENGINE_H
#include <map>
#include <set>
namespace CeEngine {
template<typename Object>
class Signal_engine{
    std::set<Object *> infiniti_fun;
    std::set<Object *> input_code_fun;

    std::map<int,Object *> key_fun;

    std::multimap<int, Object *> id_fun;
    std::size_t  id_detector_before=0;
public:
    Signal_engine(){}

    int connect(Object * object,int min,int max=-1){
        if(max==-1){
            max=min;
        }

        for(int id=min;id<=max;id++){
           id_fun.emplace(id,object);
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

    template< typename Info_environment>
    void new_frame(Info_environment * ISFU){
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
            auto range_bef = id_fun.equal_range(id_detector_before);
            for (auto T = range_bef.first; T != range_bef.second; ++T){
                if(T->second!=nullptr){
                    int t=false;
                    auto range = id_fun.equal_range(id_detector);
                    for (auto J = range.first; J != range.second; ++J){
                        t+=(J->second==T->second);
                    }
                    if(t){
                        continue;
                    }
                    (*(T->second))(ISFU,type);
                }
            }
        };


        auto range = id_fun.equal_range(ISFU->id_detector);
        for (auto T = range.first; T != range.second; ++T){
            if(T->second!=nullptr){
                (*(T->second))(ISFU,typename Object::Interaction_Repetition_area());
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
}// namespace CeEngine

#endif // SIGNAL_ENGINE_H
