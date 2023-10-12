// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __EMBEDDED_BINARY_TREE_HPP__DSTRUCT
#define __EMBEDDED_BINARY_TREE_HPP__DSTRUCT

#include <core/ds/tree/tree-base.hpp>

namespace dstruct {

namespace tree {
// Binary Tree
/*
   Parent
    ^
    |
  TreeNode
   / \
  L   R
*/
struct _BinaryTreeLink {
    _BinaryTreeLink *parent;
    _BinaryTreeLink *left;
    _BinaryTreeLink *right;

    _BinaryTreeLink() : parent { nullptr }, left { nullptr }, right { nullptr } {}
};

static _BinaryTreeLink * left_rotate(_BinaryTreeLink *root) {
    _BinaryTreeLink *newRoot = root->right;
    newRoot->parent = root->parent;

    // update subTree
    root->right = newRoot->left;
    if (nullptr != newRoot->left) {
        newRoot->left->parent = root;
    }

    root->parent = newRoot;
    newRoot->left = root;
    return newRoot;
}

static _BinaryTreeLink * right_rotate(_BinaryTreeLink *root) {
    _BinaryTreeLink *newRoot = root->left;
    newRoot->parent = root->parent;

    // update subTree
    root->left = newRoot->right;
    if (nullptr != newRoot->right) {
        newRoot->right->parent = root;
    }

    root->parent = newRoot;
    newRoot->right = root;
    return newRoot;
}

static int height(_BinaryTreeLink *root) {
    int h = 0;
    if (root) {
        int lH, rH;
        lH = height(root->left);
        rH = height(root->right);
        h = (lH > rH ? lH : rH) + 1;
    }
    return h;
}

static _BinaryTreeLink * next_preorder(_BinaryTreeLink *link) {
    if (link == nullptr) {
        return nullptr;
    }

    // check and try to return left leaf
    if (link->left != nullptr) {
        return link->left;
    }

    // check and try to return right leaf
    if (link->right != nullptr) {
        return link->right;
    }

    while (link->parent != nullptr) {
        if (link->parent->right != nullptr && link->parent->right != link) {
            return link->parent->right;
        }
        link = link->parent;
    }
    return nullptr;
}

template <typename Callback>
static void preorder_traversal(_BinaryTreeLink *root, Callback &cb) {
    if (root) {
        cb(root);
        preorder_traversal(root->left, cb);
        preorder_traversal(root->right, cb);
    }
}

static _BinaryTreeLink * next_inorder(_BinaryTreeLink *link) {
    if (link == nullptr) {
        return nullptr;
    }

    if (link->right != nullptr) {
        link = link->right;
        while (link->left != nullptr) {
            link = link->left;
        }
        return link;
    }

    while (link->parent != nullptr && link->parent->left != link) {
        link = link->parent;
    }

    return link->parent;
}

template <typename Callback>
static void inorder_traversal(_BinaryTreeLink *root, Callback &cb) {
    if (root) {
        inorder_traversal(root->left, cb);
        cb(root);
        inorder_traversal(root->right, cb);
    }
}

static _BinaryTreeLink * next_postorder(_BinaryTreeLink *link) {
    if (link == nullptr) {
        return nullptr;
    }

    if (link->right != nullptr) {
        link = link->right;
        while (link->left != nullptr || link->right != nullptr) {
            if (link->left != nullptr) {
                link = link->left;
            } else {
                link = link->right;
            }
        }
        return link;
    }

    // link->parent != nullptr : root check
    while (link->parent != nullptr && link->parent->right == link) {
        link = link->parent;
    }
    return link->parent;
}

template <typename Callback>
static void postorder_traversal(_BinaryTreeLink *root, Callback cb) {
    if (root) {
        postorder_traversal(root->left, cb);
        postorder_traversal(root->right, cb);
        cb(root);
    }
}

template <typename T>
using EmbeddedBinaryTreeNode = TreeNode<T, _BinaryTreeLink>;

}
}

#endif