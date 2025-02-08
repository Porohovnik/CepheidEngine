#include "incoming_signals_from_user.h"

Incoming_signals_from_user::Incoming_signals_from_user(GLFWwindow* window){

    std::cout<<"ISFU"<<GLFW_KEY_BACKSPACE<<"|"<<GLFW_KEY_ENTER<<"|"<<std::endl;
    glfwSetKeyCallback(window,key_callback);
    glfwSetScrollCallback(window,scroll_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window,cursor_position_callback);
    //glfwSetWindowFocusCallback(window, );
    glfwSetDropCallback(window,drop_callback);
    setting_curs.inzialization_cursor();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetWindowUserPointer(window,this);
    glfwGetFramebufferSize(window, &width, &height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
};

void Incoming_signals_from_user::cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    auto current= reinterpret_cast<Incoming_signals_from_user*>(glfwGetWindowUserPointer(window));
    current->setting_curs.lastX=current->setting_curs.xpos;
    current->setting_curs.lastY=current->setting_curs.ypos;

    current->setting_curs.xpos=xpos;
    current->setting_curs.ypos=ypos;
    //glfwGetCursorPos(window,&current->setting_curs.xpos,&current->setting_curs.ypos);
    //std::cout<<current->setting_curs.last_lastX<<"|"<<current->setting_curs.lastX<<"|"<<current->setting_curs.xpos<<std::endl;
}

void Incoming_signals_from_user::new_frame(){
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    //std::cout<<"FPS: "<<static_cast<int>(1/(deltaTime))<<std::endl;
    setting_curs.lastX=setting_curs.xpos;
    setting_curs.lastY=setting_curs.ypos;
    yoffset=0.0;
    input_code_text=1000000000;
//    double xpos=0.0;
//    double ypos=0.0;
   // glfwGetCursorPos(window,&setting_curs.xpos,&setting_curs.ypos);

//
    isChangeSize=false;
    glfwPollEvents();
};

void Incoming_signals_from_user::drop_callback(GLFWwindow* window, int count, const char** paths){
    auto current= reinterpret_cast<Incoming_signals_from_user*>(glfwGetWindowUserPointer(window));
    std::cout<<*paths<<std::endl;
    current->path_drop.clear();
    for(int i=0;i<count;i++){
        current->path_drop.push_back(paths[i]);
    }
}

void Incoming_signals_from_user::key_callback(GLFWwindow* window, int key, int scancode, int action,int mode){
    auto current= reinterpret_cast<Incoming_signals_from_user*>(glfwGetWindowUserPointer(window));
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
        glfwHideWindow (window);
    }

    if (key >= 0 && key < 1024){
        if(action == GLFW_PRESS || action==GLFW_REPEAT){
            current->keys[key] = {key,scancode,action,mode};
//            if(current->ineteraction[key]!=nullptr ){
//                current->ineteraction[key](current->input_text,current->input_code_text);
//            }
        }else if(action == GLFW_RELEASE){
            current->keys.erase(key);
        }
    }
};


void Incoming_signals_from_user::character_callback(GLFWwindow* window, unsigned int codepoint){
    auto current= reinterpret_cast<Incoming_signals_from_user*>(glfwGetWindowUserPointer(window));
    current->input_code_text=codepoint;
//        if(codepoint<256){
//            current->input_text+=static_cast<char>(codepoint);
//        }else{
//            char out[2];
//            wchar_t *in=new wchar_t(static_cast<wchar_t>(codepoint));
//            std::wcstombs(out,in,2);
//            current->input_text+=out;
//        }

}

void Incoming_signals_from_user::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    auto current= reinterpret_cast<Incoming_signals_from_user*>(glfwGetWindowUserPointer(window));
    if (button >= 0 && button < 1024){

        if(action == GLFW_PRESS){
            current->keys[button] = {button,0,action,mods};
        }else if(action == GLFW_RELEASE){
            current->keys.erase(button);
        }
    }

   // std::cout<<button<<std::endl;
};

void Incoming_signals_from_user::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
     auto current= reinterpret_cast<Incoming_signals_from_user*>(glfwGetWindowUserPointer(window));
     current->yoffset=yoffset;
}

void Incoming_signals_from_user::framebuffer_size_callback(GLFWwindow* window, int width, int height){
    auto current= reinterpret_cast<Incoming_signals_from_user*>(glfwGetWindowUserPointer(window));
    current->width=width;
    current->height=height;

    current->isChangeSize=true;
}

