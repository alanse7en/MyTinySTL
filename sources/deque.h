//
//  deque.h
//  test
//
//  Created by deng on 14/12/16.
//  Copyright (c) 2014年 deng. All rights reserved.
//

#ifndef test_deque_h
#define test_deque_h

#include <cstddef>
#include "allocator.h"
#include "algorithms.h"
#include "stl_iterator.h"

namespace TinySTL {
    template < class T, size_t BufSize = 0>
    struct __deque_iterator {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef std::ptrdiff_t              difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
        typedef size_t                      size_type;
        typedef T**                         map_pointer;
        
        static size_t buffer_size() {
            return BufSize != 0 ? BufSize
            : ( sizeof(T) < 512 ? size_type(512/sizeof(T) ): size_type(1) );
        }
        
        T* curr;
        T* first;
        T* last;
        map_pointer node;
        
        void print_debug() {
            std::cerr << curr-first << std::endl;
        }
        
        void set_node(map_pointer new_node) {
            node = new_node;
            first = *new_node;
            last = first + difference_type( buffer_size() );
        }
        
        reference operator*() const { return *curr; }
        pointer operator->() const { return &(operator*() ); }
        difference_type operator-( const __deque_iterator& x) const {
            return difference_type( buffer_size() ) * (node - x.node - 1) +
                (curr - first) + (x.last - x.curr);
        }
        __deque_iterator& operator++() {
            ++curr;
            if ( curr == last ) {
                set_node(node+1);
                curr = first;
            }
            return *this;
        }
        __deque_iterator operator++( int ) {
            __deque_iterator tmp = *this;
            ++*this;
            return tmp;
        }
        __deque_iterator& operator--() {
            if ( curr == first ) {
                set_node(node-1);
                curr = last;
            }
            --curr;
            return *this;
        }
        __deque_iterator operator--( int ) {
            __deque_iterator tmp = *this;
            --*this;
            return tmp;
        }
        __deque_iterator& operator+=( difference_type n ) {
            difference_type offset = n + ( curr - first );
            if ( offset >= 0 && offset < difference_type(buffer_size()))
                curr += n;
            else {
                difference_type node_offset =
                    offset > 0 ? offset/ difference_type(buffer_size())
                    : -difference_type((-offset - 1)/buffer_size()) - 1;
                set_node(node + node_offset);
                curr = first + (offset - node_offset*difference_type(buffer_size()));
            }
            return *this;
        }
        __deque_iterator& operator+( difference_type n ) {
            __deque_iterator tmp = *this;
            return tmp += n;
        }
        __deque_iterator& operator-=( difference_type n ) { return *this += -n;}
        __deque_iterator& operator-( difference_type n ) {
            __deque_iterator tmp = *this;
            return tmp -= n;
        }
        reference operator[]( difference_type n ) const { return *(*this + n);}
        bool operator==( const __deque_iterator& x ) const { return curr == x.curr;}
        bool operator!=( const __deque_iterator& x ) const { return curr != x.curr;}
        bool operator<( const __deque_iterator& x) const {
            return ( node == x.node ) ? (curr < x.curr) : (node < x.node);
        }
    };
    
    template < class T, class Alloc = alloc , size_t BufSize = 0 >
    class deque{
    public:
        typedef T                             value_type;
        typedef allocator< T, Alloc >         allocator_type;
        typedef T*                            pointer;
        typedef const T*                      const_poniter;
        typedef std::size_t                   size_type;
        typedef std::ptrdiff_t                difference_type;
        typedef T&                            reference;
        typedef const T&                      const_reference;
        typedef __deque_iterator<T,BufSize>   iterator;
        typedef const iterator                const_iterator;
        
    protected:
        typedef pointer* map_pointer;
        
        iterator         start;
        iterator         finish;
        map_pointer      map;
        size_type        map_size;
        
        typedef allocator_type data_allocator;
        typedef allocator<pointer,Alloc> map_allocator;
        
