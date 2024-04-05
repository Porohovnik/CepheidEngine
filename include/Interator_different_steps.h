#ifndef INTERATOR_DIFFERENT_STEPS_H
#define INTERATOR_DIFFERENT_STEPS_H

namespace CeEngine {
template <typename T>
class Interator_different_steps:public T{
public:
    unsigned int data_step=1;
    Interator_different_steps(T &&inter,unsigned int data_step_):T(inter),data_step(data_step_){}
    Interator_different_steps( ){}

    auto operator++(){
        for (int i=0;i<data_step;i++) {
            this->T::operator++();
        }
        return *this;
    }
    auto operator++(int){
        auto t=*this;
        for (int i=0;i<data_step;i++) {
            this->T::operator++();
        }
        return t;
    }
};
} // namespace CeEngine


#endif // INTERATOR_DIFFERENT_STEPS_H
