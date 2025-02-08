//#ifndef LSD
//#define LSD

//inline int digit(int n, int k, int N, int M) noexcept {
//        return (n >> (N * k) & (M - 1));
//}
//void _radixsort(int* l, int* r, int N) {
//        int k = (32 + N - 1) / N;
//        int M = 1 << N;
//        int sz = r - l;
//        int* b = new int[sz];
//        int* c = new int[M];
//        for (int i = 0; i < k; i++) {
//                for (int j = 0; j < M; j++)
//                        c[j] = 0;
//                for (int* j = l; j < r; j++)
//                        c[digit(*j, i, N, M)]++;
//                for (int j = 1; j < M; j++)
//                        c[j] += c[j - 1];
//                for (int* j = r - 1; j >= l; j--)
//                        b[--c[digit(*j, i, N, M)]] = *j;
//                int cur = 0;
//                for (int* j = l; j < r; j++)
//                        *j = b[cur++];
//        }
//        delete[] b;
//        delete[] c;
//}
////void radixsort(int* l, int* r) {
////        _radixsort(l, r, 8);
////}
//#endif
