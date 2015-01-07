//
//  vectorTest.h
//  test
//
//  Created by deng on 14/12/23.
//  Copyright (c) 2014年 deng. All rights reserved.
//

#ifndef test_vectorTest_h
#define test_vectorTest_h

#include <string>
#include <initializer_list>
#include "algorithms.h"
#include "vector.h"
#include <gtest/gtest.h>

class IntVectorTest : public testing::Test {
public:
    typedef TinySTL::vector<int> IntVector;
    IntVector v0;
    IntVector v1;
    IntVector v2;
    IntVector v3;
    IntVector v4;
    
    virtual void SetUp() {
        v0 = IntVector();
        v1 = IntVector({1});
        v2 = IntVector(10,7);
        v3 = IntVector(1e4,7);
    }
};


TEST_F(IntVectorTest, Constructors) {
    EXPECT_EQ(NULL, v0.data()) << "v0 should be empty";
    EXPECT_EQ(0, v0.size()) << "The size of v0 should be 0 instead of " << v0.size();
    EXPECT_EQ(1, *v1.data()) << "The first element of v1 should be 1";
    EXPECT_EQ(1, v1.size()) << "The size of v1 should be 1 instead of " << v1.size();
    EXPECT_EQ(7, *v2.data()) << "The first element of v1 should be 1";
    EXPECT_EQ(10, v2.size()) << "The size of v1 should be 1 instead of " << v2.size();
    EXPECT_EQ(7, *v3.data()) << "The first element of v1 should be 1";
    EXPECT_EQ(1e4, v3.size()) << "The size of v1 should be 1 instead of " << v3.size();
    IntVector tmp(v3);
    EXPECT_EQ(7, *tmp.data()) << "The first element of vector should be 1";
    EXPECT_EQ(1e4, tmp.size()) << "The size of vector should be 1 instead of " << tmp.size();
}

#endif
