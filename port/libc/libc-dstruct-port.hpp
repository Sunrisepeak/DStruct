#ifndef __LIBC_DSTRUCT_PORT_HPP__DSTRUCT
#define __LIBC_DSTRUCT_PORT_HPP__DSTRUCT

#include <cstdlib>
#include <cassert>

#include <port/dstruct-port.h>

#define __DSTRUCT_PORT_ASSERT(expr) assert(expr)

namespace dstruct {

namespace port {
struct Alloc {
    static void *allocate(size_t bytes) {
        return malloc(bytes);
    }

    static void deallocate(void *addr, size_t bytes) {
        free(addr);
    }
};

}

}

#endif