#ifndef CUSTOM_OBJECT_LIST_TEMPLATE_H
#define CUSTOM_OBJECT_LIST_TEMPLATE_H

#include <string>
#include <filesystem>
#include <any>

#include "ceobject.h"

#include "CePlate.h"
#include "CeText.h"
namespace CeEngine {
struct Data_Point_WorldMap{
   // Data_Bottom bottom_MC;
    float lon, lang=0;
    std::string name;
    int type_base=-1;
    std::string CC_code="";
    int wmo_id=0;

    std::filesystem::path path_font;
    std::filesystem::path path_image;
    Data_Point_WorldMap(int wmo_id_,std::string CC_code_,float lon_, float lang_, int type_base_, std::string name_,std::filesystem::path path_font_,std::filesystem::path path_image_):
        lon(lon_),lang(lang_),name(name_),type_base(type_base_),wmo_id(wmo_id_),CC_code(CC_code_),
        path_font(path_font_),path_image(path_image_){
    }

    operator CePlate::Data(){
        return CePlate::Data{path_image};
    }
    operator CeText::Data(){
        return CeText::Data{path_font,name};
    }
};

#include "URL_curl.h"

Type_curl API_rp5_ru(Data_Point_WorldMap data,int i=1){
    std::string https=" ";
    std::string data_pos="      ";
    Type_curl atribute;


    if(data.type_base==1){

    }

    if(data.type_base==0){
        std::string data_from{"16.12.2022"};
        std::string data_to{"22.12.2022"};

        //https="http://rp5.ru/responses/reFileSynop.php";
        https="http://rp5.ru";
        data_pos=
        "wmo_id="+std::to_string(data.wmo_id)+
        "&a_date1="+data_from+
        "&a_date2="+data_to+
        "&f_ed3="+std::to_string(12)+
        "&f_ed4="+std::to_string(12)+
        "&f_ed5="+std::to_string(23)+
        "&f_pe="+std::to_string(1)+
        "&f_pe1="+std::to_string(1)+
        "&lng_id="+std::to_string(2)+
        "&type="+"xls";
    }
    std::cout<<https<<std::endl;
    std::cout<<data_pos<<"|"<<data_pos.size()<<std::endl;

    Curl_list list;



////cайт
    list.append("Accept: host/uri, */*;");
    //list.append("Accept: text/html, */*; q=0.01");
    list.append("Accept-Language: ru,en");//
    //list.append("Accept-Encoding: gzip, deflate, br");
    list.append("Cache-Control: no-cache");
    list.append("Connection: keep-alive");
    //list.append("Content-Encoding:gzip");
    //list.append("Content-Length:110");
    list.append("Content-Type: application/x-www-form-urlencoded");
    list.append("DNT: 1");
    list.append("Origin: https://rp5.ru");
    list.append("Pragma: no-cache");
    list.append("Referer: https://rp5.ru/");
    list.append("Sec-Fetch-Dest: empty");
    list.append("Sec-Fetch-Mode: cors");
    list.append("Sec-Fetch-Site: same-origin");
    list.append("User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 YaBrowser/19.6.1.153 Yowser/2.5 Safari/537.36");
//    list.append("X-Requested-With: XMLHttpRequest");//
    list.append(R"(sec-ch-ua: "Not?A_Brand";v="8", "Chromium";v="108", "Yandex";v="23")");
    list.append("sec-ch-ua-mobile: ?0");
    list.append(R"(sec-ch-ua-platform: "Windows")");


    //std::string cookie="extreme_open=true; tab_metar=2; tab_wug=1; __utmz=66441069.1670767473.7.3.utmcsr=yandex|utmccn=(organic)|utmcmd=organic; tab_synop=2; PHPSESSID=43dc883a0f3ea8065d938c8c7bdcb7ac; format=xls; f_enc=ansi; __utma=66441069.1845303057.1669569517.1671483271.1671797457.15; __utmc=66441069; __utmt=1; i=10048%7C6926%7C90612%7C5652%7C5483; iru=10048%7C6926%7C90612%7C5652%7C5483; ru=%D0%A1%D0%B5%D0%BA%D1%82%D0%B0%D0%B3%D0%BB%D0%B8%7C%D0%9F%D1%80%D0%BE%D1%85%D0%BB%D0%B0%D0%B4%D0%BD%D1%8B%D0%B9%7C%D0%94%D0%B6%D0%B5%D0%BD%D0%B0%D0%BB%7C%D0%9D%D0%B0%D1%80%D1%82%D0%BA%D0%B0%D0%BB%D0%B0%7C%D0%9C%D0%BE%D1%81%D0%BA%D0%B2%D0%B0%20%28%D0%92%D0%94%D0%9D%D0%A5%29; last_visited_page=http%3A%2F%2Frp5.ru%2F%D0%9F%D0%BE%D0%B3%D0%BE%D0%B4%D0%B0_%D0%B2_%D0%9C%D0%BE%D1%81%D0%BA%D0%B2%D0%B5_%28%D0%92%D0%94%D0%9D%D0%A5%29; __utmb=66441069.6.10.1671797457; lang=ru";

//    list.append("Pragma:no-cache");
//
//    list.append("Transfer-Encoding:chunked");
//
//    list.append("Connection:keep-alive");
//    list.append("Accept:text/html, */*; q=0.01");
//    list.append("Accept-Encoding:gzip, deflate, br");
//    list.append("Accept-Language:ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4");
//    list.append("Content-Type:application/x-www-form-urlencoded");
//    list.append("Host:rp5.ru");
//    list.append("Origin:https://rp5.ru");
//    list.append("Referer:https://rp5.ru/");

//    list.append("X-Requested-With:XMLHttpRequest");
//    list.append("DNT:1");
//    list.append("Accept-Language:ru,en");
//    list.append("Pragma:no-cache");
//    list.append("Cache-Control:no - cache");
//    list.append("User-Agent:Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 YaBrowser/22.11.4.696 (beta) Yowser/2.5 Safari/537.36");


    std::string cookie="extreme_open=false;"
                       "tab_wug=1;"
                       "i=5483%7C5483;"
                       "iru=5483%7C5483;"
                       "ru=%D0%9C%D0%BE%D1%81%D0%BA%D0%B2%D0%B0+%28%D0%92%D0%94%D0%9D%D0%A5%29%7C%D0%9C%D0%BE%D1%81%D0%BA%D0%B2%D0%B0+%28%D0%92%D0%94%D0%9D%D0%A5%29;"
                       "last_visited_page=https://rp5.ru;"
                       "tab_synop=2; "
                       "__utma=66441069.1845303057.1669569517.1670102001.1670767473.7;"
                       " __utmc=66441069;"
                       " __utmz=66441069.1561820218.1.1.utmcsr;"
                       " tab_metar=2; "
                       "is_adblock=0;"
                       "f_enc=ansi;"
                       "format=xls;"
                       "__utmt=1;"
                       "lang=ru;"
                       "__utmb=66441069.6.10.1561820218;"
                       "PHPSESSID=43dc883a0f3ea8065d938c8c7bdcb7ac;";


    atribute.set_data<CURLOPT_URL>(https);
    atribute.set_data<CURLOPT_HTTPHEADER>(list);
   // atribute.set_data<CURLOPT_POST>(1);
    //atribute.set_data<CURLOPT_POSTFIELDSIZE>(data_pos.size());
    //atribute.set_data<CURLOPT_POSTFIELDS >(data_pos);//?????
  //  atribute.set_data<CURLOPT_COOKIE>(cookie);
    std::string t=R"(C:\Users\ARHIV\Desktop\1122334455.txt)";
    //atribute.set_data<CURLOPT_COOKIEJAR>(t);

    return atribute;
}

#include "CeWorldMap.h"



class  CePoint_WorldMap: public CeObject{
    //то что всегда видно
    CePlate MC;
    CeText MC_name;
    inline static int id=0;
    Data_Point_WorldMap data;
public:
    inline static std::string current_path___="";
    URL_curl curl{};
    std::function<void(std::string)> fun_cursor=nullptr;

