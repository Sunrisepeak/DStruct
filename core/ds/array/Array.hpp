#ifndef __ARRAY_HPP__DSTRUCT
#define __ARRAY_HPP__DSTRUCT

#include <core/common.hpp>

namespace dstruct {

template <typename T, size_t N>
class Array : public DStructTypeSpec<T>{

public: // big Five

    Array() = default;

    Array(const T &value) : Array() {
        for (size_t i = 0; i < N; i++) {
            _mC[i] = value;
        }
    }

    Array(const Array &arr) {  *this = arr; }
    Array & operator=(const Array &arr) {
        for (size_t i = 0; i < N; i++) {
            _mC[i] = arr._mC[i];
        }
        return *this;
    }

    Array(Array &&arr) { *this = dstruct::move(arr); };
    Array & operator=(Array &&arr) {
        for (size_t i = 0; i < N; i++) _mC[i] = dstruct::move(arr._mC[i]);
        return *this;
    }

    ~Array() = default; // array: auto-destroy for every element

public: // base op
    size_t empty() const { return N == 0; }
    size_t size() const { return N; }
    T back() const { return _mC[N - 1]; }
    T front() const { return _mC[0]; }

public: // operator

    T & operator[](int index) {
        if (index < 0)
            index = N + index;
        DSTRUCT_ASSERT(index < static_cast<int>(N));
        return _mC[index];
    }

    T operator[](int index) const {
        if (index < 0)
            index = N + index;
        DSTRUCT_ASSERT(index < static_cast<int>(N));
        return _mC[index];
    }

public: // iterator
    typename Array::IteratorType begin() { return _mC; }
    typename Array::ConstIteratorType begin() const { return _mC; }
    typename Array::ConstIteratorType cbegin() const { return _mC; }
    typename Array::IteratorType end() { return _mC + N; }
    typename Array::ConstIteratorType end() const { return _mC + N; }
    typename Array::ConstIteratorType cend() const { return _mC + N; }

protected:
    T _mC[N == 0 ? 1 : N];
}; // Array

};



#endif /* __ARRAY_HPP__TSL */