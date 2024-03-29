// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __LIBC_DSTRUCT_PORT_HPP__DSTRUCT
#define __LIBC_DSTRUCT_PORT_HPP__DSTRUCT

#include <cstdlib>
#include <cassert>

#include <port/dstruct-port.h>

#define __DSTRUCT_PORT_ASSERT(expr) assert(expr)

namespace dstruct {
namespace port {

using size_t = ::size_t;

static void * allocate(int bytes) {
    return malloc(bytes);
}

static void deallocate(void *addr, int bytes) {
    free(addr);
}

}

}

#endif