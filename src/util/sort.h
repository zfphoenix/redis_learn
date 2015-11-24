#ifndef __SORT_H_
#define __SORT_H_

#include <stack>
#include <utility>


typedef std::stack< std::pair<int, int> > PairStack;

template< class T, class Comparator >
void quick_sort(T arr[], int low, int high) {
    if (low >= high) {
        return ;
    }
    T key = arr[low];
    int begin = low;
    int end = high;
    while (begin < end) {
        while (begin < end && Comparator()(arr[end], key)) {
            end--;
        }
        arr[begin] = arr[end];
        while (begin < end && Comparator()(key, arr[begin])) {
            begin++;
        }
        arr[end] = arr[begin];
    }
    arr[begin] = key;
    quick_sort<T, Comparator>(arr, low, begin - 1);
    quick_sort<T, Comparator>(arr, begin + 1, high);
}

template< class T, class Comparator >
void quick_sort_no_recursive(T arr[], int low, int high) {
    if (low >= high) {
        return ;
    } 
    PairStack pstack;
    pstack.push(std::make_pair(low, high));
    while (!pstack.empty()) {
        std::pair<int, int> p = pstack.top();
        pstack.pop();
        int begin = p.first;
        int end   = p.second;
        if (begin >= end)
            continue;
        int b = begin, e = end;
        T key = arr[begin];
        while (begin < end) {
            while (begin < end && Comparator()(arr[end], key)) {
                end--;
            }
            arr[begin] = arr[end];
            while (begin < end && Comparator()(key, arr[begin])) {
                begin++;
            }
            arr[end] = arr[begin];
        }
        arr[begin] = key; 
        pstack.push(std::make_pair(b, begin - 1));
        pstack.push(std::make_pair(begin + 1, e));
    }
}

#endif
