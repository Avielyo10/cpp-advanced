#pragma once

#include <bits/stdc++.h>

using namespace std;

template<typename It> It
Add_n(It begin, It end, uint n) {
    It it = begin;
    while (it < end) {
        if (it + n > end) n = end - it;
        *begin = accumulate(it, it + n, 0.0);
        it += n;
        begin++;
    }
    return begin;
}

template<typename It> It
Transpose(It begin, It end) { 
    It it = begin;
    while (begin < end) {
        if (begin + 1 == end) break;
        iter_swap(begin, begin + 1);
        begin+=2;
    }
    return begin;      
}

template<typename It, typename oIt,typename Function> oIt
Transform(It begin, It end, oIt oBegin, Function func) {
    if (begin == end || distance(begin, end) % 2 == 1) return oBegin;
    while (begin < end) {
        *oBegin = func(*begin, *(begin + 1));
        oBegin++;
        begin += 2;
    }
    return oBegin;
}
