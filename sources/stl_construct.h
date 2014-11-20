//
//  stl_construct.h
//  test
//
//  Created by deng on 14/11/20.
//  Copyright (c) 2014年 deng. All rights reserved.
//

#ifndef test_stl_construct_h
#define test_stl_construct_h

namespace TinySTL {
    template <typename T>
    void __construct(T* p) {
        new (p) T ();// placement new
    }
    
    template <typename T>
    void __construct(T* p, const T& value) {
        new (p) T (value);
    }
    
    template <typename T>
    void __destroy(T* p) {
        p->~T();
    }
    
    template <typename T>
    void __destroy(T* first, T* last) {
        for (; first != last; ++first) {
            first->~T();
        }
    }
}

#endif
