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
    
    virtual void SetUp() {
        for (int i = -1e3; i <= 1e3; ++i) {
            tree.insert_equal(i);
        }
    }
};

TEST_F(IntRBTreeTest, FindTest) {
    auto ite = tree.find(1);
    EXPECT_EQ(1, *ite);
    ite = tree.find(10);
    EXPECT_EQ(10, *ite);
    ite = tree.find(-100);
    EXPECT_EQ(-100, *ite);
    ite = tree.find(1000);
    EXPECT_EQ(1000, *ite);
    ite = tree.find(1001);
    EXPECT_EQ(tree.end(), ite);
    ite = tree.find(-1001);
    EXPECT_EQ(tree.end(), ite);
}

TEST_F(IntRBTreeTest, SizeTest) {
    EXPECT_EQ(2001, tree.size());
}

#endif
