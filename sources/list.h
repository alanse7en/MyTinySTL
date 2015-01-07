//
//  list.h
//  test
//
//  Created by deng on 14/12/11.
//  Copyright (c) 2014年 deng. All rights reserved.
//

#ifndef test_list_h
#define test_list_h

#include <cstddef>
#include "allocator.h"
#include "algorithms.h"
#include "stl_iterator.h"

namespace TinySTL {
    template <typename T>
    struct __list_node{
        T      data;
        void*  prev;
        void*  next;
    };
    
    template <class T>
    struct __list_const_iterator;
    
    template <class T>
    struct __list_iterator{
        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef ptrdiff_t difference_type;
        typedef __list_node<T>* link_type;
        
        link_type node_ptr;
        
        __list_iterator(link_type x) : node_ptr(x) {}
        __list_iterator() {}
        __list_iterator(const __list_iterator& x) : node_ptr(x.node_ptr) {}
        
        reference operator* () const { return  (reference)((*node_ptr).data);}
        pointer operator-> () const { return (pointer)(&(operator*()));}
        
        __list_iterator& operator++() {
            node_ptr = (link_type)((*node_ptr).next);
            return *this;
        }
        __list_iterator operator++(int) {
            __list_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        __list_iterator& operator--() {
            node_ptr = (link_type)((*node_ptr).prev);
            return *this;
        }
        __list_iterator operator--(int) {
            __list_iterator tmp = *this;
            --(*this);
            return tmp;
        }
        
        bool operator==( const __list_iterator& x) {
            return (node_ptr == x.node_ptr);
        }
        bool operator==( const __list_const_iterator<T>& x) {
            return (node_ptr == x.node_ptr);
        }
        bool operator!=( const __list_iterator& x) {
            return (node_ptr != x.node_ptr);
        }
        bool operator!=( const __list_const_iterator<T>& x) {
            return (node_ptr != x.node_ptr);
        }
    };
    
    template <class T>
    struct __list_const_iterator{
        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef const T* pointer;
        typedef const T& reference;
        typedef ptrdiff_t difference_type;
        typedef __list_node<T>* link_type;
        
        link_type node_ptr;
        
        __list_const_iterator(link_type x) : node_ptr(x) {}
        __list_const_iterator() {}
        __list_const_iterator(const __list_iterator<T>& x) : node_ptr(x.node_ptr) {}
        __list_const_iterator(const __list_const_iterator& x) : node_ptr(x.node_ptr) {}
        
        reference operator* () const { return  (reference)((*node_ptr).data);}
        pointer operator-> () const { return (pointer)(&(operator*()));}
        
        __list_const_iterator& operator++() {
            node_ptr = (link_type)((*node_ptr).next);
            return *this;
        }
        __list_const_iterator operator++(int) {
            __list_const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        __list_const_iterator& operator--() {
            node_ptr = (link_type)((*node_ptr).prev);
            return *this;
        }
        __list_const_iterator operator--(int) {
            __list_const_iterator tmp = *this;
            --(*this);
            return tmp;
        }
        
        bool operator==( const __list_const_iterator& x) {
            return (node_ptr == x.node_ptr);
        }
        bool operator==( const __list_iterator<T>& x) {
            return (node_ptr == x.node_ptr);
        }
        bool operator!=( const __list_const_iterator& x) {
            return (node_ptr != x.node_ptr);
        }
        bool operator!=( const __list_iterator<T>& x) {
            return (node_ptr != x.node_ptr);
        }
    };
    
    template <typename T , typename Alloc = alloc>
    class list{
    public:
        typedef __list_node<T>* link_type;
        typedef __list_iterator<T> iterator;
        typedef __list_const_iterator<T> const_iterator;
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
    protected:
        link_type end_ptr;
        typedef allocator<__list_node<T>, Alloc> data_allocator;
        
        void empty_initialize() {
            end_ptr = get_node();
            end_ptr->next = end_ptr;
            end_ptr->prev = end_ptr;
        }
        
        link_type get_node() {
            return data_allocator::allocate();
        }
        
        void put_node(link_type p) { data_allocator::deallocate(p);};
        
