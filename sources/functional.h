//
//  functional.h
//  test
//
//  Created by deng on 15/1/7.
//  Copyright (c) 2015年 deng. All rights reserved.
//

#ifndef test_functional_h
#define test_functional_h

#include "pair.h"

namespace TinySTL {
    template <class T>
    struct identity : public std::unary_function<T, T> {
        const T& operator() (const T& x) const { return x;}
    };
    
    
    template <class T>
    struct less {
        bool operator()(const T &x, const T &y) const
        {
            if (x<y)
                return true;
            else
                return false;
        }
    };
    
    template <class T>
    struct select1st;
    template <class K,class T>
    struct select1st<std::pair<K, T> > {
        K operator()(std::pair<K,T> x) {
            return x.first;
        }
    };
}

#endif