        void create_map_and_node(size_type count) {
            size_type num_nodes = count/(iterator::buffer_size()) + 1;
            map_size = std::max(size_type(8), num_nodes+2);
            map = map_allocator::allocate(map_size);
            
            map_pointer node_start = map + (map_size - num_nodes)/2;
            map_pointer node_finish = node_start + num_nodes - 1;
            map_pointer curr;
            try {
                for (curr = node_start; curr <= node_finish; ++curr) {
                    *curr = data_allocator::allocate(iterator::buffer_size());
                }
            } catch (...) {
                for (; curr >= node_start; --curr)
                    data_allocator::deallocate(*curr,iterator::buffer_size());
            }
            
            start.set_node(node_start);
            finish.set_node(node_finish);
            start.curr = start.first;
            finish.curr = finish.first + count % iterator::buffer_size();
        }
        
        void fill_initialize( size_type count, const T& value) {
            create_map_and_node(count);
            map_pointer curr;
            try {
                for (curr = start.node; curr != finish.node; ++curr)
                    TinySTL::uninitialized_fill(*curr, *curr+iterator::buffer_size(), value);
                TinySTL::uninitialized_fill(finish.first, finish.curr, value);
            } catch (...) {
                for (; curr >= start.node; --curr)
                    TinySTL::__destroy(*curr,*curr+iterator::buffer_size());
            }
            
        }
        
        void deque_aux( size_type count ,const T& value, std::true_type) {
            fill_initialize(count, value);
        }
        
        template <class InputIterator>
        void deque_aux( InputIterator first, InputIterator last, std::false_type) {
            size_type num_elements = last - first;
            create_map_and_node(num_elements);
            TinySTL::uninitialized_copy(first, last, start);
        }
        
        void destroy_and_deallocate() {
            for (map_pointer curr = start.node; curr != finish.node; ++curr) {
                __destroy(*curr, *curr+iterator::buffer_size());
                data_allocator::deallocate(*curr,iterator::buffer_size());
            }
            __destroy(finish.first,finish.curr);
            data_allocator::deallocate(finish.first,iterator::buffer_size());
            
            __destroy(map, map+map_size);
            map_allocator::deallocate(map,map_size);
        }
        
        template < class... Args >
        void emplace_back_aux(Args&&... args);
        
        template < class... Args >
        void emplace_front_aux(Args&&... args);
        
        void reallocate_map(size_type nodes_to_add, bool add_at_front);
        
        iterator erase_aux(iterator pos,std::true_type) {
            iterator next = pos;
            ++next;
            difference_type num_front = pos - start;
            difference_type num_back = finish - pos;
            if (num_front < num_back) {
                TinySTL::move_backward(start, pos, next);
                pop_front();
            }
            else {
                TinySTL::move(next, finish, pos);
                pop_back();
            }
            
            return start+num_front;
        }
        iterator erase_aux(iterator pos,std::false_type) {
            iterator next = pos;
            ++next;
            difference_type num_front = pos - start;
            difference_type num_back = finish - pos;
            if (num_front < num_back) {
                TinySTL::copy_backward(start, pos, next);
                pop_front();
            }
            else {
                TinySTL::copy(next, finish, pos);
                pop_back();
            }
            
            return start+num_front;
        }
        
        iterator erase_aux(iterator first, iterator last, std::true_type) {
            if (first==start && last==finish) {
                clear();
                return finish;
            }
            else {
                difference_type num_to_delte = last-first;
                difference_type num_front = first-start;
                difference_type num_back = size()-num_front-num_to_delte;
                if (num_front<num_back) {
                    TinySTL::move_backward(start, first, last);
                    __destroy(start.curr, start.curr+num_to_delte);
                    for (map_pointer node=start.node; node < (start+num_to_delte).node; ++node)
                        data_allocator::deallocate(*node,iterator::buffer_size());
                    start = start+num_to_delte;
                }
                else {
                    TinySTL::move(last, finish, first);
                    iterator new_finish = finish-num_to_delte;
                    __destroy(new_finish.curr,finish.curr);
                    for (map_pointer node=new_finish.node+1; node<=finish.node; ++node)
                        data_allocator::deallocate(*node,iterator::buffer_size());
                    finish = new_finish;
                }
                return start+num_to_delte+num_front;
            }
        }
        
