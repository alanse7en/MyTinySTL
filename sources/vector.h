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
#include "algorithms.h"

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
        typedef Alloc data_allocator;
        
    private:
        iterator start;
        iterator finish;
        iterator end_of_storage;
        
    public:
        // constructor, copy constructor, move constructor and destructor
        vector() : start(0), finish(0), end_of_storage(0) {}
        explicit vector(size_type n);
        vector(size_type n, const T& x);
        template <typename InputIterator>
        vector(InputIterator first, InputIterator last);
        vector(const vector& vec);
        vector(vector&& vec);
        vector(std::initializer_list<T> ilist);
        vector& operator= (const vector& vec);
        vector& operator= (vector&& vec);
        vector& operator= (std::initializer_list<T> ilist);
        ~vector() {
            data_allocator::destroy(start,finish);
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
        size_type size() const { return finish - start;}
        size_type capacity() const { return end_of_storage - start;}
        bool empty() const { return start == finish;}
        void reserve(size_type n);
        void shrink_to_fit() {
            data_allocator::deallocate(finish,end_of_storage-finish);
            end_of_storage = finish;
        }
        // element access
        reference at(size_type position) { return *(start+position);}
        const_reference at(size_type position) const { return *(start+position);}
        reference operator[] (size_type position) { return *(start+position);}
        const_reference operator[] (size_type position) const { return *(start+position);}
        reference front() { return *start;}
        const_reference front() const { return *start;}
        reference back() {return *(finish-1);}
        const_reference back() const { return *(finish-1);}
        pointer data() { return start;}
        const T* data() const { return start;}
        // modify vector
        void clear(){
            data_allocator::destroy(start, finish);
            finish = start;
        }
        void swap(vector& vec) {
            if (this != &vec) {
                std::swap(start, vec.start);
                std::swap(finish, vec.finish);
                std::swap(end_of_storage, vec.end_of_storage);
            }
        }
        template <typename ...Args>
        iterator emplace(const_iterator position, Args&&... args);
        template <typename ...Args>
        void emplace_back(Args&&... args);
        iterator erase(const_iterator position);
        iterator erase(const_iterator first, const_iterator last);
        template <typename Tp>
        void push_back(Tp&& x);
        void pop_back() {
            --finish;
            data_allocator::destroy(finish);
        }
        template <typename Tp>
        iterator insert(const_iterator position, Tp&& x);
        iterator insert(const_iterator position, size_type n, const T& x);
        template <typename InputIterator>
        iterator insert(const_iterator position, InputIterator first, InputIterator last);
        iterator insert(const_iterator position, std::initializer_list<T> ilist);
        void resize(size_type n, const_reference x);
        void resize(size_type n);
        // allocator
        data_allocator get_allocator() const { return data_allocator();}
    protected:
        void allocate_and_fill(size_type n, const T& x) {
            start = data_allocator::allocate(n);
            TinySTL::uninitialized_fill_n(start, n, x);
            finish = end_of_storage = start + n;
        }
        template <typename InputIterator>
        void allocate_and_copy(InputIterator first, InputIterator last) {
            start = data_allocator::allocate(last-first);
            finish = uninitialized_copy(first, last, start);
            end_of_storage = finish;
        }
        template <typename Integer>
        void vector_aux(Integer n, const_reference x, std::true_type) {
            allocate_and_fill(n, x);
        }
        template <typename InputIterator>
        void vector_aux(InputIterator first, InputIterator last, std::false_type) {
            allocate_and_copy(first, last);
        }
        template <typename... Args>
        void emplace_aux(iterator position, std::true_type, Args&&... args);
        template <typename... Args>
        void emplace_aux(iterator position, std::false_type, Args&&... args);
    public:
        template<typename t, typename alloc>
        friend bool operator== (const vector<t,alloc>& vec1, const vector<t,alloc>& vec2);
        template<typename t, typename alloc>
        friend bool operator!= (const vector<t,alloc>& vec1, const vector<t,alloc>& vec2);
    };
    
    template <typename T, typename Alloc>
    vector<T, Alloc>::vector(size_type n) {
        allocate_and_fill(n, T());
    }
    
    template <typename T, typename Alloc>
    vector<T, Alloc>::vector(size_type n, const T& x) {
        allocate_and_fill(n, x);
    }
    
    template <typename T, typename Alloc>
    template <typename InputIterator>
    vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
        vector_aux(first, last, typename std::is_integral<InputIterator>::type());
    }
    
    template <typename T, typename Alloc>
    vector<T,Alloc>::vector(const vector& vec) {
        start = data_allocator::allocate(vec.capacity());
        finish = uninitialized_copy(vec.begin(), vec.end(), start);
        end_of_storage = start + vec.capacity();
    }
    
    template <typename T, typename Alloc>
    vector<T,Alloc>::vector(vector&& vec) {
        start = vec.start;
        finish = vec.finish;
        end_of_storage = end_of_storage;
        vec.start = vec.finish = vec.end_of_storage = 0;
    }
    
    template <typename T, typename Alloc>
    vector<T, Alloc>::vector(std::initializer_list<T> ilist) {
        allocate_and_copy(ilist.begin(), ilist.end());
    }
    template <typename T, typename Alloc>
    vector<T,Alloc>& vector<T,Alloc>::operator= (const vector& vec) {
        __destroy(start, finish);
        data_allocator::deallocate(start,capacity());
        allocate_and_copy(vec.begin(), vec.end());
        return *this;
    }
    
    template <typename T, typename Alloc>
    vector<T,Alloc>& vector<T,Alloc>::operator= (vector&& vec) {
        start = vec.start;
        finish = vec.finish;
        end_of_storage = vec.end_of_storage;
        vec.start = vec.finish = vec.end_of_storage = 0;
        return *this;
    }
    
    template <typename T, typename Alloc>
    vector<T,Alloc>& vector<T, Alloc>::operator= (std::initializer_list<T> ilist) {
        __destroy(start, finish);
        data_allocator::deallocate(start,capacity());
        allocate_and_copy(ilist.begin(), ilist.end());
        return *this;
    }
    
    template <typename T, typename Alloc>
    void vector<T,Alloc>::reserve(size_type n) {
        if (n > capacity()) {
            iterator new_start = data_allocator::allocate(n);
            iterator new_finish = uninitialized_move(start, finish, new_start);
            data_allocator::deallocate(start,capacity());
            start = new_start;
            finish = new_finish;
            end_of_storage = start + n;
        }
    }
    
    template <typename T, typename Alloc>
    typename vector<T,Alloc>::iterator
    vector<T, Alloc>::erase(const_iterator position) {
        return erase(position, position+1);
    }
    
    template <typename T, typename Alloc>
    typename vector<T,Alloc>::iterator
    vector<T,Alloc>::erase(const_iterator first, const_iterator last) {
        iterator i = TinySTL::copy(last, finish, first);
        data_allocator::destroy(i,finish);
        finish = finish - (last - first);
        return first;
    }
    
    template <typename T, typename Alloc>
    template <typename ...Args>
    void vector<T,Alloc>::emplace_back(Args&&... args) {
        if (finish != end_of_storage) {
            __construct(finish, std::forward<Args>(args)...);
            ++finish;
        }
        else
            emplace_aux(finish, std::is_move_constructible<T>(), std::forward<Args>(args)...);
    }
    
    template <typename T, typename Alloc>
    template <typename... Args>
    typename vector<T,Alloc>::iterator
    vector<T,Alloc>::emplace(const_iterator position, Args &&... args) {
        size_type len = position - start;
        if (len <= size() && len >= 0) {
            emplace_aux(position, std::is_move_constructible<T>(), std::forward<Args>(args)...);
        }
        return start + len;
    }
    
    template <typename T, typename Alloc>
    template <typename... Args>
    void vector<T, Alloc>::emplace_aux(iterator position, std::true_type, Args&&... x) {
        if (finish != end_of_storage && start != finish) {
            __construct(finish, *(finish-1));
            ++finish;
            TinySTL::move_backward(position, finish-2, finish-1);
            *position = {std::forward<Args>(x)...};
        }
        else {
            const size_type old_size = size();
            const size_type len = old_size != 0 ? 2*old_size : 1;
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try {
                new_finish = TinySTL::uninitialized_move(start, position, new_start);
                __construct(new_finish, std::forward<Args>(x)...);
                ++new_finish;
                new_finish = TinySTL::uninitialized_move(position, finish, new_finish);
            } catch (...) {
                __destroy(new_start,new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
            data_allocator::deallocate(start,capacity());
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }
    
    template <typename T, typename Alloc>
    template <typename... Args>
    void vector<T, Alloc>::emplace_aux(iterator position, std::false_type,Args&&... x) {
        if (finish != end_of_storage) {
            __construct(finish, *(finish-1));
            ++finish;
            TinySTL::copy_backward(position, finish-2, finish-1);
            *position = {std::forward<Args>(x)...};
        }
        else {
            const size_type old_size = size();
            const size_type len = old_size != 0 ? 2*old_size : 1;
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try {
                new_finish = TinySTL::uninitialized_copy(start, position, new_start);
                __construct(new_finish, std::forward<Args>(x)...);
                ++new_finish;
                new_finish = TinySTL::uninitialized_copy(position, finish, new_finish);
            } catch (...) {
                __destroy(new_start,new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
            __destroy(start,finish);
            data_allocator::deallocate(start, capacity());
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }


    
    template <typename T, typename Alloc>
    template <typename Tp>
    void vector<T,Alloc>::push_back(Tp&& x) {
        emplace_back(std::forward<Tp>(x));
    }
    
    template <typename T, typename Alloc>
    template <typename Tp>
    typename vector<T,Alloc>::iterator
    vector<T,Alloc>::insert(const_iterator position, Tp&& x) {
        return emplace(position, std::forward<Tp>(x));
    }

    template <typename T, typename Alloc>
    typename vector<T,Alloc>::iterator
    vector<T,Alloc>::insert(const_iterator position, size_type n, const T &x) {
        if (n != 0) {
            if (size_type(end_of_storage-finish) >= n) {
                const size_type elems_after = finish - position;
                iterator old_finish = finish;
                if (elems_after > n) {
                    TinySTL::uninitialized_copy(finish-n, finish, finish);
                    finish += n;
                    TinySTL::copy_backward(position, old_finish-n, old_finish);
                    std::fill(position, position+n, x);
                }
                else {
                    TinySTL::uninitialized_fill_n(finish, n-elems_after, x);
                    finish += n-elems_after;
                    TinySTL::uninitialized_copy(position, old_finish, finish);
                    finish += elems_after;
                    std::fill(position, old_finish, x);
                }
                return position;
            }
            else {
                const size_type elems_before = position - start;
                const size_type old_size = size();
                const size_type len = old_size + std::max(old_size,n);
                iterator new_start = data_allocator::allocate(len);
                iterator new_finish = new_start;
                try {
                    new_finish = TinySTL::uninitialized_copy(start, position, new_start);
                    new_finish = TinySTL::uninitialized_fill_n(new_finish, n, x);
                    new_finish = TinySTL::uninitialized_copy(position, finish, new_finish);
                } catch (...) {
                    __destroy(new_start, new_finish);
                    data_allocator::deallocate(new_start,len);
                    throw;
                }
                __destroy(start, finish);
                data_allocator::deallocate(start,capacity());
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
                return new_start + elems_before;
            }
        }
        else {
            return position;
        }
    }

    template <typename T, typename Alloc>
    template <typename InputIterator>
    typename vector<T,Alloc>::iterator
    vector<T,Alloc>::insert(const_iterator position, InputIterator first, InputIterator last) {
        const size_type n = last - first;
        if (n > 0) {
            if (size_type(end_of_storage-finish) >= n) {
                const size_type elems_after = finish - position;
                iterator old_finish = finish;
                if (elems_after > n) {
                    TinySTL::uninitialized_copy(finish-n, finish, finish);
                    finish += n;
                    TinySTL::copy_backward(position, old_finish-n, old_finish);
                    TinySTL::copy(first, last, position);
                }
                else {
                    TinySTL::uninitialized_fill_n(finish, n-elems_after, *(finish-1));
                    finish += n-elems_after;
                    TinySTL::uninitialized_copy(position, old_finish, finish);
                    finish += elems_after;
                    TinySTL::copy(first, last, position);
                }
                return position;
            }
            else {
                const size_type elems_before = position - start;
                const size_type old_size = size();
                const size_type len = old_size + std::max(old_size,n);
                iterator new_start = data_allocator::allocate(len);
                iterator new_finish = new_start;
                try {
                    new_finish = TinySTL::uninitialized_copy(start, position, new_start);
                    new_finish = TinySTL::uninitialized_copy(first, last, new_finish);
                    new_finish = TinySTL::uninitialized_copy(position, finish, new_finish);
                } catch (...) {
                    __destroy(new_start, new_finish);
                    data_allocator::deallocate(new_start,len);
                    throw;
                }
                __destroy(start, finish);
                data_allocator::deallocate(start,capacity());
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
                return new_start + elems_before;
            }
        }
        else {
            return position;
        }
    }
    
    
    template <typename T, typename Alloc>
    typename vector<T,Alloc>::iterator
    vector<T,Alloc>::insert(const_iterator position, std::initializer_list<T> ilist) {
        return insert(position, ilist.begin(), ilist.end());
    }
    
    template <typename T, typename Alloc>
    void vector<T,Alloc>::resize(size_type n, const_reference x) {
        const size_type old_size = size();
        if (old_size > n) {
            __destroy(start+n, finish);
            finish = start+n;
        }
        else if (old_size < n) {
            if (n > capacity()) {
                iterator new_start = data_allocator::allocate(n);
                iterator new_finish = new_start;
                try {
                    new_finish = TinySTL::uninitialized_copy(start, finish, new_start);
                    new_finish = TinySTL::uninitialized_fill_n(new_finish, n-old_size, x);
                } catch (...) {
                    __destroy(new_start,new_finish);
                    data_allocator::deallocate(new_start,n);
                    throw;
                }
                __destroy(start,finish);
                data_allocator::deallocate(start,capacity());
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + n;
            }
            else {
                finish = TinySTL::uninitialized_fill_n(finish, n-old_size, x);
            }
        }
    }
    
    template <typename T, typename Alloc>
    void vector<T,Alloc>::resize(size_type n) {
        resize(n, T());
    }
    
    template<typename t, typename alloc>
    bool operator== (const vector<t,alloc>& vec1, const vector<t,alloc>& vec2) {
        if (vec1.size() != vec2.size()) {
            return false;
        }
        else {
            auto ite1 = vec1.begin();
            auto ite2 = vec2.begin();
            for (typename vector<t,alloc>::size_type i = 0; i != vec1.size(); ++i) {
                if (*ite1 != *ite2) {
                    return false;
                }
                else {
                    ++ite1;
                    ++ite2;
                }
            }
            return true;
        }
    }
    template<typename t, typename alloc>
    bool operator!= (const vector<t,alloc>& vec1, const vector<t,alloc>& vec2) {
        return !(vec1==vec2);
    }
//end of namespace
}
#endif
