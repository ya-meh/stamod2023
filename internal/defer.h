#ifndef STATMOD_DEFER_H
#define STATMOD_DEFER_H


// GoLang-like defer
template<typename T>
class Defer {
    T func;
public:
    explicit Defer(T func) : func(func) {}

    ~Defer() { func(); }
};

#define __concat_inner(a, b) a ## b
#define __concat(a, b) __concat_inner(a, b)
#define __random_name() __concat(defer_, __COUNTER__)

#define defer(func) \
    volatile auto __random_name = Defer(func)

#endif //STATMOD_DEFER_H