        template <typename... Args>
        link_type create_node(Args&&... args) {
            link_type p = get_node();
            __construct(&(p->data),std::forward<Args>(args)...);
            return p;
        }
        
        void destroy_node(link_type p) {
            __destroy(&(p->data));
            put_node(p);
        }
        
        void list_aux(size_type count, const T& value, std::true_type);
        template <typename InputIterator>
        void list_aux(InputIterator first, InputIterator last, std::false_type);
        
        iterator insert_aux(const_iterator pos, size_type count, const T& value ,std::true_type);
        template <typename InputIterator>
        iterator insert_aux(const_iterator pos, InputIterator first, InputIterator last, std::false_type);
        
        void transfer(iterator possition, iterator first, iterator last);
        
    public:
        // Constructors and destructor
        list() {
            empty_initialize();
        }
        explicit list(size_type count, const T& value) {
            list_aux(count, value, std::true_type());
        }
        explicit list( size_type count) {
            list_aux(count, T(), std::true_type());
        }
        template <typename InputIterator>
        list(InputIterator first, InputIterator last) {
            list_aux(first, last, std::is_integral<InputIterator>());
        }
        list(const list& other ) {
            list_aux(other.cbegin(), other.cend(), std::false_type());
        }
        list(list&& other) {
            empty_initialize();
            std::swap(end_ptr, other.end_ptr);
        }
        list(std::initializer_list<T> init) {
            list_aux(init.begin(), init.end(), std::false_type());
        }
        ~list() {
            iterator ite = begin();
            iterator last = end();
            for (; ite != last; ++ite) {
                destroy_node(ite.node_ptr);
            }
            destroy_node(end_ptr);
        }
        
        // Copy operator
        list& operator= (const list& other);
        list& operator= (list&& other);
        list& operator= (std::initializer_list<T> ilist);
        
        // Assign operator
        void assign( size_type count, const T& value);
        template <typename InputIterator>
        void assign(InputIterator first, InputIterator last);
        void assign(std::initializer_list<T> ilist);
        
        // Allocator
        data_allocator get_allocator() const { return data_allocator();}
        
        // Iterators
        iterator begin() { return iterator((link_type) end_ptr->next);}
        const_iterator begin() const { return const_iterator((link_type) end_ptr->next);}
        const_iterator cbegin() const { return const_iterator((link_type) end_ptr->next);}
        
        iterator end() { return iterator(end_ptr);}
        const_iterator end() const { return const_iterator(end_ptr);}
        const_iterator cend() const { return const_iterator(end_ptr);}
        
        // Capacity
        bool empty() const { return end_ptr == end_ptr->next;}
        size_type size() const {
            return TinySTL::__distance(cbegin(), cend());
        }
        
        // Element access
        reference front() { return *begin();}
        const_reference front() const { return *begin();}
        reference back() { return *(--end());}
        const_reference back() const { return *(--end());}
        // Modifiers
        void clear() {
            link_type curr = (link_type)end_ptr->next;
            while (curr != end_ptr) {
                link_type tmp = curr;
                curr = (link_type)curr->next;
                destroy_node(tmp);
            }
            end_ptr->next = end_ptr;
            end_ptr->prev = end_ptr;
        }
        
        iterator insert(const_iterator pos, const T& value);
        iterator insert(const_iterator pos, T&& value);
        iterator insert(const_iterator pos, size_type count, const T& value);
        template <typename InputIterator>
        iterator insert(const_iterator pos, InputIterator first, InputIterator last);
        iterator insert(const_iterator pos, std::initializer_list<T> ilist);
        
        template <typename... Args>
        iterator emplace(const_iterator pos, Args&&... args);
        template <typename... Args>
        void emplace_back(Args&&... args) {
            emplace(end(), std::forward<Args>(args)...);
        }
        template <typename... Args >
        void emplace_front(Args&&... args) {
            emplace(cbegin(), std::forward<Args>(args)...);
        }
        