        iterator erase_aux(iterator first, iterator last, std::false_type) {
            if (first==start && last==finish) {
                clear();
                return finish;
            }
            else {
                difference_type num_to_delte = last-first;
                difference_type num_front = first-start;
                difference_type num_back = size()-num_front-num_to_delte;
                if (num_front<num_back) {
                    TinySTL::copy_backward(start, first, last);
                    __destroy(start.curr, start.curr+num_to_delte);
                    for (map_pointer node=start.node; node < (start+num_to_delte).node; ++node)
                        data_allocator::deallocate(*node,iterator::buffer_size());
                    start = start+num_to_delte;
                }
                else {
                    TinySTL::copy(last, finish, first);
                    iterator new_finish = finish-num_to_delte;
                    __destroy(new_finish.curr,finish.curr);
                    for (map_pointer node=new_finish.node; node<=finish.node; ++node)
                        data_allocator::deallocate(*node,iterator::buffer_size());
                    finish = new_finish;
                }
                return start+num_to_delte+num_front;
            }
        }
        
        template <class... Args>
        iterator emplace_aux(iterator pos, std::true_type, Args&&... args)  {
            value_type tmp(std::forward<Args>(args)...);
            difference_type index = pos-start;
            
            if (index < size()/2) {
                push_front(front());
                iterator front1 = start+1;
                iterator front2 = front1+1;
                pos = start+index;
                iterator pos1 = pos+1;
                TinySTL::move(front2, pos1, front1);
            }
            else {
                push_back(back());
                iterator back1 = finish-1;
                iterator back2 = back1-1;
                pos = start+index;
                TinySTL::move_backward(pos, back2, back1);
            }
            
            *pos = tmp;
            return pos;
        }
        
        template <class... Args>
        iterator emplace_aux(iterator pos, std::false_type, Args&&... args) {
            value_type tmp(std::forward<Args>(args)...);
            difference_type index = pos-start;
            
            if (index < size()/2) {
                push_front(front());
                iterator front1 = start+1;
                iterator front2 = front1+1;
                pos = start+index;
                iterator pos1 = pos+1;
                TinySTL::copy(front2, pos1, front1);
            }
            else {
                push_back(back());
                iterator back1 = finish-1;
                iterator back2 = back1-1;
                pos = start+index;
                TinySTL::copy_backward(pos, back2, back1);
            }
            
            *pos = tmp;
            return pos;
        }
        
