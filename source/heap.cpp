#include "heap.h"

heap::heap(Data *a, Data *b)
    :N{b-a+1}, begin{a}, end{b}
{
    pa = new pData[N+1];
    for(int i = 1; i <= N; i++){
        pa[i].a = begin +i -1;
    }
}

heap::~heap(){
    delete[] pa;
}

void heap::sink(int k){
    while(2*k <= N){
        int j = 2*k;
        if(j < N && pa[j].a->f < pa[j+1].a->f) j++;
        if(pa[k].a->f >= pa[j].a->f) break;
        swap(pa[k], pa[j]);
        k = j;
    }
}

void heap::sink(int k, int n){
    while(2*k <= n){
        int j = 2*k;
        if(j < n && pa[j].a->f < pa[j+1].a->f) j++;
        if(pa[k].a->f >= pa[j].a->f) break;
        swap(pa[k], pa[j]);
        k = j;
    }
}

void heap::heapConstruct(){
    for(int k = N/2; k >= 1; k--){
        sink(k);
    }
}

void heap::acHeapSort(int k){
    int n = N;
    heapConstruct();
    while(n > N - k - 1 && n > 1){
        swap(pa[1], pa[n--]);
        sink(1, n);
    }
}

HeapSort::HeapSort(Data *p, int v)
    :a{p}, N{v}
{ }

void HeapSort::sink(int k){
    while(2*k <= N){
        int j = 2*k;
        if(j < N && a[j].s < a[j+1].s) j++;
        if(a[k].s >= a[j].s) break;
        swap(a[k], a[j]);
        k = j;
    }
}

void HeapSort::sink(int k, int n){
    while(2*k <= n){
        int j = 2*k;
        if(j < n && a[j].s < a[j+1].s) j++;
        if(a[k].s >= a[j].s) break;
        swap(a[k], a[j]);
        k = j;
    }
}

void HeapSort::heapConstruct(){
    for(int k = N/2; k >= 1; k--){
        sink(k);
    }
}

void HeapSort::acHeapSort(){
    int n = N;
    heapConstruct();
    while(n > 1){
        swap(a[1], a[n--]);
        sink(1, n);
    }
}