        iterator erase(const_iterator pos) {
            ((link_type)pos.node_ptr->prev)->next = pos.node_ptr->next;
            ((link_type)pos.node_ptr->next)->prev = pos.node_ptr->prev;
            link_type result = (link_type)pos.node_ptr->next;
            destroy_node(pos.node_ptr);
            return result;
        }
        iterator erase(const_iterator first, const_iterator last) {
            iterator ite = first;
            while (ite != last) {
                ite = erase(ite);
            }
            return ite;
        }
        
        void push_back(const T& value) {
            emplace_back(value);
        }
        void push_back(T&& value) {
            emplace_back(std::forward<T>(value));
        }
        
        void pop_back() {
            iterator tmp = end();
            erase(--tmp);
        }
        
        void push_front(const T& value) {
            emplace_front(value);
        }
        void push_front(T&& value) {
            emplace_front(std::forward<T>(value));
        }

        void pop_front() {
            erase(begin());
        }

        void resize(size_type count);
        void resize(size_type count, const value_type& value);
        
        void swap(list& other) {
            std::swap(end_ptr, other.end_ptr);
        }
        // Operations
        void merge(list& other);
        void merge(list&& other);
        template <typename Compare>
        void merge(list& other, Compare comp);
        template <typename Compare>
        void merge(list&& other, Compare comp);
        
        void splice(const_iterator pos, list& other);
        void splice(const_iterator pos, list&& other);
        void splice(const_iterator pos, list& other, const_iterator it);
        void splice(const_iterator pos, list&& other, const_iterator it);
        void splice(const_iterator pos, list& other,
                    const_iterator first, const_iterator last);
        void splice(const_iterator pos, list&& other,
                    const_iterator first, const_iterator last);
        
        void remove(const T& value);
        template <typename UnaryPredicate>
        void remove_if(UnaryPredicate p);
        
        void reverse();
        
        void unique();
        template <typename BinaryPredicate>
        void unique(BinaryPredicate p);
        
        void sort();
        template <typename Compare>
        void sort(Compare comp);
        
    protected:
        
    public:
        template < class t, class alloc >
        friend bool operator==( const list<t,alloc>& lhs, const list<t,alloc>& rhs );
        template < class t, class alloc >
        friend bool operator!=( const list<t,alloc>& lhs, const list<t,alloc>& rhs );
        template < class t, class alloc >
        friend bool operator>=( const list<t,alloc>& lhs, const list<t,alloc>& rhs );
        template < class t, class alloc >
        friend bool operator<=( const list<t,alloc>& lhs, const list<t,alloc>& rhs );
        template < class t, class alloc >
        friend bool operator>( const list<t,alloc>& lhs, const list<t,alloc>& rhs );
        template < class t, class alloc >
        friend bool operator<( const list<t,alloc>& lhs, const list<t,alloc>& rhs );
    };
    
    
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::list_aux(size_type count, const T &value, std::true_type) {
        empty_initialize();
        while (count--) {
            push_back(value);
        }
    }
    
    template <typename T, typename Alloc>
    template <typename InputIterator>
    void list<T,Alloc>::list_aux(InputIterator first, InputIterator last, std::false_type) {
        empty_initialize();
        while (first != last) {
            push_back(*first);
            ++first;
        }
    }
    
