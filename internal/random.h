#ifndef STATMOD_RANDOM_H
#define STATMOD_RANDOM_H


#include "vector.h"

#include <random>

const size_t SEED = 0;

template<typename Float=double>
Float rnd() {
//    random_device rd;
    static std::mt19937 mt(0);
    static std::uniform_real_distribution<Float> dis;
    return dis(mt);
}

#endif //STATMOD_RANDOM_H
