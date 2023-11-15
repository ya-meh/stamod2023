#ifndef STATMOD_RANDOM_H
#define STATMOD_RANDOM_H


#include "vector.h"

#include <random>

const size_t SEED = 0;

template<typename Float=double>
Float rnd(std::mt19937 &mt) {
    static std::uniform_real_distribution<Float> dis;
    return dis(mt);
}

template<typename Float=double>
Float rnd_static() {
//    random_device rd;
    static std::mt19937 mt(SEED);
    static std::uniform_real_distribution<Float> dis;
    return dis(mt);
}

#endif //STATMOD_RANDOM_H