    template <typename T, typename Alloc>
    list<T,Alloc>& list<T,Alloc>::operator= (const list& other) {
        clear();
        list_aux(other.begin(), other.end(), std::false_type());
        return *this;
    }
    template <typename T, typename Alloc>
    list<T,Alloc>& list<T,Alloc>::operator= (list&& other) {
        clear();
        std::swap(end_ptr, other.end_ptr);
        return *this;
    }
    template <typename T, typename Alloc>
    list<T,Alloc>& list<T,Alloc>::operator= (std::initializer_list<T> ilist) {
        clear();
        list_aux(ilist.begin(), ilist.end(), std::false_type());
        return *this;
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::assign(size_type count, const T& value) {
        clear();
        list_aux(count, value, std::true_type());
    }
    template <typename T, typename Alloc>
    template <typename InputIterator>
    void list<T,Alloc>::assign(InputIterator first, InputIterator last) {
        clear();
        list_aux(first, last, std::is_integral<InputIterator>());
    }
    template <typename T, typename Alloc>
    void list<T,Alloc>::assign(std::initializer_list<T> ilist) {
        clear();
        list_aux(ilist.begin(), ilist.end(), std::false_type());
    }
    
    template <typename T, typename Alloc>
    typename list<T,Alloc>::iterator
    list<T,Alloc>::insert( const_iterator pos, const T& value ) {
        return emplace(pos, value);
    }
    template <typename T, typename Alloc>
    typename list<T,Alloc>::iterator
    list<T,Alloc>::insert( const_iterator pos, T&& value ) {
        return emplace(pos, std::forward<T>(value));
    }
    
    template <typename T, typename Alloc>
    template <typename InputIterator>
    typename list<T,Alloc>::iterator
    list<T,Alloc>::insert_aux(const_iterator pos, InputIterator first, InputIterator last, std::false_type) {
        iterator result = pos;
        while (first != last) {
            --last;
            result = insert(result, *last);
        }
        return result;
    }

    template <typename T, typename Alloc>
    typename list<T,Alloc>::iterator
    list<T,Alloc>::insert_aux(const_iterator pos, size_type count, const T& value, std::true_type) {
        iterator result = pos;
        for (size_type i = 0; i < count; ++i) {
            result = insert(result, value);
        }
        return result;
    }
    
    template <typename T, typename Alloc>
    template <typename InputIterator>
    typename list<T,Alloc>::iterator
    list<T,Alloc>::insert(const_iterator pos, InputIterator first, InputIterator last) {
        return insert_aux(pos, first, last, std::is_integral<InputIterator>());
    }
    
    template <typename T, typename Alloc>
    typename list<T,Alloc>::iterator
    list<T,Alloc>::insert(const_iterator pos, size_type count, const T& value) {
        return insert_aux(pos, count, value, std::true_type());
    }

    
    template <typename T, typename Alloc>
    typename list<T,Alloc>::iterator
    list<T,Alloc>::insert( const_iterator pos, std::initializer_list<T> ilist ) {
        iterator result = pos;
        auto first = ilist.begin();
        auto last  = ilist.end();
        while (first != last) {
            --last;
            result = insert(result, *last);
        }
        return result;
    }
    
    template <typename T, typename Alloc>
    template <typename... Args>
    typename list<T,Alloc>::iterator
    list<T,Alloc>::emplace(const_iterator pos, Args&&...args) {
        link_type tmp = create_node(std::forward<Args>(args)...);
        tmp->next = pos.node_ptr;
        tmp->prev = pos.node_ptr->prev;
        ((link_type)pos.node_ptr->prev)->next = tmp;
        pos.node_ptr->prev = tmp;
        return iterator(tmp);
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::resize(size_type count) {
        resize(count, T());
    }
    template <typename T, typename Alloc>
    void list<T,Alloc>::resize(size_type count, const value_type& value) {
        if (size() > count) {
            size_type num_to_delete = size() - count;
            for (; num_to_delete != 0; --num_to_delete) {
                pop_back();
            }
        }
        else if (size() < count) {
            insert(end(), count-size(), value);
        }
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::transfer(iterator possition, iterator first, iterator last) {
        if (possition != last && first != last) {
            ((link_type)possition.node_ptr->prev)->next = first.node_ptr;
            ((link_type)last.node_ptr->prev)->next = possition.node_ptr;
            ((link_type)first.node_ptr->prev)->next = last.node_ptr;
            link_type tmp = (link_type)possition.node_ptr->prev;
            possition.node_ptr->prev = last.node_ptr->prev;
            last.node_ptr->prev = first.node_ptr->prev;
            first.node_ptr->prev = tmp;
        }
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::merge(list& other) {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = other.begin();
        iterator last2 = other.end();
        
        while (first1 != last1 && first2 != last2) {
            if (*first2 < *first1) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else
                ++first1;
        }
        if (first2 != last2)
            transfer(last1, first2, last2);
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::merge(list&& other) {
        merge(other);
    }
    
    template <typename T, typename Alloc>
    template <typename Compare>
    void list<T,Alloc>::merge(list& other, Compare comp) {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = other.begin();
        iterator last2 = other.end();
        while (first1 != last1 && first2 != last2) {
            if (comp(*first2,*first1)) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else
                ++first1;
        }
        if (first2 != last2)
            transfer(last1, first2, last2);
    }
    
    template <typename T, typename Alloc>
    template <typename Compare>
    void list<T,Alloc>::merge(list&& other, Compare comp) {
        merge(other, comp);
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::splice(const_iterator pos, list& other) {
        transfer(pos, other.begin(), other.end());
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::splice(const_iterator pos, list&& other) {
        transfer(pos, other.begin(), other.end());
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::splice(const_iterator pos, list& other, const_iterator it) {
        iterator next = it;
        ++next;
        if (pos == it || pos == next) return;
        transfer(pos, it, next);
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::splice(const_iterator pos, list&& other, const_iterator it) {
        iterator next = it;
        ++next;
        if (pos == it || pos == next) return;
        transfer(pos, it, next);
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::splice(const_iterator pos, list& other,
                               const_iterator first, const_iterator last) {
        transfer(pos, first, last);
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::splice(const_iterator pos, list&& other,
                               const_iterator first, const_iterator last) {
        transfer(pos, first, last);
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::remove(const T& value) {
        for (iterator ite = begin(); ite != end(); ++ite) {
            if (*ite == value) erase(ite);
        }
    }
    
    template <typename T, typename Alloc>
    template <typename UnaryPredicate>
    void list<T,Alloc>::remove_if(UnaryPredicate p) {
        for (iterator ite = begin(); ite != end(); ++ite) {
            if (p(*ite)) erase(ite);
        }
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::reverse() {
        if (end_ptr->next == end_ptr || link_type(end_ptr->next)->next == end_ptr) return;
        
        iterator first = begin();
        ++first;
        while (first.node_ptr != end_ptr) {
            iterator old = first;
            ++first;
            transfer(begin(), old, first);
        }
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::unique() {
        if (begin() == end()) return;
        iterator first = begin();
        iterator last = end();
        iterator next = first;
        while (++next != last) {
            if (*next == *first)
                erase(next);
            else
                first = next;
            next = first;
        }
    }
    
    template <typename T, typename Alloc>
    template <typename BinaryPredicate>
    void list<T,Alloc>::unique(BinaryPredicate p) {
        if (begin() == end()) return;
        iterator first = begin();
        iterator last = end();
        iterator next = first;
        while (++next != last) {
            if (p(*next,*first))
                erase(next);
            else
                first = next;
            next = first;
        }
    }
    
    template <typename T, typename Alloc>
    void list<T,Alloc>::sort() {
        if (end_ptr->next == end_ptr || link_type(end_ptr->next)->next== end_ptr) return;
        
        list<T,Alloc> carry;
        list<T,Alloc> counter[64];
        int fill = 0;
        while (!empty()) {
            carry.splice(carry.begin(),*this,begin());
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill) ++fill;
        }
        
        for (int i = 1; i < fill; ++i) {
            counter[i].merge(counter[i-1]);
        }
        swap(counter[fill-1]);
    }
    
    template <typename T, typename Alloc>
    template <typename Compare>
    void list<T,Alloc>::sort(Compare comp) {
        if (end_ptr->next == end_ptr || link_type(end_ptr->next)->next== end_ptr) return;
        
        list<T,Alloc> carry;
        list<T,Alloc> counter[64];
        int fill = 0;
        while (!empty()) {
            carry.splice(carry.begin(),*this,begin());
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry, comp);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill) ++ fill;
        }
        
        for (int i = 1; i < fill; ++i) {
            counter[i].merge(counter[i-1], comp);
        }
        swap(counter[fill-1]);
    }
    
    template < class t, class alloc >
    bool operator==( const list<t,alloc>& lhs, const list<t,alloc>& rhs ) {
        if ( lhs.size() == rhs.size() ) {
            auto ite1 = lhs.begin();
            auto ite2 = rhs.begin();
            for (typename vector<t,alloc>::size_type i = 0; i != lhs.size(); ++i) {
                if (*ite1 != *ite2) return false;
                else                ++ite1; ++ite2;
            }
            return true;
        }
        else
            return false;
    }
    
// end of namespace
}

#endif
