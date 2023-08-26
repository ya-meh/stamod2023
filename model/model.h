#ifndef STATMOD_MODEL_H
#define STATMOD_MODEL_H


#include "../internal/vector.h"

#include <cstddef>
#include <algorithm>
#include <numeric>


// Model interface
class Model {
public:
    virtual void first(int n, int p_values_n, int k) = 0;

    virtual void second(int n, int p_values_n, int k) = 0;
};




#endif //STATMOD_MODEL_H
