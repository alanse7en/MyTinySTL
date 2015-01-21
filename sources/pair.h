//
//  pair.h
//  test
//
//  Created by deng on 15/1/21.
//  Copyright (c) 2015年 deng. All rights reserved.
//

#ifndef test_pair_h
#define test_pair_h

namespace TinySTL {
    template <class T1, class T2>
    struct pair {
        typedef T1 first_type;
        typedef T2 second_type;
        
        first_type first;
        second_type second;
        
        pair() : first(T1()), second(T2()) {}
        pair(const T1& x, const T2& y) : first(x), second(y) {}
        
        bool operator==(const pair& rhs) {
            return (first==rhs.first)&&(second==rhs.second);
        }
    };
}

#endif
