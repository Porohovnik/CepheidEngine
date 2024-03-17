#ifndef DATA_ABC_H
#define DATA_ABC_H
#include "CeEnumeration.h"
#include "GL_type_to_cpp.h"
namespace CeEngine {
template<TIME_BIND bind_time,typename T,typename J=T>
class Data_ABC{
public:
    Data_ABC(){}

    Data_ABC(const Data_ABC& )=delete;
    Data_ABC(Data_ABC&& )=delete;
    Data_ABC & operator=(Data_ABC &&)=delete;
    Data_ABC & operator=(const Data_ABC &)=delete;

    static T Type();
    static J Type_gl();
    static constexpr TIME_BIND bind_time_(){return bind_time;}

    inline constexpr static TYPE_CONTAINER type_container=TYPE_CONTAINER::ARRAY;
    inline constexpr static TYPE_RES_STORAGE type_res_=TYPE_RES_STORAGE::LOCAL;

    template<typename M>
    class Change_guard{
        M change;
    public:
        template<typename ...Arg>
        Change_guard(Arg...arg):change(arg...){}

        inline auto * get_data(){
            return change.data;
        }

        ~Change_guard(){
            change.confirm();
        }
    };
public:
    inline constexpr static TRIVIAL_DELETE trivial_delete=TRIVIAL_DELETE::YES;

};

template<typename Memory, typename Info_environment>
struct Data_herald{
    Memory *res;
    Info_environment * info;
};
}// namespace CeEngine
#endif // DATA_ABC_H
