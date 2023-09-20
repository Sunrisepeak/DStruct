// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __STATIC_MEM_ALLOCATOR_HPP__DSTRUCT
#define __STATIC_MEM_ALLOCATOR_HPP__DSTRUCT

// only use no-dependency and static data-structures
#include <core/ds/array/Array.hpp>
#include <core/ds/linked-list/EmbeddedList.hpp>

namespace dstruct {

#define SMA_MEM_ALIGN 8
#define SMA_MEM_VERIFY(expr) __DSTRUCT_CRASH((expr) == nullptr)
#define SMA_POINTER_CMP(p1, p2) (reinterpret_cast<char *>(p1) == reinterpret_cast<char *>(p2))

template <int MEMORY_SIZE, int MAX_BLOCK_SIZE = 128>
struct StaticMemAllocator {

private:
    using __Link = _SinglyLink;

private: // big five
    StaticMemAllocator() : _mFreeMemSize { MEMORY_SIZE }, _mMemoryPool { 0 }, _mFreeMemList(__Link{ nullptr }) {
        int addrOffset = 0;
        __Link *tailPtr = &(_mFreeMemList[_TO_INDEX(MAX_BLOCK_SIZE)]);
        while (addrOffset + MAX_BLOCK_SIZE <= MEMORY_SIZE) {
            tailPtr->next = reinterpret_cast<__Link *>(_mMemoryPool + addrOffset);
            tailPtr = tailPtr->next;
            addrOffset += MAX_BLOCK_SIZE;
        }
    }

    StaticMemAllocator(const StaticMemAllocator&) = delete;
    StaticMemAllocator & operator=(const StaticMemAllocator&) = delete;

public: // mem-alloc interface
    static void * allocate(int bytes) {
        return _Instance()._allocate(bytes);
    }

    static bool deallocate(void *addr, int bytes) {
        return _Instance()._deallocate(addr, bytes);
    }

public: // mem-manager interface

    constexpr static int MEM_ALIGN_ROUND_UP(int bytes) {
        return (((bytes) + SMA_MEM_ALIGN - 1) & ~(SMA_MEM_ALIGN - 1));
    }

    static int free_mem_size() {
        return _Instance()._mFreeMemSize;
    }

    static int max_free_mblock_size() {
        int listIndex = _Instance()._mFreeMemList.size() - 1;
        while (listIndex >= 0 && _Instance()._mFreeMemList[listIndex].next == nullptr)
            listIndex--;
        return (listIndex + 1) * SMA_MEM_ALIGN;
    }

    static void memory_merge() {
        _Instance()._memory_merge();
    }

protected:
    int _mFreeMemSize;
    char _mMemoryPool[MEMORY_SIZE];
    dstruct::Array<__Link, MAX_BLOCK_SIZE / SMA_MEM_ALIGN> _mFreeMemList;

    static StaticMemAllocator & _Instance() {
        static StaticMemAllocator sma; // create & manage static memory area
        return sma;
    }

    constexpr int _TO_INDEX(int bytes) {
        return MEM_ALIGN_ROUND_UP(bytes) / SMA_MEM_ALIGN - 1;
    }

    void * _allocate(int bytes) {
        if (bytes <= 0 || bytes > MAX_BLOCK_SIZE) return nullptr;

        int freeMemListIndex = _TO_INDEX(bytes);
        while ( // search free memory block
            freeMemListIndex < _mFreeMemList.size() &&
            _mFreeMemList[freeMemListIndex].next == nullptr
        ) freeMemListIndex++;

        if (freeMemListIndex == _mFreeMemList.size()) {
            return nullptr;
        }

        // delete memory block
        __Link *memPtr = _mFreeMemList[freeMemListIndex].next;
        _mFreeMemList[freeMemListIndex].next = memPtr->next;

        // check and insert memory fragment to list
        int memFragmentListIndex = freeMemListIndex - _TO_INDEX(bytes) - 1;
        void *memFragmentAddr = reinterpret_cast<char *>(memPtr) + MEM_ALIGN_ROUND_UP(bytes);
        if (memFragmentListIndex >= 0) {
            _insert_mem_block_to_list(memFragmentAddr, memFragmentListIndex);
        }

        _mFreeMemSize -= MEM_ALIGN_ROUND_UP(bytes);

        return memPtr;
    }

