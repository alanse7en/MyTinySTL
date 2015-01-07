//
//  functional.h
//  test
//
//  Created by deng on 15/1/7.
//  Copyright (c) 2015年 deng. All rights reserved.
//

#ifndef test_functional_h
#define test_functional_h

namespace TinySTL {
    struct identity {
        template<typename U>
        constexpr auto operator()(U&& v) const noexcept
        -> decltype(std::forward<U>(v))
        {
            return std::forward<U>(v);
        }
    };
    
    
    template <class T>
    struct less {
        int operator()(const T &x, const T &y) const
        {
            if (x==y)
                return 0;
            if (x<y) {
                return -1;
            }
            return 1;
        }
    };
}

#endif