        iterator insert_aux( iterator pos, size_type count, const T& value, std::true_type) {
            difference_type index = pos-start;
            size_type num_empty_front = start.curr - start.first;
            size_type num_empty_back = finish.last - finish.curr;
            if (index < size()/2) {
                if (num_empty_front <= count) {
                    size_type nodes_to_add  = (count-num_empty_front)/iterator::buffer_size()+1;
                    if (nodes_to_add > (start.node-map)) {
                        difference_type offset_front = start.curr-start.first;
                        difference_type offset_back = finish.curr-finish.first;
                        reallocate_map(nodes_to_add-(start.node-map), true);
                        start.curr = start.first+offset_front;
                        finish.curr = finish.first+offset_back;
                        pos = start+index;
                    }
                    for (map_pointer node = start.node-nodes_to_add; node < start.node; ++node) {
                        *node = data_allocator::allocate(iterator::buffer_size());
                    }
                }
                
                if (count <= index) {
                    pos = start+index;
                    TinySTL::uninitialized_copy(start, start+count, start-count);
                    start = start-count;
                    iterator front1 = start+count;
                    iterator front2 = front1+count;
                    TinySTL::copy(front2, pos, front1);
                    iterator position = start+index;
                    std::fill_n(position, count, value);
                    return position;
                }
                else {
                    TinySTL::uninitialized_copy(start, pos, start-count);
                    start = start-count;
                    iterator position = start+index;
                    TinySTL::uninitialized_fill_n(position, count-index, value);
                    iterator front1 = start+count;
                    std::fill_n(front1, index, value);
                    return position;
                }
            }
            else {
                index = finish-pos;
                if (num_empty_back <= count) {
                    size_type nodes_to_add  = (count-num_empty_back)/iterator::buffer_size()+1;
                    if (nodes_to_add > (map_size-(finish.node-map+1))) {
                        difference_type offset_front = start.curr-start.first;
                        difference_type offset_back = finish.curr-finish.first;
                        reallocate_map(nodes_to_add-(map_size-(finish.node-map+1)), false);
                        start.curr = start.first+offset_front;
                        finish.curr = finish.first+offset_back;
                    }
                    for (map_pointer node = finish.node+1; node <= finish.node+nodes_to_add; ++node) {
                        *node = data_allocator::allocate(iterator::buffer_size());
                    }
                }
                
                if (count <= index) {
                    TinySTL::uninitialized_copy(finish-count, finish, finish);
                    finish = finish+count;
                    iterator position = finish-index;
                    TinySTL::copy_backward(pos, finish-2*count, finish-count);
                    std::fill_n(pos, count, value);
                    return position;
                }
                else {
                    iterator position = finish-index;
                    TinySTL::uninitialized_copy(position, finish, position+count);
                    finish = finish+count;
                    TinySTL::uninitialized_fill_n(finish-count, count-index, value);
                    std::fill_n(position, index, value);
                    return position;
                }
            }
        }
        
