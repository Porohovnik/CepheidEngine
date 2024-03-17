#ifndef LOAD_FILE_H
#define LOAD_FILE_H
#include <sstream>
#include <fstream>
#include <filesystem>

#include <iostream>
namespace CeEngine {
std::string load_file(std::filesystem::path path){
    std::cout<<":::" <<path.string() .c_str()<<std::endl;
    std::string data;
    std::ifstream file{path};
    file.exceptions(std::ifstream::badbit);
    try{
        std::stringstream file_stream;
        file_stream << file.rdbuf();

        data=file_stream.str();

        file.close();
    }
    catch (std::ifstream::failure e){
        std::cout<<data<<std::endl;
        std::cout<<"FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    return data;
}

template<std::size_t n>
std::array<std::string,n> load_files(std::array<std::filesystem::path,n> pachs){
    std::array<std::string,n> data;
    for(int i=0;i<n;i++){
        data[i]=load_file(pachs[i]);
    }
    return data;
}
}// namespace CeEngine
#endif // LOAD_FILE_H
