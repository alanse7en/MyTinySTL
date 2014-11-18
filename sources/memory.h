//
//  memory.h
//  test
//
//  Created by deng on 14/11/18.
//  Copyright (c) 2014年 deng. All rights reserved.
//

#ifndef test_memory_h
#define test_memory_h

#include "allocator.h"
#include "type_traits.h"
#include "stl_iterator.h"

namespace TinySTL {
    
    template <typename T>
    void construct(T* p) {
        new (p) T ();// placement new
    }
    
    template <typename T>
    void construct(T* p, const T& value) {
        new (p) T (value);
    }
    
    
    template <typename T>
    void destroy(T* p) {
        p->~T();
    }
    
    template <typename T>
    void destroy(T* first, T* last) {
        for (; first != last; ++first) {
            first->~T();
        }
    }
    
    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
        ForwardIterator curr = first;
        for(; n > 0; --n, ++curr)
            construct(&*curr, x);
        return curr;
    }
    
    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
        return fill_n(first, n, x);
    }
    
    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T&x, T*) {
        typedef typename __type_traits<T*>::is_POD_type is_POD;
        return __uninitialized_fill_n_aux(first, n, x, is_POD());
    }
    
    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
        return __uninitialized_fill_n(first, n, x, value_type(first));
    }
    
    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
        return copy(first, last, result);
    }
    
    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
        ForwardIterator curr = result;
        for(;first != last; ++curr, ++first)
            construct(&*curr, *first);
        return curr;
    }
    
    template <typename InputIterator, typename ForwardIterator, typename T>
    inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*) {
        typedef typename __type_traits<T>::is_POD_type is_POD;
        return __uninitialized_copy_aux(first, last, result, is_POD());
    }
    
    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
        return __uninitialized_copy(first, last, result, value_type(result));
    }
    
    inline char* uninitialized_copy(const char* first, const char* last, char* result) {
        memmove(result, first, last-first);
        return result + (last-first);
    }
    
    inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
        memmove(result, first, sizeof(wchar_t) * (last-first) );
        return result + (last-first);
    }
    
    template <typename ForwardIterator, typename T>
    inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
        ForwardIterator curr = first;
        for(; curr != last; ++curr)
            construct(&*curr, x);
    }
    
    template <typename ForwardIterator, typename T>
    inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type) {
        return fill(first, last, x);
    }
    
    template <typename ForwardIterator, typename T>
    inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T&x, T*) {
        typedef typename __type_traits<T*>::is_POD_type is_POD;
        return __uninitialized_fill_aux(first, last, x, is_POD());
    }
    
    template <typename ForwardIterator, typename T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
        return __uninitialized_fill(first, last, x, value_type(first));
    }
}





#endif