    bool _deallocate(void *addr, int bytes) {
        if (bytes <= 0 || bytes > MAX_BLOCK_SIZE || addr == nullptr) return false;
        _insert_mem_block_to_list(addr, _TO_INDEX(bytes));
        _mFreeMemSize += MEM_ALIGN_ROUND_UP(bytes);
        return true;
    }

    void _memory_merge() {

        if (_mFreeMemList.back().next != nullptr) return;

        for (int i = 0; i < _mFreeMemList.size() - 1; i++) {
            auto firstMemLinkPtr = &(_mFreeMemList[i]);
            __Link *firstMemBlockPtr = firstMemLinkPtr->next;
            __Link *secondMemBlockPtr = nullptr;
            while (firstMemBlockPtr) {
                secondMemBlockPtr = firstMemBlockPtr->next;
                if (SMA_POINTER_CMP(reinterpret_cast<char *>(firstMemBlockPtr) + (i + 1) * SMA_MEM_ALIGN, secondMemBlockPtr)) {
                    // merge firstMemBlockPtr and secondMemBlockPtr
                    firstMemLinkPtr->next = secondMemBlockPtr->next; // delete first/secondMemBlockPtr from list[i]
                    int listIndex = i * 2 + 1;
                    //printf("memory_merge: [%p, %p], from list %d\n", firstMemBlockPtr, secondMemBlockPtr, i);
                    _insert_mem_block_to_list(firstMemBlockPtr, listIndex);
                } else {
                    bool secondMemBlockFlag = false;
                    for (int j = i + 1; j < _mFreeMemList.size() - 1 && !secondMemBlockFlag; j++) {
                        auto secondMemLinkPtr = &(_mFreeMemList[j]);
                        secondMemBlockPtr = secondMemLinkPtr->next;
                        while (secondMemBlockPtr != nullptr) {
                            /*                         +-----+ 
                              xUp                -->   |     |
                              firstMemBlockPtr   -->   |     |
                              xDown              -->   |     |
                                                       +-----+
                            */
                            void *addrUP = reinterpret_cast<char *>(firstMemBlockPtr) - (j + 1) * SMA_MEM_ALIGN;
                            void *addrDown = reinterpret_cast<char *>(firstMemBlockPtr) + (i + 1) * SMA_MEM_ALIGN;

                            if (SMA_POINTER_CMP(addrUP, secondMemBlockPtr) || SMA_POINTER_CMP(addrDown, secondMemBlockPtr)) { // check merge-able
                                firstMemLinkPtr->next = firstMemBlockPtr->next; // delete firstMemBlock from list[i]
                                secondMemLinkPtr->next = secondMemBlockPtr->next; // delete secondMemBlock from list[j]
                                _insert_mem_block_to_list(
                                    firstMemBlockPtr < secondMemBlockPtr ? firstMemBlockPtr : secondMemBlockPtr,
                                    i + j + 1
                                );
                                secondMemBlockFlag = true;
                                break;
                            }

                            if (firstMemBlockPtr > secondMemBlockPtr) break;

                            secondMemLinkPtr = secondMemBlockPtr;
                            secondMemBlockPtr = secondMemLinkPtr->next;
                        }
                    }

                    if (secondMemBlockFlag == false) // update firstMemLinkPtr
                        firstMemLinkPtr = firstMemBlockPtr;
                }

                firstMemBlockPtr = firstMemLinkPtr->next;
            }
        }
    }

    void _insert_mem_block_to_list(void *addr, int ListIndex) {
        if (ListIndex < _mFreeMemList.size()) {
            __Link *freeMemLinkPtr = &(_mFreeMemList[ListIndex]);
            while (freeMemLinkPtr->next != nullptr && addr > freeMemLinkPtr->next) {
                freeMemLinkPtr = freeMemLinkPtr->next;
            }
            __Link::add(freeMemLinkPtr, static_cast<__Link *>(addr));
        } else {
            // example: _mFreeMemList.size() == 16
            // when ListIndex equal 16:
            //     list1 -> index is 15
            //     list2 -> index is 0
            _insert_mem_block_to_list(addr, _mFreeMemList.size() - 1); // list1
            _insert_mem_block_to_list(reinterpret_cast<char *>(addr) + MAX_BLOCK_SIZE, ListIndex - _mFreeMemList.size() - 1); // list2
        }
    }
};

}

#endif