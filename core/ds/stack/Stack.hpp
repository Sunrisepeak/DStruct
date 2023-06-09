#ifndef __STACK_HPP__DSTRUCT
#define __STACK_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/array/Vector.hpp>

namespace dstruct {

namespace adapter {

// DS: need support pop_back / push_back / back
template<typename T, typename DS>
class Stack : public _DStructTypeSpec<T, typename DS::ConstIteratorType, typename DS::ConstIteratorType> {

    DSTRUCT_TYPE_SPEC_DECLARE(DS)

public:
    Stack() = default;

public: // base op
    // status
    SizeType size() const { return _mDS.size(); }
    //SizeType capacity() const { return _mDS.capacity(); }
    bool empty() const { return _mDS.empty(); }

    // check
    ValueType top /* back */ () const { return _mDS.back(); }

    // push/pop
    void push(const T &obj) { _mDS.push_back(obj); }
    void pop() {
        T data = _mDS.back();
        _mDS.pop_back();
    }

    // Iterator/range-for
    ConstIteratorType begin() const { return _mDS.begin(); }
    ConstIteratorType end() const { return _mDS.end(); }

protected:
    DS _mDS;
};

} // namespace adapter

} // 

#endif