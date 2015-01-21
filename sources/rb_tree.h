//
//  rb_tree.h
//  test
//
//  Created by deng on 15/1/7.
//  Copyright (c) 2015年 deng. All rights reserved.
//

#ifndef test_rb_tree_h
#define test_rb_tree_h

//
//  RBTree.h
//  test
//
//  Created by deng on 14/12/27.
//  Copyright (c) 2014年 deng. All rights reserved.
//
//  I implement red black tree as a left leaning red black tree to reduce the complexity of insertation and
//  deletion. Further details can be found at "https://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf".
//

#ifndef test_RBTree_h
#define test_RBTree_h

#include <cstddef>
#include <utility>
#include "functional.h"

namespace TinySTL {
    typedef bool __rb_tree_color_type;
    const __rb_tree_color_type __rb_tree_red = false;
    const __rb_tree_color_type __rb_tree_black = true;
    
    /* Node structure of red black tree */
    template < class Value >
    struct __rb_tree_node {
        typedef __rb_tree_color_type color_type;
        typedef __rb_tree_node<Value>* link_type;
        
        color_type color;
        link_type  parent;
        link_type  left;
        link_type  right;
        Value      value;
        
        static link_type minimum(link_type x) {
            while (x->left != NULL)
                x = x->left;
            return x;
        }
        
        static link_type maximum(link_type x) {
            while (x->right != NULL)
                x = x->right;
            return x;
        }
    };
    
    /* Base iterator of red black tree */
    template < class Value>
    struct __rb_tree_iterator {
        typedef typename __rb_tree_node<Value>::link_type link_type;
        typedef TinySTL::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;
        
        link_type node;
        
        void increment() {
            if (node->right != 0) {
                node = node->right;
                while (node->left != 0)
                    node = node->left;
            }
            else {
                link_type y = node->parent;
                while (node == y->right) {
                    node = y;
                    y = y->parent;
                }
                if (node->right != y)
                    node = y;
            }
        }
        
        void decrement() {
            if (node->color == __rb_tree_red &&
                node->parent->parent == node)
                node = node->right;
            else if (node->left != 0) {
                node = node->left;
                while (node->right != 0)
                    node = node->right;
            }
            else {
                link_type y = node->parent;
                while (node == y->left) {
                    node = y;
                    y = y->parent;
                }
                node = y;
            }
        }
        
        typedef Value  value_type;
        typedef Value& reference;
        typedef Value* pointer;
        typedef __rb_tree_iterator<Value> iterator;
        
        __rb_tree_iterator() {}
        __rb_tree_iterator(link_type x) { node = x;}
        __rb_tree_iterator(const iterator& ite) { node = ite.node;}
        
        reference operator*() const { return node->value;}
        pointer operator->() const { return &(operator*());}
        
        iterator& operator++() { increment(); return *this;}
        iterator operator++(int) {
            iterator tmp = *this;
            increment();
            return tmp;
        }
        
        iterator& operator--() { decrement(); return *this;}
        iterator operator--(int) {
            iterator tmp = *this;
            decrement();
            return tmp;
        }
        
        bool operator==(const iterator& rhs) {
            return node==rhs.node;
        }
        
        bool operator!=(const iterator& rhs) {
            return node!=rhs.node;
        }
    };
    
    template < class Value>
    struct __rb_tree_const_iterator {
        typedef typename __rb_tree_node<Value>::link_type link_type;
        typedef TinySTL::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;
        
        link_type node;
        
        void increment() {
            if (node->right != 0) {
                node = node->right;
                while (node->left != 0)
                    node = node->left;
            }
            else {
                link_type y = node->parent;
                while (node == y->right) {
                    node = y;
                    y = y->parent;
                }
                if (node->right != y)
                    node = y;
            }
        }
        
        void decrement() {
            if (node->color == __rb_tree_red &&
                node->parent->parent == node)
                node = node->right;
            else if (node->left != 0) {
                node = node->left;
                while (node->right != 0)
                    node = node->right;
            }
            else {
                link_type y = node->parent;
                while (node == y->left) {
                    node = y;
                    y = y->parent;
                }
                node = y;
            }
        }
        
        
        typedef Value value_type;
        typedef const Value& reference;
        typedef const Value* pointer;
        typedef __rb_tree_iterator<Value> iterator;
        typedef __rb_tree_const_iterator<Value> const_iterator;
        
        __rb_tree_const_iterator() {}
        __rb_tree_const_iterator(link_type x) { node = x;}
        __rb_tree_const_iterator(const const_iterator& ite) { node = ite.node;}
        __rb_tree_const_iterator(const iterator& ite) { node = ite.node;}
        operator iterator() const { return iterator(node);}
        
