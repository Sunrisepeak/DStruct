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

    {
        dstruct::BSTree<int> bst;

        for (int i = 0; i < 10; i++) {
            DSTRUCT_ASSERT(2 * i == bst.size());
            bst.push(i);
            bst.push(100 - i);
        }

        for (int i = 0; i < 10; i++) {
            //bst.pop(i);
            bst.pop(100 - i);
        }

        auto it = bst.begin();
        for (int i = 0; it != bst.end(); i++) {
            //std::cout << i << ": " << *it << std::endl;
            DSTRUCT_ASSERT(i == *it);
            it = bst.erase(it);
        }

        DSTRUCT_ASSERT(bst.empty());

    }

    {
        int arr[5] { 3, 4, 2, 5, 1 };
        dstruct::BSTree<int> bst(arr, arr + sizeof(arr) / sizeof(int));
        DSTRUCT_ASSERT(bst.size() == sizeof(arr) / sizeof(int));

        for (auto v : bst) {
            //std::cout << v << std::endl;
        }

        int sum { 0 };
        bst.traversal(
            [&](int val) {
                sum += val;
            },
            decltype(bst)::TraversalType::PreOrder
        );
        DSTRUCT_ASSERT(sum == 1 + 2 + 3 + 4 + 5);
    }

    std::cout << "   pass" << std::endl;

    return 0;
}