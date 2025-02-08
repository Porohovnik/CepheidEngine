#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include <iostream>


#include "incoming_signals_from_user.h"

class Win{
    GLFWwindow* window=nullptr;
protected:
    bool active=true;

    inline GLFWwindow* create_win(std::string name_okno_,int WIDTH, int HEIGHT){
        std::cout<<"L"<<std::endl;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        //glfwWindowHint(GLFW_SAMPLES, 4);
        //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        std::cout<<"L1"<<std::endl;
        window=glfwCreateWindow(WIDTH, HEIGHT, name_okno_.data(), nullptr, nullptr);//<-тут создался конткст окна и всё с него

        if (window == nullptr){
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        return window;
    }
public:
    Incoming_signals_from_user  ISFU;

    Win(std::string name_okno_,int WIDTH, int HEIGHT):
        window(create_win(name_okno_,WIDTH, HEIGHT)),ISFU(this->window){
        std::cout<<"||"<<window<<std::endl;

        glfwMakeContextCurrent(window);
        glewExperimental = GL_TRUE;
        // Initialize GLEW to setup the OpenGL Function pointers
        if (glewInit() != GLEW_OK){
            std::cout << "Failed to initialize GLEW" << std::endl;
        }
        //glfwSwapInterval(0);
    }

    ~Win(){
        std::cout<<"eeeeeeeeeeeeeeeeeeeeeeee"<<std::endl;
        glfwDestroyWindow(window);
    }

    inline void swap_buffer(){
        glfwSwapBuffers(window);
    }

    inline void make_context(){
        glfwMakeContextCurrent(window);;
    }

    inline bool isOpen() const{
        return (!glfwWindowShouldClose(window) && active);
    }

    inline void set_cursor(std::string type){
        ISFU.setting_curs.set_cursor(this->window,type);
    }
};



#endif // WINDOW_H
