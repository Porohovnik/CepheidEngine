#ifndef CEAXIS_H
#define CEAXIS_H
#include "ceobject.h"

#include "CeText.h"
#include "CeLine.h"

#include "CeGraph.h"
#include "utiliti_json.h"
namespace CeEngine {
struct Setting_axis{
    enum TYPE_POSITON_AXIS:int{VERTICAL=0, HORIZONTAL};\

    std::string name_axis;
    float max=0;
    float min=0;
    int type=HORIZONTAL;
    int cout_divisions=0;
    int accurate=2;
    std::filesystem::path path_font;

    std::vector<std::pair<std::string,float>> dimensions;
};


class CeAxis: public CeObject{
    Setting_axis setting;

    CeText name_axis;
    CeLine line;
    std::vector<std::shared_ptr<CeLine>> divisions_line;
   // std::list<CeLine<Engine>> grid;
    std::vector<std::shared_ptr<CeText>> name_divisions;

    float max=0;
    float min=0;

    float max_const=0;
    float min_const=0;

    float max_d=0;
    float min_d=0;

public:
    struct Graf_contoroller{
        float max;
        float min;
        std::shared_ptr<CeGraph> graf;
    };
private:
    std::unordered_map<std::string,Graf_contoroller> graf_menedgment;

public:
    struct Data_initialization{
        Setting_axis setting;
        template<typename Engine>
        Data_initialization(Engine * win,nlohmann::json &json,CePlagin_menedger * plugin_engine){}

        Data_initialization(Setting_axis setting_):
            setting(setting_){}
    };

    template<typename Engine>
    CeAxis(Engine *win_,typename Engine::List * celist_,Data_initialization data):
        setting(data.setting),
        name_axis(celist_,CeText::Data(setting.path_font,setting.name_axis)),
        line(celist_,CeLine::Data(line_mesh(),"line")){

        std::cout<<setting.name_axis<<" CeAxis: "<<setting.path_font<<std::endl;
        for (std::size_t i=0;i<setting.cout_divisions;i++) {
           divisions_line.emplace_back(std::make_shared<CeLine>(celist_,CeLine::Data(line_mesh(),"line")));
           // grid.          emplace_back(win_,celist_,Data_CeLine(line_mesh()));
           name_divisions.emplace_back(std::make_shared<CeText>(celist_,CeText::Data(setting.path_font,"1.0")));
        }
    }

    void change_divisions_text(float max,float min){
        float sum=max-min;

        std::size_t i=0;
        for (auto &T:name_divisions) {
            auto s=std::to_string(min+i*sum/static_cast<float>(setting.cout_divisions-1));
            s.erase(s.size()-6+setting.accurate,s.size()-1);
            if(setting.accurate==0){
                s.erase(s.find_first_of(','));
            }
            //std::cout<<i<<"|:::::::::::::"<<s<<std::endl;
            T->change_text(s.data());
            i++;
        }
    }

    void add_graf(std::string name,Graf_contoroller graf_contrl){
        graf_menedgment.emplace(name,graf_contrl);
        change();
    }

    void delete_graf(std::string name){
        graf_menedgment.erase(name);
        if(graf_menedgment.size()){
            change();
        }
    }

    void change(){

        max=std::max_element(graf_menedgment.begin(),graf_menedgment.end(),[](const auto &a,const auto &b){return std::abs(a.second.max)<std::abs(b.second.max);})->second.max;
        min=std::min_element(graf_menedgment.begin(),graf_menedgment.end(),[](const auto &a,const auto &b){return std::abs(a.second.min)<std::abs(b.second.min);})->second.min;


        if(static_cast<int>(min_const)!=static_cast<int>(max_const)){

            max=min_const;
            min=max_const;

        }
        if(static_cast<int>(min_d)!=static_cast<int>(max_d)){
            max=max_d;
            min=min_d;

        }

        change_divisions_text(max,min);
        std::cout<<"max:"<<max<<"|"<<"min:"<<min<<std::endl;


        for(auto &[s,t]:graf_menedgment){
            auto move_old= t.graf->change_position().position->get_position_world();
            auto scale_old=t.graf->change_position().position->get_scale_object();
            if(setting.type==Setting_axis::VERTICAL){

                t.graf->change_position().position->new_move(glm::vec3{move_old.x,-0.5f,-0.01f});
                t.graf->change_position().position->new_scale(glm::vec3{scale_old.x,1.0f,1.0f});

                t.graf->change_position().position->move(glm::vec3{0.0f,-min/(max-min),0.0f});
                t.graf->change_position().position->scale(glm::vec3{1.0f,1.0f/(max-min),1.0f});
            }
            if(setting.type==Setting_axis::HORIZONTAL){
                t.graf->change_position().position->new_move(glm::vec3{-0.5f,move_old.y,-0.01f});
                t.graf->change_position().position->new_scale(glm::vec3{1.0f,scale_old.y,1.0f});

                t.graf->change_position().position->move(glm::vec3{-min/(max-min),0.0f,0.0f});
                t.graf->change_position().position->scale(glm::vec3{1.0f/(max-min),1.0f,1.0f});
            }

        }
    }

