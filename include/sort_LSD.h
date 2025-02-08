#ifndef SORT_LSD_H
#define SORT_LSD_H

//inline int digit(int n, int k, int N, int M) noexcept {
//        return (n >> (N * k) & (M - 1));
//}
//template<class T>
//void _radixsort(int* l, int* r,T * l_d,T* r_d,int N) {
//    int k = (32 + N - 1) / N;
//    int M = 1 << N;
//    int sz = r - l;
//    int* b = new int[sz];
//    T* d= new T[sz];
//    int* c = new int[M];

//    for (int i = 0; i < k; i++) {
//        for (int j = 0; j < M; j++)
//            c[j] = 0;
//        for (int* j = l; j < r; j++)
//            c[digit(*j, i, N, M)]++;
//        for (int j = 1; j < M; j++)
//            c[j] += c[j - 1];
//        T * u=r_d-1;
//        for (int* j = r - 1; j >= l; j--,u--){
//            int f=--c[digit(*j, i, N, M)];
//            b[f] = *j;
//            d[f] = *u;
//            //std::cout<<*j<<"|#|"<<std::endl;
//        }
//        int cur = 0;
//        u=l_d;
//        for (int* j = l; j < r; j++,u++){
//            *j = b[cur];
//            *u = d[cur];
//            cur++;
//        }
//    }
//    delete[] b;
//    delete[] d;
//    delete[] c;
//}



#endif // SORT_LSD_H
