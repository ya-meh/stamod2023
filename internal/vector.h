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
T *New(size_t size = 1) {
    if (size == 0)
        return nullptr;

    auto ptr = new T[size];
    while (size--)
        ptr[size] = T{};

    return ptr;
}

template<typename T>
class Vector {
    size_t size_{}, capacity_{};
    T *data_ = nullptr;

public :
    Vector() = default;

    explicit Vector(size_t size) : size_(size), capacity_(size), data_(New<T>(capacity_)) {}

    Vector(std::initializer_list<T> init) : size_(init.size()), capacity_(init.size()), data_(New<T>(capacity_)) {
        size_t i = 0;
        for (const auto &el: init)
            (*this)[i++] = el;
    }

    Vector(const Vector<T> &other) : size_(other.size_), capacity_(other.capacity_), data_(New<T>(capacity_)) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    template<typename U=T>
    explicit Vector(Vector<U> &&other) : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    Vector<T> &operator=(const Vector<T> &other) {
        if (data_ == other.data_) {
            return *this;
        }

        delete[]data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = New<T>(capacity_);

        std::copy(other.data_, other.data_ + size_, data_);

        return *this;
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
    [[nodiscard]] Vector<T> operator/(U value) const {
        auto multiplied = Vector<T>(size());
        for (size_t i = 0; i < size(); ++i) {
            multiplied[i] = (*this)[i] / value;
        }
        return multiplied;
    }

    template<typename U=T>
    Vector<T> &push_back(U val) {
        if (size_ >= capacity_) {
            if (data_ == nullptr) {
                data_ = New<T>(64);
                size_ = 0;
                capacity_ = 64;
            } else {
                auto old_data = data_;
                capacity_ *= 2;
                data_ = New<T>(capacity_ * 2);
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
        oss << join(", ");
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
    Vector<U> deviate(U delta) const {
        auto vec = Vector<U>(*this);
        for (auto &el: vec) {
            el += abs(2 * (rnd() - 0.5) * delta);
        }
        return vec;
    }

    template<typename Float=double>
    [[nodiscard]] Vector<double> normalize() const {
        auto normal = Vector<Float>(size());
        auto s = sum();

        for (size_t i = 0; i < size(); ++i)
            normal[i] = static_cast<double>((*this)[i]) / s;

        return normal;
    }

    Vector<size_t> count_buckets(size_t count, T from = 0, T to = 1) {
        auto step = (to - from) / count;
        auto buckets = Vector<size_t>(count);
        for (size_t i = 1; i <= count; ++i) {
            double threshold = step * i;
            buckets[i - 1] = static_cast<double>(
                    std::count_if(begin(), end(), [threshold](auto val) { return val <= threshold; })
            );
        }
        return buckets;
    }

    [[nodiscard]] T sum() const {
        T ret = 0;
        for (const auto &el: *this)
            ret += el;
        return ret;
    }

    [[nodiscard]] Vector<T> sorted() const {
        auto cpy = Vector<T>(*this);

        std::sort(cpy.begin(), cpy.end());

        return cpy;
    }

    [[nodiscard]] std::string join(std::string_view sep = " ") const {
        auto ss = std::stringstream{};

        for (size_t i = 0; i < size(); ++i) {
            if (i != 0)
                ss << sep;

            ss << (*this)[i];
        }

        return ss.str();
    }


    template<typename Target=double>
    [[nodiscard]]  Vector<Target> convert() const {
        auto target = Vector<Target>{};

        for (auto el: *this)
            target.push_back(static_cast<Target>(el));

        return target;
    }

    virtual ~Vector() {
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
