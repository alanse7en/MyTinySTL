//
//  defalloc.h
//  
//
//  Created by deng on 14/11/8.
//
//

#ifndef _defalloc_h
#define _defalloc_h

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

namespace TinySTL {
    template <typename T>
    inline T* _allocate(ptrdiff_t size) {
        set_new_handler(0);
        T* ret = (T*)(::operator new ((size_t)(size * sizeof(T) ) ) );
        if (tmp == 0) {
            std::cerr << "out of memory" << endl;
            exit(1);
        }
        return ret;
    }
    
    template <typename T>
    inline void _defallocate(T* buffer) {
        ::operator delete(buffer);
    }
        
    template <typename T1, typename T2>
    inline void _construct(T1* p, const T2& value) {
        new (p) T1 (value);
    }
    
    template <typename T>
    inline void _destroy(T* p) {
        p->~T();
    }
    
    template <typename T>
    class def_allocator{
    public:
        typedef T         value_type;
        typedef T*        pointer;
        typedef const T*  const_pointer;
        typedef T&        reference;
        typedef const T&  const_reference;
        typedef size_t    size_type;
        typedef ptrdiff_t difference_type;
        
        template <typename U>
        struct rebind {
            typedef allocator<U> other;
        }
        
        pointer address(reference x) const {
            return (pointer)(&x);
        }
        
        const_pointer address(const_reference x) const {
            return (const_pointer)(&x);
        }
        
        pointer allocate(size_type n) {
            return _allocate((difference_type)n);
        }
        
        void deallocate(pointer p, size_type n) {
            _defallocate(p);
        }
        
        size_type max_size() const {
            return (size_type)(UINT_MAX/sizeof(T) );
        }
        
        void construct(pointer p, const_reference x) {
            _construct(p, x);
        }
        
        void destroy(pointer p) {
            _destroy(p);
        }
    }
}

#endif
