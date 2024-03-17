#ifndef CEGRAPHICS_H
#define CEGRAPHICS_H
#include "CePlate.h"
#include "CeAxis.h"

#include "utiliti_json.h"
namespace CeEngine {
class CeGraphics: public CeObject{
    CePlate backgraund;

    std::unordered_map<std::string, std::shared_ptr<CeAxis>> axis_H;
    std::unordered_map<std::string, std::shared_ptr<CeAxis>> axis_V;

    std::unordered_map<std::string, std::shared_ptr<CeGraph>> lines;

public:
    template<typename Engine>
    void add_element(std::vector<float> && data_,Color color,std::string list,std::string name,std::string name_x_axis,std::string name_y_axis,Engine* win){
        //обработка данных
        float size=data_.size()-1;

        float max=*std::max_element(data_.begin(),data_.end());
        float min=*std::min_element(data_.begin(),data_.end());

        //загрузка листа
        auto celist=win->resource.get_list(list);

        //создание графика


        if(lines.find(name)!=lines.end()){
            lines.erase(name);

            axis_H[name_x_axis]->delete_graf(name);
            axis_V[name_y_axis]->delete_graf(name);
            std::cout<<lines.size()<<"|dddddddddddddddddddddddddddddddddddd|"<<std::endl;
        }

        auto line=std::make_shared<CeGraph>(win,celist,CeLine::Data(std::make_shared<Mesh_data>(Mesh_data{std::move(data_)})));

        lines.emplace(name,line);

        //настройка графика


        Them_centre_::Them_object_base_base theme;
        theme.set_data<Enum_them::color_line>(color);

        lines[name]->update_theme(&theme);

        std::cout<<max<<"|jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj|"<<min<<std::endl;
        lines[name]->change_position().position->move(glm::vec3{-0.5f,-0.5f-min/(max-min),-0.1f});
        lines[name]->change_position().position->scale(glm::vec3{1.0f/size,1.0f/(max-min),1.0f});
        //присоединение осей
        axis_H[name_x_axis]->add_graf(name,typename CeAxis::Graf_contoroller{size,0,line});
        axis_V[name_y_axis]->add_graf(name,typename CeAxis::Graf_contoroller{max,min,line});

        this->change_position();
    }


    struct Data_initialization{
        std::filesystem::path path_image="";

        std::unordered_map<std::string,Setting_axis> setting_axis_H;
        std::unordered_map<std::string,Setting_axis> setting_axis_V;

        std::function<void(void *,CeGraphics *)> funs_out;

        template<typename Engine>
        inline static std::function f{
            [](std::vector<float> && data,uint32_t color,std::string list,std::string name,std::string x,std::string y, void * win, CeGraphics * ptr){
            if(ptr!=nullptr){
               ptr->add_element(std::move(data),color,list,name,x,y,static_cast<Engine *>(win));
            }
        }};


        template<typename Engine>
        Data_initialization(Engine * win,nlohmann::json &json,CePlagin_menedger * plugin_engine):funs_out(fun_out<2>(plugin_engine,json,f<Engine>)){
            get_to_check(json,"path_image",path_image);

            for(auto &[M,J]:json["Setting_axis"].items()){
                Setting_axis setting;
                get_to_check(J,"type",setting.type);
                get_to_check(J,"name_axis",setting.name_axis);
                get_to_check(J,"accurate",setting.accurate);
                get_to_check(J,"cout_divisions",setting.cout_divisions);
                get_to_check(J,"font",setting.path_font);

                if(setting.type==1){
                    setting_axis_H.emplace(setting.name_axis,setting);

                }else{
                    setting_axis_V.emplace(setting.name_axis,setting);
                }
            }
        }

        Data_initialization(std::filesystem::path path_image_):path_image(path_image_){}
        operator CePlate::Data(){
            return CePlate::Data{path_image};
        }
    };

    template<typename Engine>
    CeGraphics(Engine *win_,typename Engine::List * celist_,Data_initialization data):
        backgraund(celist_,data){

        //std::cout<<data.funs_out.size()<<">"<<std::endl;
        data.funs_out(win_,this);

        //переделать
        for (auto &[S,T]:data.setting_axis_H) {
            auto t=std::make_shared<CeAxis>(win_,celist_,typename CeAxis::Data_initialization(T));
            axis_H.emplace(T.name_axis,t);
            axis_H[T.name_axis]->placement_of_elements();
            axis_H[T.name_axis]->installing_the_theme();
        }

        for (auto &[S,T]:data.setting_axis_V) {
            auto t=std::make_shared<CeAxis>(win_,celist_,typename CeAxis::Data_initialization(T));
            axis_V.emplace(T.name_axis,t);
            axis_V[T.name_axis]->placement_of_elements();
            axis_V[T.name_axis]->installing_the_theme();
        }
    }


    void placement_of_elements(){
        int i=0;
        for(auto &[SS,TT]:axis_H){
            TT->change_position().position->move(glm::vec3{-0.5f,-0.5f-i*0.2,-0.01f});
            i++;
        }

        i=0;
        for(auto &[SS,TT]:axis_V){
            TT->change_position().position->rotete(glm::vec3(1.0f, 1.0f, 0.0f),180.0f);
            TT->change_position().position->move(glm::vec3{-0.5f-i*0.15,-0.5f,-0.03f});
            i++;
        }
    }

    virtual void update_position(const Position *view){
        backgraund.template change <Position>().get_data()->set_view(view);
        for(auto &[SS,TT]:lines){
            TT->change_position().position->set_view(view);
        }
        for(auto &[SS,TT]:axis_H){
            TT->change_position().position->set_view(view);
        }
        for(auto &[SS,TT]:axis_V){
            TT->change_position().position->set_view(view);
        }
    }



    void installing_the_theme(){

    }

    virtual void update_theme(Them_centre_::Them_object_base_base * theme){//а вот тут нужен будет массив

    }
};
}// namespace CeEngine

#endif // CEGRAPHICS_H
