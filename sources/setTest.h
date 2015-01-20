//
//  setTest.h
//  test
//
//  Created by deng on 15/1/15.
//  Copyright (c) 2015年 deng. All rights reserved.
//

#ifndef test_setTest_h
#define test_setTest_h

#include "set.h"
//
//class SetTest : public testing::Test {
//public:
//    typedef TinySTL::set<int> IntSet;
//    IntSet iset;
////    int size;
//    
//    virtual void SetUp() {
//        int ia[] = {0,1,2,3,4};
//        IntSet tmp(ia,ia+5);
//    }
//};

TEST(SetTest, ConstructorTest) {
    int ia[] = {0,1,2,3,4};
    TinySTL::set<int> iset1(ia,ia+5);
    EXPECT_EQ(5, iset1.size());
    TinySTL::set<int> iset2({1,2,3});
    EXPECT_EQ(3, iset2.size());
    TinySTL::set<int> iset3(iset1);
    EXPECT_EQ(5, iset3.size());
}

TEST(SetTest, ClearTest) {
    int ia[] = {0,1,2,3,4};
    TinySTL::set<int> iset(ia,ia+5);
    EXPECT_EQ(5, iset.size());
    iset.clear();
    EXPECT_EQ(0, iset.size());
}

TEST(SetTest, InsertTest) {
    TinySTL::set<int> iset;
    int ia[] = {0,1,2,3,4};
    iset.insert(ia,ia+5);
    EXPECT_EQ(5, iset.size());
    auto ite = iset.begin();
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(i, *(ite++));
    }
    auto res = iset.insert(-1);
    EXPECT_EQ(-1, *(res.first));
    res = iset.insert(0);
    EXPECT_EQ(false, res.second);
}

TEST(SetTest, FindTest) {
    TinySTL::set<int> iset({0,1,2,3,4,5,6,7});
    for (int i = 0; i < 8; ++i) {
        auto ite = iset.find(i);
        EXPECT_EQ(i, *ite);
    }
}

TEST(SetTest, EraseTest) {
    TinySTL::set<int> iset({0,1,2,3,4,5,6,7});
    auto ite = iset.find(0);
    ite = iset.erase(ite);
    EXPECT_EQ(1, *ite);
    size_t n = iset.erase(1);
    EXPECT_EQ(1, n);
    n = iset.erase(1);
    EXPECT_EQ(0, n);
}

TEST(SetTest, BoundTest) {
    TinySTL::set<int> iset({1,2,3,4,5,7,8});
    auto ite = iset.lower_bound(7);
    EXPECT_EQ(7, *ite);
    ite = iset.lower_bound(6);
    EXPECT_EQ(7, *ite);
    ite = iset.upper_bound(7);
    EXPECT_EQ(8, *ite);
    ite = iset.lower_bound(2);
    EXPECT_EQ(2, *ite);
    ite = iset.upper_bound(8);
    bool flag = (iset.end()==ite);
    EXPECT_EQ(true,flag);
}

#endif