        template< class InputIterator >
        iterator insert_aux( iterator pos, InputIterator first, InputIterator last, std::false_type) {
            difference_type index = pos-start;
            size_type count = __distance(first, last);
            size_type num_empty_front = start.curr - start.first;
            size_type num_empty_back = finish.last - finish.curr;
            if (index < size()/2) {
                if (num_empty_front <= count) {
                    size_type nodes_to_add  = (count-num_empty_front)/iterator::buffer_size()+1;
                    if (nodes_to_add > (start.node-map)) {
                        difference_type offset_front = start.curr-start.first;
                        difference_type offset_back = finish.curr-finish.first;
                        reallocate_map(nodes_to_add-(start.node-map), true);
                        start.curr = start.first+offset_front;
                        finish.curr = finish.first+offset_back;
                        pos = start + index;
                    }
                    for (map_pointer node = start.node-nodes_to_add; node < start.node; ++node) {
                        *node = data_allocator::allocate(iterator::buffer_size());
                    }
                }
                
                if (count <= index) {
                    TinySTL::uninitialized_copy(start, start+count, start-count);
                    start = start-count;
                    iterator front1 = start+count;
                    iterator front2 = front1+count;
                    TinySTL::copy(front2, pos, front1);
                    iterator position = start+index;
                    TinySTL::copy(first, last, position);
                    return position;
                }
                else {
                    TinySTL::uninitialized_copy(start, pos, start-count);
                    start = start-count;
                    iterator position = start+index;
                    TinySTL::uninitialized_copy(first, first+(count-index), position);
                    iterator front1 = start+count;
                    std::copy(first+(count-index), last, front1);
                    return position;
                }
            }
            else {
                index = finish-pos;
                if (num_empty_back <= count) {
                    size_type nodes_to_add  = (count-num_empty_back)/iterator::buffer_size()+1;
                    if (nodes_to_add > (map_size-(finish.node-map+1))) {
                        difference_type offset_front = start.curr-start.first;
                        difference_type offset_back = finish.curr-finish.first;
                        reallocate_map(nodes_to_add-(map_size-(finish.node-map+1)), false);
                        start.curr = start.first+offset_front;
                        finish.curr = finish.first+offset_back;
                    }
                    for (map_pointer node = finish.node+1; node <= finish.node+nodes_to_add; ++node) {
                        *node = data_allocator::allocate(iterator::buffer_size());
                    }
                }
                
                if (count <= index) {
                    TinySTL::uninitialized_copy(finish-count, finish, finish);
                    finish = finish+count;
                    TinySTL::copy_backward(pos, finish-2*count, finish-count);
                    TinySTL::copy(first, last, pos);
                    return pos;
                }
                else {
                    iterator position = finish+count-index;
                    TinySTL::uninitialized_copy(pos, finish, position);
                    finish = finish+count;
                    TinySTL::copy(first, first+index, pos);
                    TinySTL::uninitialized_copy(first+index, last, finish-count);
                    return pos;
                }
            }
        }

        
    public:
        /* Member functions */
        // Constructors and destructor
        deque() :start(), finish(), map(0), map_size(0) { deque_aux(0, T(), std::true_type());}
        explicit deque( size_type count, const T& value )
        : start(), finish(), map(0), map_size(0) {
            deque_aux(count, value, std::true_type());
        }
        explicit deque( size_type count )
        : start(), finish(), map(0), map_size(0) {
            deque_aux(count, T(), std::true_type());
        }
        template< class InputIterator >
        deque( InputIterator first, InputIterator last )
        : start(), finish(), map(0), map_size(0) {
            deque_aux(first, last, std::is_integral<InputIterator>());
        }
        deque( const deque& other )
        : start(), finish(), map(0), map_size(0) {
            deque_aux(other.begin(), other.end(), std::false_type());
        }
        deque( deque&& other )
        : start(), finish(), map(0), map_size(0) {
            std::swap(start, other.start);
            std::swap(finish, other.finish);
            std::swap(map, other.map);
            std::swap(map_size, other.map_size);
        }
        deque( std::initializer_list<T> init )
        : start(), finish(), map(0), map_size(0) {
            deque_aux(init.begin(), init.end(), std::false_type());
        }
        ~deque() {
            destroy_and_deallocate();
        }
        
        
        // Operator=
        deque& operator=( const deque& other ) {
            destroy_and_deallocate();
            deque_aux(other.begin(), other.end(), std::false_type());
            return *this;
        }
        deque& operator=( deque&& other ) {
            destroy_and_deallocate();
            start = iterator();
            finish = iterator();
            map = 0;
            map_size = 0;
            std::swap(start, other.start);
            std::swap(finish, other.finish);
            std::swap(map, other.map);
            std::swap(map_size, other.map_size);
            return *this;
        }
        deque& operator=( std::initializer_list<T> ilist ) {
            destroy_and_deallocate();
            deque_aux(ilist.begin(), ilist.end(), std::false_type());
            return *this;
        }
        
        
        // Assign
        void assign( size_type count, const T& value ) {
            destroy_and_deallocate();
            deque_aux(count, value, std::true_type());
        }
        template< class InputIterator >
        void assign( InputIterator first, InputIterator last ) {
            destroy_and_deallocate();
            deque_aux(first, last, std::is_integral<InputIterator>());
        }
        void assign( std::initializer_list<T> ilist ) {
            destroy_and_deallocate();
            deque_aux(ilist.begin(), ilist.end(), std::false_type());
        }
        
        
        // Get allocator
        allocator_type get_allocator() const { return allocator_type(); }
        // Element access
        reference at( size_type pos ) { return *(start+pos);}
        const_reference at( size_type pos ) const { return *(iterator(start)+pos);}
        reference operator[]( size_type pos ) { return *(start+pos);}
        const_reference operator[]( size_type pos ) const { return *(iterator(start)+pos);}
        reference front() { return *start;}
        const_reference front() const { return *start;}
        reference back() { return *(finish-1);}
        const_reference back() const { return *(iterator(finish)-1);}
        
        
        // Iterators
        iterator begin() { return start;}
        const_iterator begin() const { return start;}
        const_iterator cbegin() const { return start;}
        
