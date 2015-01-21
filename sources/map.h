//
//  map.h
//  test
//
//  Created by deng on 15/1/21.
//  Copyright (c) 2015年 deng. All rights reserved.
//

#ifndef test_map_h
#define test_map_h

#include "rb_tree.h"
#include "pair.h"

namespace TinySTL {
    template <class Key, class T,
              class Compare = TinySTL::less<Key>,
              class Alloc = TinySTL::alloc>
    class map {
    public:
        // typedefs
        typedef Key key_type;
        typedef T data_type;
        typedef std::pair<Key, T> value_type;
        typedef Compare key_compare;
        
        class value_compare : public std::binary_function<value_type, value_type, bool> {
            friend class map<Key, T, Compare, Alloc>;
        protected:
            Compare comp;
            value_compare(Compare c) : comp(c) {}
        public:
            bool operator() (const value_type& x, const value_type& y) {
                return comp(x.first,y.first);
            }
        };
    private:
        typedef rb_tree<Key, value_type, TinySTL::select1st<value_type>, key_compare, Alloc> rep_type;
        rep_type tree;
    public:
        typedef typename rep_type::pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::reference reference;
        typedef typename rep_type::const_reference const_reference;
        typedef typename rep_type::iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::size_type size_type;
        typedef typename rep_type::difference_type difference_type;
        // constructors
        map() : tree(Compare()) {}
        explicit map(const Compare& comp) : tree(comp) {}
        
        template <class InputIterator>
        map(InputIterator first, InputIterator last, const Compare& comp = Compare()) : tree(comp) {
            tree.insert_unique(first,last);
        }
        map(const std::initializer_list<value_type>& ilist) : tree(Compare()) {
            tree.insert_unique(ilist.begin(),ilist.end());
        }
        
        map(const map& x, const Compare& comp = Compare()) : tree(comp) {
            tree = x.tree;
        }
        map(map&& x, const Compare& comp = Compare()) : tree(comp) {
            tree = std::move(x.tree);
        }
        
        map& operator=(const map& x) {
            tree = x.tree;
            return *this;
        }
        map& operator=(map&& x) {
            tree = std::move(x.tree);
            return *this;
        }
        // accessors
        key_compare key_comp() const { return tree.key_comp();}
        value_compare value_comp() const { return value_compare(tree.key_comp());}
        iterator begin() { return tree.begin();}
        const_iterator begin() const { return tree.begin();}
        const_iterator cbegin() const { return tree.cbegin();}
        iterator end() { return tree.end();}
        const_iterator end() const { return tree.end();}
        const_iterator cend() const { return tree.cend();}
        
        bool empty() const { return tree.empty();}
        size_type size() const { return tree.size();}
        
        T& operator[] (const key_type& key) {
            return (*((insert( value_type( key,T() ) ) ).first)).second;
        }
        
        std::pair<iterator, bool> insert(const value_type& x) {
            return tree.insert_unique(x);
        }
        
        template <class InputIterator>
        void insert(InputIterator first, InputIterator last) {
            tree.insert_unique(first,last);
        }
        
        void insert( std::initializer_list<value_type> ilist ) {
            tree.insert_unique(ilist.begin(), ilist.end());
        }
        
        iterator find( const Key& key ) { return tree.find(key);}
        const_iterator find( const Key& key ) const { return tree.find(key);}
        
        iterator erase( const_iterator pos ) { return (tree.deleteNode((*pos).first)).first;}
        size_type erase( const key_type& key ) { return tree.deleteNode(key).second ? 1:0;}
        
        
        iterator lower_bound (const key_type& key) { return tree.lower_bound(key);}
        iterator upper_bound (const key_type& key) { return tree.upper_bound(key);}
        const_iterator lower_bound (const key_type& key) const { return tree.lower_bound(key);}
        const_iterator upper_bound (const key_type& key) const { return tree.upper_bound(key);}
        
        bool operator==(const map& rhs) {
            return tree==rhs.tree;
        }
        bool operator!=(const map& rhs) {
            return tree!=rhs.tree;
        }
        // End of map
    };
    
    // End of namespace
}

#endif
