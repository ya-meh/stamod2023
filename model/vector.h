#ifndef STATMOD_VECTOR_H
#define STATMOD_VECTOR_H


#include "random.h"

#include <cstddef>
#include <algorithm>
#include <random>
#include <sstream>
#include <iostream>


using std::size_t;


template<typename T>
class Vector {
    size_t size_{}, capacity_{};
    T *data_ = nullptr;

public :
    Vector() =default;

    explicit Vector(size_t size) : size_(size), capacity_(size), data_(new T[size]) {}

    template<typename U=T>
    explicit Vector(const Vector<U> &other) : size_(other.size_), capacity_(other.capacity_), data_(new T[capacity_]) {
        std::copy(other.data_, other.data_ + capacity_, data_);
    }

    template<typename U=T>
    explicit Vector(Vector<U> &&other) : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    const T &operator[](size_t i) const {
        return data_[i];
    }

    T &operator[](size_t i) {
        return data_[i];
    }

    template<typename U=T>
    [[nodiscard]] Vector<T> operator*(U value) const {
        auto multiplied = Vector<T>(size());
        for (size_t i = 0; i < size(); ++i) {
            multiplied[i] = (*this)[i] * value;
        }
        return multiplied;
    }

    template<typename U=T>
    Vector &push_back(U val) {
        if (size_ == capacity_) {
            if (data_ == nullptr) {
                data_ = new T[64];
                size_ = 0;
                capacity_ = 64;
            } else {
                auto old_data = data_;
                capacity_ *= 2;
                data_ = new T[capacity_ * 2];
                std::copy(old_data, old_data + capacity_, data_);
                delete[]old_data;
            }
        }

        data_[size_] = std::move(val);
        size_ += 1;

        return *this;
    }

    [[nodiscard]] std::string to_string() const {
        std::ostringstream oss;
        oss << '[';
        for (size_t i = 0; i < size(); ++i) {
            if (i != 0) {
                oss << ", ";
            }
            oss << (*this)[i];
        }
        oss << ']';
        return oss.str();
    }

    [[nodiscard]] size_t size() const {
        return size_;
    }

    [[nodiscard]] size_t capacity() const {
        return capacity_;
    }

    [[nodiscard]] bool empty() const {
        return size_ == 0;
    }

    [[nodiscard]] T &back() {
        return data_[size() - 1];
    }

    T *begin() const {
        return data_;
    }

    T *end() const {
        return data_ + size_;
    }

    template<typename U=T>
    Vector<U> deviate(T delta) const {
        auto vec = Vector<U>(*this);
        for (auto &el: vec) {
            el += abs(2 * (rnd() - 0.5) * delta);
        }
        return vec;
    }

    T sum() const {
        T ret = 0;
        for (const auto &el: *this)
            ret += el;
        return ret;
    }

    ~Vector() {
        delete[]data_;
    }

    template<typename Integer>
    static Vector<Integer> generate(size_t size, Integer limit) {
        static std::mt19937 mt(SEED);
        auto vec = Vector<T>(size);
        for (auto &el: vec)
            el = mt() % limit;
        return vec;
    }

    template<typename Float=double>
    static Vector<Float> generate_normal(size_t size) {
        static std::mt19937 mt(SEED);
        std::uniform_real_distribution<Float> dis(0, 1);
        auto vec = Vector<Float>(size);
        for (auto &el: vec)
            el = dis(mt);
        auto sum = std::accumulate(vec.begin(), vec.end(), static_cast<Float>(0.0));
        for (auto &el: vec)
            el /= sum;
        return vec;
    }
};


template<typename T>
std::ostream &operator<<(std::ostream &os, const Vector<T> &vec) {
    return os << vec.to_string();
}


#endif //STATMOD_VECTOR_H
