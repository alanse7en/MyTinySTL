//
//  mapTest.h
//  test
//
//  Created by deng on 15/1/21.
//  Copyright (c) 2015年 deng. All rights reserved.
//

#ifndef test_mapTest_h
#define test_mapTest_h

#include "map.h"
#include <gtest/gtest.h>

class MapTest : public testing::Test {
public:
    typedef TinySTL::map<int, double> map;
    typedef typename map::value_type value_type;
    map m1;
    map m2;
    map m3;
    virtual void SetUp() {
        std::initializer_list<value_type> ilist = {value_type(1,0.1),value_type(2,0.2),value_type(3,0.3)};
        {
            map tmp(ilist);
            m2 = tmp;
        }
        {
            map tmp(ilist.begin(),ilist.end());
            m3 = tmp;
        }
    }
};

TEST_F(MapTest, ConstructorTest) {
    EXPECT_EQ(0, m1.size());
    EXPECT_EQ(3, m2.size());
    EXPECT_EQ(3, m3.size());
    map m4(std::move(m2));
    EXPECT_EQ(3, m4.size());
    EXPECT_EQ(0, m2.size());
    map m5;
    EXPECT_EQ(0, m5.size());
    m5 = std::move(m4);
    EXPECT_EQ(3, m5.size());
    EXPECT_EQ(0, m4.size());
}

TEST_F(MapTest, EqualTest) {
    EXPECT_EQ(false, m1==m2);
    EXPECT_EQ(true, m2==m3);
}

TEST_F(MapTest, FindTest) {
    auto ite = m2.find(1);
    EXPECT_EQ(0.1, (*ite).second);
    ite = m3.find(10);
    EXPECT_EQ(true, ite==m3.end());
}

TEST_F(MapTest, OperatorTest) {
    EXPECT_EQ(0.1, m2[1]);
    m2[1] = 1.0;
    EXPECT_EQ(1, m2[1]);
    m1[10] = 1.0;
    EXPECT_EQ(1, m1[10]);
}

#endif