    template<typename Engine>
    CePoint_WorldMap(Engine *win_,typename Engine::List * celist_,typename CeObject::Data data_):
        data(std::any_cast<Data_Point_WorldMap>(data_)),
        MC(celist_,std::any_cast<Data_Point_WorldMap>(data_).operator CePlate::Data()),
        MC_name(celist_,std::any_cast<Data_Point_WorldMap>(data_).operator CeText::Data())   {

        fun_cursor=[win_](std::string s){
              win_->set_cursor(s);
        };
    }


    virtual void operator()(Win_layer::Incoming_signals_from_user * ISFU,typename CeObject::Interaction_One_input_area){
       fun_cursor("STANDART_HAND");
    }

    virtual void operator()(Win_layer::Incoming_signals_from_user * ISFU, typename CeObject::Exit_area){
       fun_cursor("STANDART_ARROW");
    }

    virtual void operator()(Win_layer::Incoming_signals_from_user * ISFU, typename CeObject::Interaction_Repetition_area){
        if(ISFU->keys[GLFW_MOUSE_BUTTON_LEFT].action==1){
            ISFU->keys[GLFW_MOUSE_BUTTON_LEFT].action=0;

            float lon=std::any_cast<Data_Point_WorldMap>(data).lon;
            float lang=std::any_cast<Data_Point_WorldMap>(data).lang;
            //curl.ptint_test();

            //std::string path=exploer_call___.get_path().string();
            curl.request(API_rp5_ru(std::any_cast<Data_Point_WorldMap>(data)));
            std::cout<<"CePoint_WorldMap: "<<std::any_cast<Data_Point_WorldMap>(data).wmo_id<<":"<<lon<<"|"<<lang<<std::endl;
        }
    }
    void connect(Signal_engine<CeObject> * signal){
        signal->connect(this,MC.id_global,-1);
        signal->connect(this,MC_name.id_global,-1);
    }

    virtual void update_position(const Position *view){
        MC.template change <Position>().get_data()->set_view(view);
        MC_name.template change <Position>().get_data()->set_view(view);
    }

    void placement_of_elements(){
        auto mc_ch=MC.template change <Position>();
        auto mc_name_ch=MC_name.template change <Position>();
        glm::vec2 xy=Sphere_to_Mercator_normalizate(glm::vec2{data.lon,data.lang});
        mc_name_ch.get_data()->move(glm::vec3{xy.x/(2.0f)-1/(640.0f*1.4f*4.0f)*0.25,xy.y/(2.0f)+1/(640.0f*1.4f)*1.8f,0.950001f});
        mc_name_ch.get_data()->scale(glm::vec3{1/(640.0f*1.4f*4.0f),1/(320.0f*1.4f*4.0f),1.0f});
        mc_ch.get_data()->move(glm::vec3{xy/(1*2.0f),0.950001f});
        mc_ch.get_data()->scale(glm::vec3{1/(640.0f*1.4f),1/(320.0f*1.4f),1.0f});
    }

};


//struct Data_WorldMap{
//    Data_CePlate plate;
//};
}// namespace CeEngine

#endif // CUSTOM_OBJECT_LIST_TEMPLATE_H
