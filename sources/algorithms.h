//
//  algorithms.h
//  test
//
//  Created by deng on 14-9-23.
//  Copyright (c) 2014年 deng. All rights reserved.
//

#ifndef __test__algorithms__
#define __test__algorithms__

namespace TinySTL {
    
    template <typename InputIterator, typename ForwardIterator>
    ForwardIterator copy_backward(InputIterator first, InputIterator last, ForwardIterator result)
    {
        while (first != last) {
            *(--result) = *(--last);
        }
        return result;
    }
    
    template <typename InputIterator, typename ForwardIterator>
    ForwardIterator move_backward(InputIterator first, InputIterator last, ForwardIterator result)
    {
        while (first != last) {
            *(--result) = std::move(*(--last));
        }
        return result;
    }
    
    template <typename InputIterator, typename OutputIterator>
    OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
        while (first != last) {
            *result++ = *first++;
        }
        return result;
    }
    
    template <typename InputIterator, typename OutputIterator>
    OutputIterator move(InputIterator first, InputIterator last, OutputIterator result) {
        while (first != last) {
            *result++ = std::move(*first++);
        }
        return result;
    }
    
    template <typename InputIterator>
    size_t __distance(InputIterator first, InputIterator last) {
        size_t result = 0;
        for (; first != last; ++first) {
            ++result;
        }
        return result;
    }
    
// end of namespace
}


#endif /* defined(__test__algorithms__) */