        iterator end() { return finish;}
        const_iterator end() const { return finish;}
        const_iterator cend() const { return finish;}
        
        
        // Capacity
        bool empty() const { return finish == start;}
        size_type size() const { return finish - start;}
        size_type max_size() const { return size_type(-1);}
        void shrink_to_fit();
        
        
        // Modifiers
        void clear() {
            for (map_pointer node = start.node+1;node < finish.node;++node)
                __destroy(*node, *node+iterator::buffer_size());
            
            if (start.node != finish.node) {
                __destroy(start.curr, start.last);
                __destroy(finish.curr, finish.last);
                data_allocator::deallocate(finish.first,iterator::buffer_size());
            }
            else
                __destroy(start.curr,finish.curr);
            
            finish = start;
        }
        
        iterator insert( const_iterator pos, const T& value ) {
            return emplace(pos, value);
        }
        
        iterator insert( const_iterator pos, T&& value ) {
            return emplace(pos, std::forward<T>(value));
        }
        
        iterator insert( const_iterator pos, size_type count, const T& value ) {
            return insert_aux(pos, count, value, std::true_type());
        }
        
        template< class InputIterator >
        iterator insert( const_iterator pos, InputIterator first, InputIterator last ) {
            return insert_aux(pos, first, last, std::is_integral<InputIterator>());
        }
        
        iterator insert( const_iterator pos, std::initializer_list<T> ilist ) {
            return insert_aux(pos, ilist.begin(), ilist.end(), std::false_type());
        }
        
        template< class... Args >
        iterator emplace( const_iterator pos, Args&&... args ) {
            if (pos == start) {
                emplace_front(std::forward<Args>(args)...);
                return start;
            }
            else if (pos == finish) {
                emplace_back(std::forward<Args>(args)...);
                return finish-1;
            }
            else
                return emplace_aux(pos, std::is_move_assignable<T>(), std::forward<Args>(args)...);
        }
        
        iterator erase( const_iterator pos ) {
            return erase_aux(pos, std::is_move_assignable<T>());
        }
        iterator erase( const_iterator first, const_iterator last ) {
            return erase_aux(first, last, std::is_move_assignable<T>());
        }
        
        void push_back( const T& value ) {
            emplace_back(value);
        }
        void push_back( T&& value ) {
            emplace_back(std::forward<T>(value));
        }
        
        template< class... Args >
        void emplace_back( Args&&... args ) {
            if (finish.curr != finish.last - 1) {
                __construct(finish.curr, std::forward<Args>(args)...);
                ++finish;
            }
            else
                emplace_back_aux(std::forward<Args>(args)...);
        }
        
        void pop_back() {
            if ( finish.curr != finish.first) {
                --finish;
                __destroy(finish.curr);
            }
            else {
                data_allocator::deallocate(finish.first,iterator::buffer_size());
                finish.set_node(finish.node-1);
                finish.curr = finish.last-1;
                __destroy(finish.curr);
            }
        }
        
        void push_front( const T& value ) {
            emplace_front(value);
        }
        void push_front( T&& value ) {
            emplace_front(std::forward<T>(value));
        }
        
        template< class... Args >
        void emplace_front( Args&&... args ) {
            if (start.curr != start.first) {
                __construct(start.curr-1, std::forward<Args>(args)...);
                --start;
            }
            else
                emplace_front_aux(std::forward<Args>(args)...);
        }
        
        void pop_front() {
            if ( start.curr != start.last-1) {
                __destroy(start.curr);
                ++start;
            }
            else {
                __destroy(start.curr);
                data_allocator::deallocate(start.first,iterator::buffer_size());
                start.set_node(start.node+1);
                start.curr = start.first;
            }
        }
        
        void resize( size_type count ) {
            if (count < size()) {
                size_type num_to_delete = size()-count;
                erase(finish-num_to_delete, finish);
            }
            else if (count > size()) {
                insert(finish, count-size(), T());
            }
        }
        void resize( size_type count, const value_type& value ) {
            if (count < size()) {
                size_type num_to_delete = size()-count;
                erase(finish-num_to_delete, finish);
            }
            else if (count > size()) {
                insert(finish, count-size(), value);
            }
        }
        