    void placement_of_elements(){
        float t=1.0f/static_cast<float>(setting.cout_divisions-1) ;
        line.template change <Position>().get_data()->scale(glm::vec3{1.0f,1.0f,1.0f});
        line.template change <Position>().get_data()->new_move(glm::vec3{0.0f,0.0f,-0.01f});

        name_axis.template change <Position>().get_data()->scale(glm::vec3{0.07f,0.07f,1.0f});

        if(setting.type==Setting_axis::VERTICAL){
            name_axis.template change <Position>().get_data()->rotete(glm::vec3(1.0f, 1.0f, 0.0f),-180.0f);
            name_axis.template change <Position>().get_data()->new_move(glm::vec3{1.08f,-0.12f,-0.01f});
        }else {
            name_axis.template change <Position>().get_data()->new_move(glm::vec3{0.5f,-0.12f,-0.01f});
        }

        std::size_t i=0;
        for (auto &T:divisions_line) {
             T->template change <Position>().get_data()->scale(glm::vec3{0.05f,0.05f,1.0f});
             T->template change <Position>().get_data()->rotete(glm::vec3(1.0f, 1.0f, 0.0f),180.0f);
             T->template change <Position>().get_data()->new_move(glm::vec3{0.00f+t*i,-0.025f,-0.01f});
             i++;
        }
//        i=0;
//        for (auto &T:grid) {
//             T.template change <Position>().get_data()->scale(glm::vec3{2.0f,2.0f,1.0f});
//             T.template change <Position>().get_data()->rotete(glm::vec3(1.0f, 1.0f, 0.0f),180.0f);
//             T.template change <Position>().get_data()->new_move(glm::vec3{-1.0f+t*i,-0.1f,0.0f});
//             i++;
//        }
        i=0;
        for (auto &T:name_divisions) {
            T->template change <Position>().get_data()->scale(glm::vec3{0.06f,0.06f,1.0f});
            if(setting.type==Setting_axis::VERTICAL){
                T->template change <Position>().get_data()->rotete(glm::vec3(1.0f, 1.0f, 0.0f),-180.0f);
                T->template change <Position>().get_data()->new_move(glm::vec3{0.00f+t*i,-0.08f,-0.01f});
            }else {
                T->template change <Position>().get_data()->new_move(glm::vec3{-0.01f+t*i,-0.05f,-0.01f});
            }

            i++;
        }
    }


    void installing_the_theme(){
        line.template change <Material_L>().get_data()->color={65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};

        name_axis.template change <Material_T>().get_data()->color={65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};

        for (auto &T:divisions_line) {
             T->template change <Material_L>().get_data()->color={65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};
        }

        for (auto &T:name_divisions) {
             T->template change <Material_T>().get_data()->color={65/2.0f/256.0f, 74/2.0f/256.0f, 76/2.0f/256.0f, 1.0f};
        }


    }

    virtual void update_theme(Them_centre_::Them_object_base_base * theme){//а вот тут нужен будет массив
        line.template change <Material_L>().get_data()->color=theme->get_element_data<Enum_them::color_line>().normal;


        name_axis.template change <Material_T>().get_data()->color=theme->get_element_data<Enum_them::color_text>().normal;

        for (auto &T:divisions_line) {
             T->template change <Material_L>().get_data()->color=theme->get_element_data<Enum_them::color_line>().normal;
        }

        for (auto &T:name_divisions) {
             T->template change <Material_T>().get_data()->color=theme->get_element_data<Enum_them::color_text>().normal;
        }
    }



    virtual void update_position(const Position *view){
        name_axis.template change <Position>().get_data()->set_view(view);
        line.template change <Position>().get_data()->set_view(view);
        for(auto TT:divisions_line){
            TT->template change <Position>().get_data()->set_view(view);
        }
//        for(auto TT:grid){
//            TT->change_position().position->set_view(view);
//        }
        for(auto TT:name_divisions){
            TT->template change <Position>().get_data()->set_view(view);
        }
    }

};
}
#endif // CEAXIS_H
