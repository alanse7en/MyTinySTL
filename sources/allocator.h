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
#include "stl_construct.h"

namespace TinySTL {
    typedef default_alloc alloc;
    
    template <typename T, typename Alloc = alloc>
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
        
        static void construct(T* p) {
            __construct(p);
        }
        
        static void construct(T* p, const T& value) {
            __construct(p,value);
        }
        
        static void destroy(T* p) {
            __destroy(p);
        }
        
        static void destroy(T* first, T* last) {
            __destroy(first, last);
        }
    };
    
    template <typename T, typename Alloc>
     T* allocator<T,Alloc>::allocate(size_t n) {
        return 0==n ? 0 : (T *)(alloc::allocate(n * sizeof(T) ) );
    }
    
    template <typename T, typename Alloc>
     T* allocator<T,Alloc>::allocate() {
        return (T *)(alloc::allocate(sizeof(T) ) );
    }
    
    template <typename T, typename Alloc>
     void allocator<T,Alloc>::deallocate(T* p, size_t n) {
        if (n > 0) {
            alloc::deallocate(p,n*sizeof(T) );
        }
    }
    
    template <typename T, typename Alloc>
     void allocator<T,Alloc>::deallocate(T* p) {
        alloc::deallocate(p,sizeof(T) );
    }
}

#endif
