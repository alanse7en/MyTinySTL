//
//  listTest.h
//  test
//
//  Created by deng on 14/12/27.
//  Copyright (c) 2014年 deng. All rights reserved.
//

#ifndef test_listTest_h
#define test_listTest_h

#include "list.h"
#include <gtest/gtest.h>

typedef TinySTL::list<int> list;

TEST(ListTest, Constructor) {
    list list1(10,7);
    list list2(1e4,7);
    list list3(list2);
    EXPECT_FALSE(list1 == list2);
    EXPECT_EQ(list2, list3);
}

#endif
