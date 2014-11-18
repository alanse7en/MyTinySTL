//
//  allocator.h
//
//
//  Created by deng on 14/11/12.
//
//

#ifndef _allocator_h
#define _allocator_h

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include "stl_alloc.h"

namespace TinySTL {
    typedef default_alloc alloc;
    
    template <typename T>
    class allocator {
    public:
        typedef T          value_type;
        typedef T*         pointer;
        typedef const T*   const_pointer;
        typedef T&         reference;
        typedef const T&   const_reference;
        typedef size_t     size_type;
        typedef ptrdiff_t  difference_type;
        
        static T* allocate(size_t n);
        static T* allocate();
        static void deallocate(T* p, size_t n);
        static void deallocate(T* p);
    };
    
    template <typename T>
    T* allocator<T>::allocate(size_t n) {
        return 0==n ? 0 : (T *)(alloc::allocate(n * sizeof(T) ) );
    }
    
    template <typename T>
    T* allocator<T>::allocate() {
        return (T *)(alloc::allocate(sizeof(T) ) );
    }
    
    template <typename T>
    void allocator<T>::deallocate(T* p, size_t n) {
        if (n > 0) {
            alloc::deallocate(p,n*sizeof(T) );
        }
    }
    
    template <typename T>
    void allocator<T>::deallocate(T* p) {
        alloc::deallocate(p,sizeof(T) );
    }
}

#endif
