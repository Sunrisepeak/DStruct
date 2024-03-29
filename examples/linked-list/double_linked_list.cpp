// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#include <iostream>

#include <dstruct.hpp>

int main() {

    std::cout << "\nTesting: " << __FILE__;

    dstruct::DoublyLinkedList<int> list(10, 2);
    dstruct::DLinkedList<double> list2(2, 1.1);

    DSTRUCT_ASSERT(list2.size() == 2);

    for (auto &v : list) { v = 3; }
    for (auto v: list) { /* const iterator */ }

    DSTRUCT_ASSERT(list.back() == 3);

    while (!list.empty()) {
        //std::cout << list.back() << " " << list.size() << std::endl;
        list.pop_back();
    }

    DSTRUCT_ASSERT(list.size() == 0);

    for (int i = 0; i < 10; i++) {
        list.push_back(i);
        DSTRUCT_ASSERT(list.back() == i);
        //std::cout << list.back() << " " << list.size() << std::endl;
    }

    DSTRUCT_ASSERT(list.size() == 10);

    list.clear();
    list.push_back(0);
    DSTRUCT_ASSERT(list.size() == 1);

    {   // test move
        dstruct::DoublyLinkedList<int> list(10, 3);
        
        DSTRUCT_ASSERT(list.size() == 10);

        decltype(list) tmpList = dstruct::move(list);
        
        int cnt = 0;
        for (auto v : tmpList) { cnt++; /* const iterator */ if (cnt == 11) break; }
        DSTRUCT_ASSERT(tmpList.size() == cnt);

        DSTRUCT_ASSERT(list.empty());

        // test copy
        list = tmpList;
        auto it1 = list.begin();
        auto it2 = tmpList.begin();
        for (int i = 0; i < 10; i++, it1++, it2++) {
            DSTRUCT_ASSERT(*it1 == *it2);
            DSTRUCT_ASSERT(*it1 == 3);
            DSTRUCT_ASSERT(it1.operator->() != it2.operator->());
        }
    }

    std::cout << "   pass" << std::endl;

    return 0;
}