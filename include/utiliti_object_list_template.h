#ifndef UTILITI_OBJECT_LIST_TEMPLATE_H
#define UTILITI_OBJECT_LIST_TEMPLATE_H
#include <OpenXLSX.hpp>
#include <filesystem>
#include <any>
#include <functional>

#include "tuple_utl.h"
#ifdef WIN32
    #include "winsock2.h"
    #include <windows.h>
#endif

#include "interlayer_gl.h"
#include "image.h"

template<GLenum type_sheme_pixel=GL_RGB,GLenum type_byte=GL_UNSIGNED_BYTE>
inline static void create_screenshot(std::size_t x,std::size_t size_x,std::size_t y,std::size_t size_y,std::filesystem::path path){
    Image im;
    im.widths=size_x;
    im.heights=size_y+1;
    if constexpr(type_sheme_pixel==GL_RGB){
        im.pixel_size=3;
    }
    im.fill({0,0,0,0});
    std::cout<<path<<std::endl;
    GL_layer::read_pixel<type_sheme_pixel,type_byte>(x,y,size_x,size_y,im.image.get());
    std::cout<<im.image.get()[0]<<std::endl;
    to_png(path,im);
}


class CeExploer{
    std::filesystem::path path="";
public:
    CeExploer(){
        std::cout<<"CeExploer"<<std::endl;
        if(sizeof (wchar_t)==2){
            std::setlocale(LC_ALL,"Russian");
            std::setlocale(LC_ALL,"ru_RU.UTF8");
        }else{
            std::setlocale(LC_ALL,"ru_RU.UTF8");
        }
    }

    std::filesystem::path get_path(){
        return path;
    }

    void open_exploer(std::string name=""){
        #ifdef WIN32
            OPENFILENAME ofn;
            name.resize(1024);

            memset(&ofn,0,sizeof(ofn));
            ofn.lStructSize     = sizeof(ofn);
            ofn.hwndOwner       = nullptr;
            ofn.hInstance       = nullptr;
            ofn.lpstrFilter     = nullptr;
            ofn.lpstrFile       = name.data();
            ofn.nMaxFile        = MAX_PATH;
            ofn.lpstrTitle      = nullptr;
            ofn.Flags           =0;
            GetSaveFileName(&ofn);

            std::cout<<name<<std::endl;

            std::size_t result_u = MultiByteToWideChar(1251,0, name.data(),-1,nullptr,0);
            std::vector<wchar_t> ures(result_u);

            if (!result_u || !MultiByteToWideChar(1251,0,name.data(),-1, ures.data(),result_u)){
               return;
            }

            std::size_t result_c = WideCharToMultiByte(CP_UTF8,0,ures.data(), -1,nullptr,0, nullptr, nullptr);
            std::string cres(" ",result_c);

            if(!result_c || !WideCharToMultiByte(CP_UTF8,0,ures.data(), -1,cres.data(),result_c,nullptr, nullptr)){
                return;
            }

            std::cout<<cres<<std::endl;
            path=name;
        #else
            char filename[1024];
            fgets(filename, 1024, popen("zenity --file-selection", "r"));
            path=filename;
        #endif
    }

};


class Exel_use{
    OpenXLSX::XLDocument doc;
public:   
    class TYPE_FAILE_OPEN{};
    class TYPE_FAILE_CREATE{};

    Exel_use(std::string patch_faile,TYPE_FAILE_OPEN){
        doc.open(patch_faile);
        std::cout<<"Open"<<std::endl;
    }

    Exel_use(std::filesystem::path patch_faile,TYPE_FAILE_CREATE){
        doc.create(patch_faile.string());
    }
    Exel_use(){}
    Exel_use(const Exel_use & t){}
    auto open_list(std::string name_list){
        return  doc.workbook().worksheet(name_list);

    }

    inline static OpenXLSX::XLCellRange range(OpenXLSX::XLWorksheet & wks,std::size_t begin_cell,std::size_t begin_columns,std::size_t end_cell,std::size_t end_columns){
        auto begin=OpenXLSX::XLCellReference(begin_cell,begin_columns);
        auto end=OpenXLSX::XLCellReference(end_cell,end_columns);
        return wks.range(begin, end);
    }

    template<typename DATA>
    inline static void read_data(OpenXLSX::XLCellRange range,std::vector<DATA> * data){
        data->resize(range.numRows()*range.numColumns());
        std::cout<<range.numRows()*range.numColumns()<<std::endl;
        std::size_t i=0;
        for(auto &T:range){
            if constexpr(std::is_same_v<float,DATA>){
                auto s=T.value().asString();
                auto f= s.find_first_of(".");
                if(f!= std::string::npos){
                    s[f]=',';
                }

                (*data)[i++]=std::stof(s);

            }
            if constexpr(std::is_same_v<int,DATA>){
                (*data)[i++]=std::stoi(T.value().asString());
            }
            if constexpr(std::is_same_v<std::string,DATA>){
                (*data)[i++]=T.value().asString();
            }

        }
    }

