#ifndef TWO_ARRAY_SYNCHCRONIC_H
#define TWO_ARRAY_SYNCHCRONIC_H
#include <vector>

template<class T1,class T2>
struct pair_upgraid{

T1 & first(){
    return *first_;
}
T2 & second(){
    return *second_;
}
pair_upgraid(std::pair<T1 *,T2 *> pair):first_(pair.first),second_(pair.second){}
private:
T1 * first_;
T2 * second_;
};

template<class T1,class T2>
class Two_array_synchcronic{

std::vector<T1> array_1{};
std::vector<T2> array_2{};
public:
Two_array_synchcronic(){}
Two_array_synchcronic(int i):array_1(i),array_2(i){}

void push_back(std::pair<T1,T2> insert){
    array_1.push_back(insert.first);
    array_2.push_back(insert.second);
}

void clear(){
    array_1.clear();
    array_2.clear();
}

void push_back(T1 insert_1,T2 insert_2){
    array_1.push_back(insert_1);
    array_2.push_back(insert_2);
}

pair_upgraid<T1,T2>  back(){
    return pair_upgraid<T1,T2>(std::pair<T1*,T2*>(array_1.data()+array_1.size()-1,array_2.data()+array_2.size()-1));
};


std::pair<T1*,T2*> begin(){
    return std::pair<T1*,T2*>(array_1.data(),array_2.data());
};

std::pair<T1*,T2*> end(){
    return std::pair<T1*,T2*>(array_1.data()+array_1.size(),array_2.data()+array_2.size());
};

int size(){
    return array_1.size();
}

Two_array_synchcronic operator=(Two_array_synchcronic & insert){
    this->array_1=insert.array_1;
    this->array_2=insert.array_2;
};

pair_upgraid<T1,T2>  operator[](int i){
return pair_upgraid<T1,T2>(std::pair<T1*,T2*>(array_1.data()+i,array_2.data()+i));


};


};

#endif // TWO_ARRAY_SYNCHCRONIC_H
