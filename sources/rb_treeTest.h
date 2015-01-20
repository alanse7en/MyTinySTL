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
    typedef TinySTL::rb_tree<int, int, TinySTL::identity<int>, TinySTL::less<int> > IntTree;
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
    const IntTree tmp(tree);
    for (int i = -1*size; i <= size; ++i) {
        auto ite = tmp.find(i);
        EXPECT_EQ(i, *ite);
    }
}

TEST_F(IntRBTreeTest, SizeTest) {
    EXPECT_EQ(2*size+1, tree.size());
}

TEST_F(IntRBTreeTest, InsertEqualTest) {
    auto ite = tree.insert_equal(1e4);
    EXPECT_EQ(1e4, *ite);
    tree.deleteNode(1);
    ite = tree.insert_equal(1);
    EXPECT_EQ(1, *ite);
}

TEST_F(IntRBTreeTest, InsertUniqueTest) {
    auto ite = tree.insert_unique(1e4);
    EXPECT_EQ(1e4, *(ite.first));
    EXPECT_EQ(true, ite.second);
    ite = tree.insert_unique(1);
    EXPECT_EQ(1, *(ite.first));
    EXPECT_EQ(false,ite.second);
}

TEST_F(IntRBTreeTest, DeleteTest) {
    int n = 1;
    for (int i = -1*size; i < size; ++i) {
        auto res = tree.deleteNode(i);
//        auto ite = tree.find(i);
        EXPECT_EQ(i+1, *(res.first));
        EXPECT_EQ(2*size+1-n, tree.size());
        ++n;
    }
    
    auto ite = tree.deleteNode(1e4);
    EXPECT_EQ(false, ite.second);
}

TEST_F(IntRBTreeTest, AssignOperatorTest) {
    IntTree tmp;
    tmp = tree;
    tmp.insert_unique(size + 1);
    EXPECT_EQ(2*size+1, tree.size());
    EXPECT_EQ(2*size+2, tmp.size());
}

TEST_F(IntRBTreeTest, MoveAssignOperatorTest) {
    IntTree tmp;
    tmp = std::move(tree);
    tmp.insert_unique(size + 1);
    EXPECT_EQ(0, tree.size());
    EXPECT_EQ(2*size+2, tmp.size());
}

TEST_F(IntRBTreeTest, ListAssignOperatorTest) {
    EXPECT_EQ(2*size+1, tree.size());
    std::initializer_list<int> ilist = {1,2,3};
    tree = ilist;
    EXPECT_EQ(ilist.size(), tree.size());
}

TEST(TreeTest, SpeCase) {
    TinySTL::rb_tree<int, int, TinySTL::identity<int>, TinySTL::less<int> > tmp;
    int ia[] = {1,2,3,4,5,7,8};
    tmp.insert_unique(ia, ia+7);
    EXPECT_EQ(7, tmp.size());
}

#endif
