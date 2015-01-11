//
//  RBTreeTest.h
//  test
//
//  Created by deng on 14/12/27.
//  Copyright (c) 2014年 deng. All rights reserved.
//

#ifndef test_RBTreeTest_h
#define test_RBTreeTest_h

#include "rb_tree.h"
#include <string>
#include <initializer_list>
#include "algorithms.h"
#include "functional.h"
#include <gtest/gtest.h>

typedef TinySTL::__rb_tree_node<int> node;
typedef TinySTL::__rb_tree_iterator<int> iterator;

class IntRBTreeTest : public testing::Test {
public:
    typedef TinySTL::rb_tree<int, int, TinySTL::identity, TinySTL::less<int> > IntTree;
    IntTree tree;
    int size;
    
    virtual void SetUp() {
        size = 1e3;
        for (int i = -1*size; i <= size; ++i) {
            tree.insert_equal(i);
        }
    }
};

TEST_F(IntRBTreeTest, FindTest) {
    for (int i = -1*size; i <= size; ++i) {
        auto ite = tree.find(i);
        EXPECT_EQ(i, *ite);
    }
}

TEST_F(IntRBTreeTest, SizeTest) {
    EXPECT_EQ(2*size+1, tree.size());
}

TEST_F(IntRBTreeTest, DeleteTest) {
    for (int i = -1*size; i <= size; ++i) {
        tree.deleteNode(i);
        auto ite = tree.find(i);
        EXPECT_EQ(tree.end(), ite);
    }
}

#endif
