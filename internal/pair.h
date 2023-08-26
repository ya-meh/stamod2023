//
// Created by Mikhail Kiselev on 26.08.2023.
//

#ifndef STATMOD_PAIR_H
#define STATMOD_PAIR_H

#include <sstream>

template<typename T, typename U>
class Pair {
public:
    T first;
    U second;

    template<typename T_=T, typename U_=U>
    bool operator<(const Pair<T_, U_> &other) {
        return first < other.first || first == other.first && second < other.second;
    }

    template<typename T_=T, typename U_=U>
    bool operator==(const Pair<T_, U_> &other) {
        return first == other.first && second == other.second;
    }

    [[nodiscard]] Pair<T, U> operator+(const Pair<T, U> &other) const {
        return {first + other.first, second + other.second};
    }
};

template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const Pair<T, U> &p) {
    return os << '(' << p.first << ", " << p.second << ')';
}

#endif //STATMOD_PAIR_H
