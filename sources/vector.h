//
//  vector.h
//  test
//
//  Created by deng on 14/11/20.
//  Copyright (c) 2014年 deng. All rights reserved.
//

#ifndef test_vector_h
#define test_vector_h

#include "memory.h"

namespace TinySTL {
    template <typename T, typename  Alloc = allocator<T> >
    class vector {
    public:
        typedef T                value_type;
        typedef T*               pointer;
        typedef T*               iterator;
        typedef const pointer    const_pointer;
        typedef const iterator   const_iterator;
        typedef T&               reference;
        typedef const reference  const_reference;
        typedef size_t           size_type;
        typedef ptrdiff_t        difference_type;
        
    protected:
        typedef allocator<T> data_allocator;
        
    private:
        iterator start;
        iterator finish;
        iterator end_of_storage;
        
    public:
        // constructor, copy constructor, move constructor and destructor
        vector() : start(0), finish(0), end_of_storage(0) {}
        explicit vector(const size_type n);
        vector(const size_type n, const_reference x);
        template <typename InputIterator>
        vector(InputIterator first, InputIterator last);
        vector(const vector& vec);
        vector(vector&& vec);
        vector& operator= (const vector& vec);
        vector& operator= (vector&& vec);
        ~vector() {
            destroy(start,finish);
            if (start) {
                data_allocator::deallocate(start,end_of_storage-start);
            }
        }
        // iterators
        iterator begin() { return start;}
        const_iterator begin() const { return start;}
        const_iterator cbegin() const { return start;}
        iterator end() { return finish;}
        const_iterator end() const { return finish;}
        const_iterator cend() const { return finish;}
        // size and capacity
        difference_type size() const { return finish-start;}
        difference_type capacity() const { return end_of_storage-start;}
        bool empty() const { return start == finish;}
        void resize(const size_type n, value_type x = value_type());
        void reserve(const size_type n);
        void shrink_to_fit() {
            data_allocator::deallocate(finish,end_of_storage-finish);
            end_of_storage = finish;
        }
        // data access
        reference operator[] (const difference_type i) { return *(start+i);}
        const_reference operator[] (const difference_type i) const { return *(start+i);}
        reference front() {return *start;}
        const_reference front() const { return *start;}
        reference back() {return *start;}
        const_reference back() const { return *start;}
        pointer data() { return start;}
        // modify vector
        void clear(){
            destroy(start, finish);
            finish = start;
        }
        void swap(vector& vec) {
            if (this != &vec) {
                std::swap(start, vec.start);
                std::swap(finish, vec.finish);
                std::swap(end_of_storage, vec.end_of_storage);
            }
        }
        void push_back(const T& x);
        void pop_back() {
            --finish;
            destroy(finish);
        }
        iterator insert(iterator position, const T&x);
        void insert(iterator position, const size_type n, const T& x);
        template <typename InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        // allocator
        data_allocator get_allocator() { return data_allocator();}
    protected:
        void allocate_and_fill(const size_type n, const T& x) {
            start = data_allocator::allocate(n);
            uninitialized_fill_n(start, n, x);
            finish = end_of_storage = start + n;
        }
        template <typename InputIterator>
        void allocate_and_copy(InputIterator first, InputIterator last) {
            start = data_allocator::allocate(last-first);
            finish = uninitialized_copy(first, last, start);
            end_of_storage = finish;
        }
        void insert_aux(iterator position, const T& x);
    public:
        template<typename t, typename alloc>
        friend bool operator== (const vector<t,alloc>& vec1, const vector<t,alloc>& vec2);
        template<typename t, typename alloc>
        friend bool operator!= (const vector<t,alloc>& vec1, const vector<t,alloc>& vec2);
    };
}

#endif