        void swap( deque& other ) {
            std::swap(start, other.start);
            std::swap(finish, other.finish);
            std::swap(map, other.map);
            std::swap(map_size, other.map_size);
        }
        
        
        /* Non-member functions */
        template < class t, class alloc >
        friend bool operator==( const deque< t, alloc >& lhs, const deque< t, alloc >& rhs );
        template < class t, class alloc >
        friend bool operator!=( const deque< t, alloc >& lhs, const deque< t, alloc >& rhs );
        template < class t, class alloc >
        friend bool operator<=( const deque< t, alloc >& lhs, const deque< t, alloc >& rhs );
        template < class t, class alloc >
        friend bool operator<( const deque< t, alloc >& lhs, const deque< t, alloc >& rhs );
        template < class t, class alloc >
        friend bool operator>=( const deque< t, alloc >& lhs, const deque< t, alloc >& rhs );
        template < class t, class alloc >
        friend bool operator>( const deque< t, alloc >& lhs, const deque< t, alloc >& rhs );
    };
    
    template < class T, class Alloc, size_t BufSize >
    void deque<T,Alloc,BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
        size_type old_num_nodes = finish.node-start.node+1;
        size_type new_num_nodes = old_num_nodes+nodes_to_add;
        
        map_pointer new_node_start;
        if (map_size > 2*new_num_nodes) {
            new_node_start = map + (map_size-new_num_nodes)/2 + (add_at_front?nodes_to_add:0);
            if (new_node_start < start.node)
                TinySTL::copy(start.node, finish.node+1, new_node_start);
            else
                TinySTL::copy_backward(start.node, finish.node+1, new_node_start+old_num_nodes);
        }
        else {
            size_type new_map_size = map_size + std::max(map_size,nodes_to_add) + 2;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_node_start = new_map + (new_map_size - new_num_nodes)/2 + (add_at_front?nodes_to_add:0);
            move(start.node, finish.node+1, new_node_start);
            map_allocator::deallocate(map,map_size);
            map = new_map;
            map_size = new_map_size;
        }
        start.set_node(new_node_start);
        finish.set_node(new_node_start+old_num_nodes-1);
    }
    
    template < class T, class Alloc, size_t BufSize>
    template < class... Args>
    void deque<T,Alloc,BufSize>::emplace_back_aux(Args&&...args) {
        if ( ( map_size-( finish.node-map ) )-1 < 1)
            reallocate_map(1, false);
        *(finish.node+1) = data_allocator::allocate(iterator::buffer_size());
        try {
            __construct(finish.curr, std::forward<Args>(args)...);
            ++finish;
        } catch (...) {
            data_allocator::deallocate(*(finish.node+1));
        }
        
    }
    
    template < class T, class Alloc, size_t BufSize>
    template < class... Args>
    void deque<T,Alloc,BufSize>::emplace_front_aux(Args&&...args) {
        if ( start.node-map < 1)
            reallocate_map(1, true);
        *(start.node-1) = data_allocator::allocate(iterator::buffer_size());
        try {
            __construct(start.curr-1, std::forward<Args>(args)...);
            --start;
        } catch (...) {
            start.set_node(start.node+1);
            start.curr = start.first;
            data_allocator::deallocate(*(start.node-1));
            throw;
        }
        
    }
    
    template < class t, class alloc >
    bool operator==( const deque< t, alloc >& lhs, const deque< t, alloc >& rhs ) {
        if (lhs.size() == rhs.size()) {
            for (size_t i = 0; i < lhs.size(); ++i)
                if (lhs[i] != rhs[i]) return false;
            return true;
        }
        return false;
    }
    
    template < class t, class alloc >
    bool operator!=( const deque< t, alloc >& lhs, const deque< t, alloc >& rhs ) {
        return !(lhs==rhs);
    }
    
    
// end of namespace
}
#endif
