#ifndef FORMULE_HPP
#define FORMULE_HPP

#include <math.h>
#include "point.hpp"

template<typename T, int N>
float distancePoint(Point<T,N> p1,Point<T,N> p2){
    T somme = 0;
    for(int i=0; i<N; i++){
        somme+=pow(p2[i]-p1[i],2);
    }
    return sqrt(somme);
}


#endif // FORMULE_HPP
