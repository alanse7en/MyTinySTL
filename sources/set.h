//
//  set.h
//  test
//
//  Created by deng on 15/1/15.
//  Copyright (c) 2015年 deng. All rights reserved.
//

#ifndef test_set_h
#define test_set_h

#include "rb_tree.h"

namespace TinySTL {
    template < class Key,
               class Compare = TinySTL::less<Key>,
               class Alloc = TinySTL::alloc >
    class set {
    public:
        typedef Key key_type;
        typedef Key value_type;
        typedef Compare key_compare;
        typedef Compare value_compare;
        
    private:
        typedef TinySTL::rb_tree<key_type, value_type, TinySTL::identity<value_type>, key_compare, Alloc> rep_type;
        rep_type tree;
        
    public:
        typedef typename rep_type::const_pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::const_reference reference;
        typedef typename rep_type::const_reference const_reference;
        typedef typename rep_type::const_iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::size_type size_type;
        typedef typename rep_type::difference_type difference_type;
        
        set() : tree(Compare()) {}
        explicit set( const Compare& comp ) : tree(comp) {}
        
        template< class InputIterator >
        set( InputIterator first, InputIterator last,
            const Compare& comp = Compare() )
        : tree(comp) { tree.insert_unique(first,last);}
        
        set(const set& other, const Compare& comp = Compare() )
        : tree(comp) {
            tree = other.tree;
        }
        
        set(set&& other, const Compare& comp = Compare() )
        : tree(comp) {
            tree = std::move(other.tree);
        }
        
        set( std::initializer_list<key_type> ilist,
             const Compare& comp = Compare() )
        : tree(comp) { tree.insert_unique(ilist.begin(),ilist.end());}
        
        set& operator=( const set& other ) {
            tree = other.tree;
            return *this;
        }
        
        set& operator=( set&& other ) {
            tree = std::move(other.tree);
            return *this;
        }
        
        set& operator=(const std::initializer_list<value_type>& ilist ) {
            tree = ilist;
        }
        
        iterator begin() { return tree.cbegin();}
        const_iterator begin() const { return tree.cbegin();}
        const_iterator cbegin() const { return tree.cbegin();}
        
        iterator end() { return tree.cend();}
        const_iterator end() const { return tree.cend();}
        const_iterator cend() const { return tree.cend();}
        
        bool empty() const { return tree.empty();}
        size_type size() const { return tree.size();}
        size_type max_size() const { return tree.max_size();}
        
        void clear() { tree.clear();}
        
        std::pair<iterator,bool> insert( const value_type& value ) {
            return tree.insert_unique(value);
        }
        
        template< class InputIterator >
        void insert( InputIterator first, InputIterator last ) {
            tree.insert_unique(first, last);
        }
        
        void insert( std::initializer_list<value_type> ilist ) {
            tree.insert_unique(ilist.begin(), ilist.end());
        }
        
        iterator find( const Key& key ) { return tree.find(key);}
        const_iterator find( const Key& key ) const { return tree.find(key);}
        
        iterator erase( const_iterator pos ) { return (tree.deleteNode(*pos)).first;}
        size_type erase( const key_type& key ) { return tree.deleteNode(key).second ? 1:0;}
        
        const_iterator lower_bound (const key_type& key) const { return tree.lower_bound(key);}
        const_iterator upper_bound (const key_type& key) const { return tree.upper_bound(key);}
        
        bool operator==(const set& rhs) { return tree==rhs.tree;};
        // End of class set
    };
    // End of namespace
}

#endif
