#ifndef CEWORLDMAP_H
#define CEWORLDMAP_H
#include "ceobject.h"

#include "CePlate.h"

#include "utiliti_json.h"
namespace CeEngine {
glm::vec2 Sphere_to_Mercator(glm::vec2 lon_lang){
    const float a=6370997;
    const float b=6356752.314;
    float f=(a-b)/b;
    float e1=std::sqrt(2*f-f*f);
    float e2=std::sqrt(1-b/a*b/a);




    auto lon_r_lang_r=glm::abs(glm::radians(lon_lang));

    float x=lon_r_lang_r.y*a;
    float t=-1+glm::tan(static_cast<float>(3.14/4.0+lon_r_lang_r.x/2.0)*std::pow((1-e1*std::sin(lon_r_lang_r.x))/(1+1-e1*std::sin(lon_r_lang_r.x)),e1/2.0f));
    float y=a*std::log1p(t);

    if(lon_lang.y<0){
        x=-x;
    }
    if(lon_lang.x<0){
        y=-y;
    }

    //std::cout<<lon_lang.x<<"|"<<lon_lang.y<<" xy: "<<x<<"|"<<y<<std::endl;
    return {x,y};
}
inline static glm::vec2 max=Sphere_to_Mercator(glm::vec2{82.5,180 });
inline static glm::vec2 max_2=Sphere_to_Mercator(glm::vec2{76,180 });

glm::vec2 Sphere_to_Mercator_normalizate(glm::vec2 lon_lang){
    if(lon_lang.x>0){
        return Sphere_to_Mercator(lon_lang)/max;
    }else{
        return Sphere_to_Mercator(lon_lang)/max_2;
    }
}


class CeWorldMap: public CeObject{
    CePlate map;
    CePlate marker;
    std::vector<std::shared_ptr<CeObject>> markers;

    glm::mat4 proj{1.0};

    float Zoom=60.0f;

    std::function<void(std::string,std::string)> fun_get_lan_long;
public:
    struct Data_initialization{
        enum  TYPE_PASS:int {FORGET=0,REMEMBER};

        std::string name_them_list="";
        std::filesystem::path path_image="";
        std::filesystem::path path_marker_image="";

        std::function<void(std::string,std::string)> fun_rep=nullptr;

        template<typename Engine>
        Data_initialization(Engine * win,nlohmann::json &json, CePlagin_menedger * plugin_engine){
            get_to_check(json,"them",name_them_list);
            get_to_check(json,"path_image",path_image);
            get_to_check(json,"path_marker_image",path_marker_image);
            fun_rep =fun_in<CePlagin_menedger,std::string,std::string>(plugin_engine,json);
        };
        Data_initialization(std::filesystem::path path_image_,std::filesystem::path path_marker_image_):
        path_image(path_image_),path_marker_image(path_marker_image_){}
        operator CePlate::Data(){
            return CePlate::Data{path_image};
        }

        auto get_data_marker(){
            return CePlate::Data{path_marker_image};
        }

    };


    template<typename Engine>
    CeWorldMap(Engine *win_,typename Engine::List * celist_,Data_initialization data):
        map(celist_,data),
        marker(celist_,data.get_data_marker()),
        fun_get_lan_long(data.fun_rep){
        std::cout<<"CeWorldMap"<<std::endl;
        proj=glm::perspective(glm::radians(Zoom), 1.0f, 0.1f, 1000.0f);
        proj[2][2]=-1.0;
        *this->change_position().position=glm::vec3{0.0f,0.0f,-2.0f};
        this->change_position().position->set_projection(&proj);
    }
    //
    virtual void add_element(std::shared_ptr<CeObject> element){
        markers.emplace_back(element);
        markers.back()->change_position().position->set_view(this->get_position());
    }

   void connect(Signal_engine<CeObject> * signal){
        signal->connect(this,map.id_global,-1);
    }

    virtual void operator()(Win_layer::Incoming_signals_from_user * ISFU,typename CeObject::Interaction_Repetition_area){
        //auto scale = this->position_object.get_scale_object();
        if(ISFU->keys[MOUSE_BUTTON_LEFT].action==1){
           auto pos_w = this->get_position()->get_position_world();
           auto move=ISFU->move_to_cursor();

           float x_sm_1=(pos_w.x+move.x)/proj[0][0]-0.5;
           float x_sm_2=(pos_w.x+move.x)/proj[0][0]+0.5;

           float y_sm_1=(pos_w.y+move.y)/proj[1][1]-0.5;
           float y_sm_2=(pos_w.y+move.y)/proj[1][1]+0.5;

           bool b=(x_sm_1>0)+(x_sm_2<0)+(y_sm_1>0)+(y_sm_2<0);

           if(!b){
               this->change_position().position->move(glm::vec3{move.x/proj[0][0],move.y/proj[1][1],0.0f});
           }
        }

        if(ISFU->yoffset!=0){
            //pos_map.new_scale_offset(pos_map.get_scale_object()+glm::vec3{win->ISFU->yoffset*0.25f,win->ISFU->yoffset*0.25f,0.0f},-pos_map.cursor_position());
            Zoom+=-ISFU->yoffset;
            if(Zoom<1.0){
                Zoom=1.0;
            }
            if(Zoom>180.0){
                Zoom=180.0;
            }
            //std::cout<<Zoom<<std::endl;
            proj=glm::perspective(glm::radians(Zoom), 1.0f, 0.1f, 1000.0f);
            proj[2][2]=-1.0;
            this->change_position();

            //pos_map.new_scale_offset(pos_map.get_scale_object()+glm::vec3{win->ISFU->yoffset*0.1f,win->ISFU->yoffset*0.1f,0.0f},glm::vec3{-0.5,-0.5,0});
        }
        auto t=ISFU->cursor_position();
        auto pos_w = this->get_position()->get_position_world();

        if(ISFU->keys[MOUSE_BUTTON_RIGHT].action==1){
            marker.template change <Position>().get_data()->new_move(glm::vec3{t.x/proj[0][0]-pos_w.x,t.y/proj[1][1]-pos_w.y+1/(16.0f*3.0f)*0.6, 0.9500001f});
            std::cout<<(t.x/proj[0][0]-pos_w.x)<<"|"<<(t.y/proj[1][1]-pos_w.y)<<std::endl;
            if(fun_get_lan_long){
                fun_get_lan_long(std::to_string((t.y/proj[1][1]-pos_w.y)*82*2),std::to_string((t.x/proj[0][0]-pos_w.x)*2*180));
            }
        }

        //


    };

    void placement_of_elements(){
        map.template change <Position>().get_data()->move(glm::vec3{0.0f, 0.0f, 0.95f});
        //marker.template change <Position>().get_data()->new_move(glm::vec3{0.0f, 0.0f, 0.9500001f});
        marker.template change <Position>().get_data()->scale(glm::vec3{1/(16.0f*3.0f),1/(16.0f*3.0f), 1.0f});
    }

    virtual void update_position(const Position *view){
        map.template change <Position>().get_data()->set_view(view);
        marker.template change <Position>().get_data()->set_view(view);
        for(auto &TT:markers){
            TT->change_position().position->set_view(view);
        }
    }

    void installing_the_theme(){

    }

    virtual void update_theme(Them_centre_::Them_object_base_base * theme){//а вот тут нужен будет массив

    }
};
}// namespace CeEngine
#endif // CEWORLDMAP_H