        reference operator*() const { return node->value;}
        pointer operator->() const { return &(operator*());}
        
        const_iterator& operator++() { increment(); return *this;}
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            increment();
            return tmp;
        }
        
        const_iterator& operator--() { decrement(); return *this;}
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            decrement();
            return tmp;
        }
        
        bool operator==(const_iterator& rhs) {
            return node==rhs.node;
        }
        
        bool operator!=(const_iterator& rhs) {
            return node!=rhs.node;
        }
    };
    
    template < class Key, class Value, class KeyOfValue, class Compare = TinySTL::less<Key>, class Alloc = TinySTL::alloc>
    class rb_tree {
    protected:
        typedef void* void_pointer;
        typedef __rb_tree_node<Value> rb_tree_node;
        typedef TinySTL::allocator<rb_tree_node, Alloc> rb_tree_node_allocator;
        typedef __rb_tree_color_type color_type;
        
    public:
        typedef Key key_type;
        typedef Value value_type;
        typedef Value* pointer;
        typedef const Value* const_pointer;
        typedef Value& reference;
        typedef const Value& const_reference;
        typedef rb_tree_node* link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef __rb_tree_iterator<Value> iterator;
        typedef __rb_tree_const_iterator<Value> const_iterator;
        
    protected:
        link_type get_node() { return rb_tree_node_allocator::allocate();}
        
        void put_node(link_type p) { rb_tree_node_allocator::deallocate(p);}
        
        link_type create_node(const Value& x) {
            link_type tmp = get_node();
            try {
                __construct(&(tmp->value), x);
            } catch (...) {
                put_node(tmp);
            }
            return tmp;
        }
        
        link_type clone_node(link_type x) {
            link_type tmp = create_node(x->value);
            tmp->color = x->color;
            tmp->left = 0;
            tmp->right = 0;
            return tmp;
        }
        
        void destroy_node(link_type p) {
            __destroy(&(p->value));
            put_node(p);
        }
        
        size_type node_count;
        link_type header;
        Compare key_compare;
        
        link_type& root() const { return (link_type&)header->parent;}
        link_type& leftmost() const { return (link_type&)header->left;}
        link_type& rightmost() const { return (link_type&)header->right;}
        
        static link_type left(link_type x) {
            if (x==0)
                return x;
            return x->left;
        }
        static link_type right(link_type x) {
            if (x==0)
                return 0;
            return x->right;
        }
        static link_type parent(link_type x) {
            if (x==0)
                return 0;
            return x->parent;
        }
        static Value value(link_type x) { return x->value;}
        static Key key(link_type x) { return KeyOfValue()(value(x));}
        static color_type color(link_type x) {
            if (x==0)
                return __rb_tree_black;
            return x->color;
        }
        
        static bool isRed(link_type x) {
            
            if (color(x) == __rb_tree_black)
                return false;
            return true;
        }
        
        static link_type minimum(link_type x) {
            return (link_type) __rb_tree_node<Value>::minimum(x);
        }
        static link_type maximum(link_type x) {
            return (link_type) __rb_tree_node<Value>::maximum(x);
        }
        
    public:
        void clear() {
            iterator end(header);
            for (iterator ite = leftmost(); ite!=end; ++ite) {
                TinySTL::__destroy(&(*ite));
                put_node(ite.node);
                --node_count;
            }
            
            header->left = header;
            header->right = header;
            header->parent = 0;
        }
        
    private:
        link_type __copy(link_type x, link_type p);
        void __erase(link_type x);
        void init() {
            header = get_node();
            header->color = __rb_tree_red;
            
            root() = 0;
            leftmost() = header;
            rightmost() = header;
        }
        
        link_type& rotateLeft(link_type h) {
            link_type x = h->right;
            link_type z = x->left;
            h->right = z;
            if (z != 0)
                z->parent = h;
            x->left = h;
            h->parent = x;
            x->color = h->color;
            h->color = __rb_tree_red;
            return (link_type&)x;
        }
        
        link_type& rotateRight(link_type h) {
            link_type x = h->left;
            link_type z = x->right;
            h->left = z;
            if (z != 0)
                z->parent = h;
            x->right = h;
            h->parent = x;
            x->color = h->color;
            h->color = __rb_tree_red;
            return (link_type&)x;
        }
        
        link_type& colorFlip(link_type h) {
            h->color        = !h->color;
            h->left->color  = !h->left->color;
            h->right->color = !h->right->color;
            return (link_type&)h;
        }
        
        link_type& fixUp(link_type x) {
            if (color(x->right)==__rb_tree_red)
                x = rotateLeft(x);
            if (color(x->left)==__rb_tree_red && color(x->left->left)==__rb_tree_red)
                x = rotateRight(x);
            if (color(x->left)==__rb_tree_red && color(x->right)==__rb_tree_red)
                colorFlip(x);
            
            return (link_type&)x;
        }
        
        link_type& __insert(link_type x, const Value& v) {
            // x is insert point, h is the parent of x
            if (x == 0) {
                link_type tmp = create_node(v);
                tmp->color = __rb_tree_red;
                tmp->left  = 0;
                tmp->right = 0;
                return (link_type&)tmp;
                
            }
            
            
            Key k = KeyOfValue()(v);
            bool cmp = key_compare(k,key(x));
            if (k == key(x)) x->value = v;
            else if (cmp) {
                link_type tmp = __insert(x->left, v);
                x->left = tmp;
                tmp->parent = x;
            }
            else {
                link_type tmp = __insert(x->right, v);
                x->right = tmp;
                tmp->parent = x;
            }
            
            // Fix up rb tree
            return fixUp(x);
        }
        
        link_type& moveRedLeft(link_type h)
        {
            colorFlip(h);
            if (isRed(left(right(h) ) ) ) {
                h->right = rotateRight(h->right);
                h = rotateLeft(h);
                colorFlip(h);
            }
            return (link_type&)h;
        }
        
        link_type& moveRedRight(link_type h)
        {
            colorFlip(h);
            if (isRed(left(left(h) ) ) )
            {
                h = rotateRight(h);
                colorFlip(h);
            }
            return (link_type&)h;
        }
        
        link_type deleteMin(link_type h)
        {
            if (left(h) == 0) {
                destroy_node(h);
                return 0;
            }
            
            if (!isRed(left(h)) && !isRed(left(left(h))) )
                h = moveRedLeft(h);
            
            
            link_type tmp = deleteMin(h->left);
            h->left = tmp;
            if (tmp != 0)
                tmp->parent = h;
            
            return fixUp(h);
        }
        
        link_type deleteNode(link_type h, Key k)
        {
            bool cmp = key_compare(k,key(h));
            if (cmp) {
                if (!isRed(left(h)) && !isRed(left(left(h))))
                    h = moveRedLeft(h);
                link_type tmp =  deleteNode(h->left, k);
                h->left = tmp;
                if (tmp != 0)
                    tmp->parent = h;
            }
            else {
                if (isRed(left(h))) {
                    h = rotateRight(h);
//                    cmp = -1;
                }
                if (k == key(h) && (right(h) == 0)) {
                    destroy_node(h);
                    --node_count;
                    return 0;
                }
                if (!isRed(right(h)) && !isRed(left(right(h))))
                    h = moveRedRight(h);
                
                if (k == key(h)) {
                    link_type tmp = minimum(h->right);
                    h->value = tmp->value;
                    tmp = deleteMin(h->right);
                    h->right = tmp;
                    if (tmp != 0)
                        tmp->parent = h;
                }
                else {
                    link_type tmp = deleteNode(h->right, k);
                    h->right = tmp;
                    if (tmp != 0)
                        tmp->parent = h;
                }
            }
            
            return fixUp(h);
        }
        
        bool isSameTree(link_type p, link_type q) {
            if(p && q){
                if(p->value == q->value){
                    return (isSameTree(p->left, q->left) && isSameTree(p->right, q->right));
                }
                else
                    return false;
            }
            else if(p == NULL && q == NULL)
                return true;
            else
                return false;
        }
        
    public:
        rb_tree(const Compare& comp = Compare())
        : node_count(0), key_compare(comp) { init();}
        rb_tree(const rb_tree& x) : node_count(0), key_compare(x.key_compare) {
            init();
            insert_unique(x.cbegin(), x.cend());
        }
        ~rb_tree() {
            clear();
            put_node(header);
        }
        
        rb_tree& operator=(const rb_tree& x) {
            clear();
            insert_unique(x.cbegin(),x.cend());
            return *this;
        }
        rb_tree& operator=(rb_tree&& x) {
            clear();
            insert_unique(x.begin(),x.end() );
            x.clear();
            return *this;
        }
        
        rb_tree& operator=(const std::initializer_list<value_type>& ilist) {
            clear();
            insert_unique(ilist.begin(),ilist.end() );
            return *this;
        }
        
        
        
        Compare key_comp() const { return key_compare;}
        iterator begin() { return iterator(header->left);}
        iterator end() { return iterator(header);}
        const_iterator begin() const { return const_iterator(header->left);}
        const_iterator end() const { return const_iterator(header);}
        const_iterator cbegin() const { return const_iterator(header->left);}
        const_iterator cend() const { return const_iterator(header);}
        
        bool empty() const { return node_count==0;}
        size_type size() const { return node_count;}
        size_type max_size() const { return size_type(-1);}
        
        iterator find (const Key& k) {
            link_type x = root();
            
            while (x != 0) {
                bool cmp = key_compare(k, key(x));
                if (k == key(x))
                    return iterator(x);
                else if (cmp)
                    x = x->left;
                else
                    x = x->right;
            }
            
            return end();
        }
        
        const_iterator find(const Key& k) const {
            link_type x = root();
            
            while (x != 0) {
                bool cmp = key_compare(k, key(x));
                if (k == key(x))
                    return const_iterator(x);
                else if (cmp)
                    x = x->left;
                else
                    x = x->right;
            }
            
            return cend();
        }
        
        iterator lower_bound(const Key& k) {
            iterator ite = begin();
            iterator end = end();
            while (ite != end) {
                if (*ite < k)
                    ++ite;
                else
                    return ite;
            }
            
            return ite;
        }
        
        const_iterator lower_bound (const Key& k) const {
            const_iterator ite = cbegin();
            const_iterator end = cend();
            while (ite != end) {
                if (*ite < k)
                    ++ite;
                else
                    return ite;
            }
            
            return ite;
        }
        
        iterator upper_bound(const Key& k) {
            iterator ite = begin();
            iterator end = end();
            while (ite != end) {
                if (*ite <= k)
                    ++ite;
                else
                    return ite;
            }
            
            return ite;
        }
        
        const_iterator upper_bound(const Key& k) const {
            const_iterator ite = cbegin();
            const_iterator end = cend();
            while (ite != end) {
                if (*ite <= k)
                    ++ite;
                else
                    return ite;
            }
            
            return ite;
        }
        
        iterator insert_equal( const Value& v) {
            link_type tmp =  __insert(root(), v);
            tmp->parent = header;
            tmp->color = __rb_tree_black;
            header->parent = tmp;
            // Change leftmost
            while (tmp->left != 0) {
                tmp = tmp->left;
            }
            header->left = tmp;
            // Change rightmost
            tmp = root();
            while (tmp->right != 0) {
                tmp = tmp->right;
            }
            header->right = tmp;
            // Add node count number
            ++node_count;
            iterator result = find( KeyOfValue()(v) );
            return result;
        }
        
        std::pair< iterator, bool> insert_unique(const Value& v) {
            iterator ite = find( KeyOfValue()(v) );
            if (ite != end())
                return std::pair<iterator,bool>(ite,false);
            
            else
            {
                return std::pair<iterator,bool>(insert_equal(v),true);
            }
        }
        
        template <class InputIterator>
        iterator insert_unique(InputIterator first, InputIterator last) {
            InputIterator ite = first;
            bool flag = true;
            std::pair<iterator, bool> result;
            
            while (ite!=last && flag==true) {
                result = insert_unique(*ite);
                ++ite;
                flag = result.second;
            }
            
            return result.first;
        }
        
        
        
        std::pair<iterator,bool> deleteNode(Key k) {
            iterator ite = find(k);
            if (ite==end())
                return std::pair<iterator, bool>(ite,false);
            ++ite;
            link_type tmp = deleteNode(root(), k);
            if (tmp != 0) {
                tmp->color = __rb_tree_black;
                tmp->parent = header;
                header->parent = tmp;
                // Change leftmost
                while (tmp->left != 0)
                    tmp = tmp->left;
                header->left = tmp;
                // Change rightmost
                tmp = root();
                while (tmp->right != 0)
                    tmp = tmp->right;
                header->right = tmp;
            }
            else {
                header->parent = 0;
                header->left  = header;
                header->right = header;
            }
            return std::pair<iterator, bool>(ite,true);
        }
        
        void deleteMin() {
            link_type tmp = deleteMin(root());
            if (tmp != 0) {
                tmp->color = __rb_tree_black;
                tmp->parent = header;
                header->parent = tmp;
                // Change leftmost
                while (tmp->left != 0)
                    tmp = tmp->left;
                header->left = tmp;
                // Change rightmost
                tmp = root();
                while (tmp->right != 0)
                    tmp = tmp->right;
                header->right = tmp;
                // Decrease node count number
                --node_count;
            }
            else {
                header->parent = 0;
                header->left  = header;
                header->right = header;
            }
            
        }
        
        bool operator==(const rb_tree& rhs) {
            auto p = root(); auto q = rhs.root();
            return isSameTree(p, q);
        }
        bool operator!=(const rb_tree& rhs) {
            auto p = root(); auto q = rhs.root();
            return !isSameTree(p, q);
        }
        // end of rb_tree
    };
    // end of namespace
}

#endif


#endif