    template<typename DATA>
    inline void save_data(OpenXLSX::XLCellRange range,std::vector<DATA> * data,std::size_t accurate=2){
        std::size_t i=0;
        for(auto &T:range){
            std::string s;
            if constexpr(std::is_same_v<std::string,DATA>){
                s=(*data)[i];
            }else {
                s=std::to_string((*data)[i]);
                s.erase(s.size()-6+accurate,s.size()-1);
                if(accurate==0){
                    s.erase(s.find_first_of(','));
                }
            }

            T.value()=s;

            i++;
            if(i>data->size()){
               break;
            }
        }
        doc.save();
    }

    ~Exel_use(){
        //doc.save();
        doc.close();
    }
};

//сделать версию для tuple
template<typename ...Arg>
static void save_data_column(Exel_use & exel,OpenXLSX::XLWorksheet &list_save,std::size_t begin_line,std::size_t begin,Arg &...arg){
    std::size_t k=begin;
    ((exel.save_data(Exel_use::range(list_save,1+begin_line,1+(k++),1+begin_line+arg.size()-1,1+k),&arg)),...);

    //((std::cout<<1+(k++)<<"|"<<1+arg.size()<<"|"<<1+k<<std::endl),...);
}


template <typename T, size_t... i>
inline void save_data_column__t(Exel_use & exel,OpenXLSX::XLWorksheet &list_save,std::size_t begin,T&& t,std::index_sequence<i...>){
    save_data_column(exel,list_save,begin,std::get<i>(t)...);
}

template<typename T>
inline void save_data_column_t(Exel_use & exel,OpenXLSX::XLWorksheet &list_save,std::size_t begin,T t) {
    save_data_column__t(exel,list_save,begin,std::forward<T>(t),std::make_index_sequence<std::tuple_size<std::remove_reference_t<T>>::value>());
}




//по одному для каждой DLL



std::tm getEpochTime(std::string dateTime,std::string dateTimeFormat){
   //static const std::string dateTimeFormat{"%d.%m.%Y"};
   std::stringstream ss{dateTime};
   std::tm dt;
   ss >> std::get_time(&dt, dateTimeFormat.c_str());
   return dt;
}


#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    reinterpret_cast<std::string *>(userp)->append(reinterpret_cast<char*>(contents), size * nmemb);
    std::cout<<size<<"|"<<nmemb<<std::endl;
    return size * nmemb;
}


class Unit_curl{
    CURL *curl=nullptr;
public:
    Unit_curl(){
        curl_global_init(CURL_GLOBAL_ALL);
        curl=curl_easy_init();
    }
    ~Unit_curl(){
        curl_easy_cleanup(curl);
    }
    operator auto(){
        return curl;
    }
};


class Curl_list{
public:
    curl_slist * list=nullptr;
    Curl_list(){}
    template<typename T>
    Curl_list(T array){
        for(auto &J:array){
            append(J);
        }
    }

    ~Curl_list(){
        if(list!=nullptr){
           //curl_slist_free_all(list);
        }
    }
    void append(std::string data){
        list=curl_slist_append(list,data.data());
    }


    operator curl_slist * (){
        return list;
    }
};


using Type_curl= tutl::Type_to_data<
    tutl::KAAT<CURLOPT_URL,std::string>,
    tutl::KAAT<CURLOPT_HEADER,std::size_t>,
    tutl::KAAT<CURLOPT_HTTPHEADER,Curl_list>,
    tutl::KAAT<CURLOPT_POST,int>,
    tutl::KAAT<CURLOPT_POSTFIELDSIZE,std::size_t>,
    tutl::KAAT<CURLOPT_POSTREDIR,int>,
    tutl::KAAT<CURLOPT_POSTFIELDS,std::string>,
    tutl::KAAT<CURLOPT_COOKIE,std::string>,
    tutl::KAAT<CURLOPT_COOKIEJAR,std::string>,

    tutl::KAAT<CURLOPT_TIMEOUT,std::size_t>,
    tutl::KAAT<CURLOPT_WRITEFUNCTION,size_t(*)(void *, size_t, size_t, void *)>,
    tutl::KAAT<CURLOPT_WRITEDATA,void *>
>;


class URL_curl{
    inline static Unit_curl curl{};

    template<typename T>
    inline constexpr T & Type_to(T &t){
        return t;
    }
    inline auto Type_to(Curl_list &t){
        return t.operator curl_slist *();
    }

    inline auto Type_to(std::string  &t){
        return t.c_str();
    }
public:
    URL_curl(){}

    void set_atribute(Type_curl &info){
        auto data=info.get_data();
        tutl::TupleForeach(data,[this](auto &t){
            if(t.data.has_value()){
                curl_easy_setopt(curl, t.type,this->Type_to(*t.data));
            }
        });
    }

    void request(Type_curl &&info){
        set_atribute(info);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        //curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);

        auto res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            std::cout<<"|: "<<curl_easy_strerror(res)<<std::endl;
        }
        curl_easy_reset(curl);
    }


    void ptint_test(){
        std::string readBuffer;

        curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        auto res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            std::cout<<"|: "<<curl_easy_strerror(res)<<std::endl;
        }
        curl_easy_reset(curl);
        std::cout << readBuffer << std::endl;
    }
};








#endif // UTILITI_OBJECT_LIST_TEMPLATE_H
