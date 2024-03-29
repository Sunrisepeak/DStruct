// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __ARRAY_HPP__DSTRUCT
#define __ARRAY_HPP__DSTRUCT

#include <spec/DStructSpec.hpp>
#include <core/Iterator.hpp>
#include <core/utils.hpp>

namespace dstruct {

template <typename T, size_t N>
class Array : public _DStructTypeSpec<T, dstruct::Alloc /*unused*/ , PrimitiveIterator> {

public: // big Five

    Array() = default;

    Array(typename Array::ConstReferenceType element) {
        for (int i = 0; i < N; i++) {
            _mC[i] = element;
        }
    }

    Array(const Array &arr) {  *this = arr; }
    Array & operator=(const Array &arr) {
        for (int i = 0; i < N; i++) {
            _mC[i] = arr._mC[i];
        }
        return *this;
    }

    Array(Array &&arr) { *this = dstruct::move(arr); };
    Array & operator=(Array &&arr) {
        for (int i = 0; i < N; i++) _mC[i] = dstruct::move(arr._mC[i]);
        return *this;
    }

    ~Array() = default; // array: auto-destroy for every element

public: // Capacity
    bool empty() const {
        return N == 0;
    }

    typename Array::SizeType size() const {
        return N;
    }

    typename Array::SizeType capacity() const {
        return N;
    }

public: // Access
    typename Array::ConstReferenceType back() const {
        return _mC[N - 1];
    }

    typename Array::ConstReferenceType front() const {
        return _mC[0];
    }

    typename Array::ConstReferenceType operator[](int index) const {
        if (index < 0)
            index = N + index;
        return _mC[index];
    }

public: // Modifiers
    typename Array::ReferenceType operator[](int index) {
        if (index < 0)
            index = N + index;
        return _mC[index];
    }

public: // iterator
    typename Array::IteratorType begin() {
        return _mC;
    }

    typename Array::ConstIteratorType begin() const {
        return _mC;
    }

    typename Array::IteratorType end() {
        return _mC + N;
    }

    typename Array::ConstIteratorType end() const {
        return _mC + N;
    }

protected:
    T _mC[N == 0 ? 1 : N];
}; // Array

};



#endif /* __ARRAY_HPP__TSL */