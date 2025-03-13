#ifndef HEAP_H
#define HEAP_H
#include <bits/stdc++.h>
#include <QDebug>
using namespace std;

struct Data{
    long long f;
    string s;

    Data()
        :f{0}, s{""}
    { }
    Data(long long a, string str)
        :f{a}, s{str}
    { }
};

struct pData{
    Data* a;
};

class heap{//sort by frequency
    long long N;
public:
    Data* begin, *end;
    pData* pa;

    heap(Data* a, Data* b);
    ~heap();
//    void swim(int k){
//        while(k > 1 && a[k/2] < a[k]){
//            swap(a[k/2], a[k]);
//            k = k/2;
//        }
//    }
    void sink(int k);
    void sink(int k, int n);


//    void insert(int x){
//        a[++N] = x;
//        swim(N);
//    }
    void heapConstruct();
    void acHeapSort(int k);
};

class HeapSort{//sort by string
public:
    Data* a;
    int N;

    HeapSort(Data* p, int v);
    ~HeapSort(){ };

    void sink(int k);
    void sink(int k, int n);

    void heapConstruct();
    void acHeapSort();
};

#endif // HEAP_H
