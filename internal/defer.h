#ifndef STATMOD_DEFER_H
#define STATMOD_DEFER_H


// GoLang-like defer, but instead of calling the func at the end of the master function,
// it calls func, at the end of the lifetime of defer()
#define defer(func) \
    volatile auto __random_name() = DeferWrapper(func)


template<typename T>
class DeferWrapper {
    T func;
public:
    explicit DeferWrapper(T func) : func(func) {}

    ~DeferWrapper() { func(); }
};

#define __concat_inner(a, b) a ## b
#define __concat(a, b) __concat_inner(a, b)
#define __random_name() __concat(defer_, __COUNTER__)

#endif //STATMOD_DEFER_H
