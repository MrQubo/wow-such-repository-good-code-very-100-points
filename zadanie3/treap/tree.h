/*!
 * =====================================================================================
 *
 *       Filename:  tree.h
 *
 *       Compiler:  g++
 *    C++ Version:  >= 20170000
 *
 *         Author:  Jakub Nowak <jakub.jakub.nowak@gmail.com>
 *
 * =====================================================================================
 */


#if __INCLUDE_LEVEL__ == 0
#pragma once
#endif


#include <iterator>         // std::bidirectional_iterator_tag
#include <memory>           // std::allocator_traits
#include <type_traits>      // std::is_same_v
#include <utility>          // std::forward, std::make_pair, std::move, std::move_if_noexcept, std::pair



/*
 * The invariants
 * header.left   === leftmost  node in the tree
 * header.right  === rightmost node in the tree
 * header.parent === tree root
 * priority of the parent is less than or equal to both children priorities
 * key of the left child is less than or equal to parent key
 * key of the right child is greater than or equal to parent key
 */



/**
 * @pre Allocator::value_type has to be the same type as Key
 */
template <class Key, class Value, class Priority, class KeyOfValue, class KeyCompareType, class RandomPriorityGenerator, class Allocator>
class tree {
static_assert(
    std::is_same_v<Key, typename std::allocator_traits<Allocator>::value_type>,
    "treap must have the same value_type as its allocator"
);

///////////////////////////////////////////////// BEGIN DEFINITIONS /////////////////////////////////////////////////
public: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PUBLIC
using key_type         = Key;
using value_type       = Value;
using priority_type    = Priority;
using key_compare      = KeyCompareType;
using random_generator = RandomPriorityGenerator;
using allocator_type   = Allocator;
using size_type        = typename std::allocator_traits<allocator_type>::size_type;
using difference_type  = typename std::allocator_traits<allocator_type>::difference_type;

using reference       = value_type&;
using const_reference = const value_type&;
using pointer         = typename std::allocator_traits<allocator_type>::pointer;
using const_pointer   = typename std::allocator_traits<allocator_type>::const_pointer;

class const_iterator;
using iterator = const_iterator;

// explicit tree(const key_compare&, const random_generator&, const allocator_type&);
//
// ~tree();
//
// tree& operator= (const tree&);
// tree& operator= (tree&&);
//
// bool empty() const;
// size_type size() const;
// size_type max_size() const;
//
// void clear();
//
// // insert, emplace
// iterator insert_equal(const value_type&);
// iterator insert_equal(value_type&&);
// iterator insert_equal(const_iterator hint, const value_type&);
// iterator insert_equal(const_iterator hint, value_type&&);
//
// iterator insert_unique(const value_type&);
// iterator insert_unique(value_type&&);
// iterator insert_unique(const_iterator hint, const value_type&);
// iterator insert_unique(const_iterator hint, value_type&&);
//
// template<class... Args> iterator emplace_equal(Args&&...);
// template<class... Args> iterator emplace_equal_hint(const_iterator hint, Args&&...);
//
// template<class... Args> iterator emplace_unique(Args&&...);
// template<class... Args> iterator emplace_unique_hint(const_iterator hint, Args&&...);
//
// // TODO: erase
// iterator erase(const_iterator);
// iterator erase(const_iterator first, const_iterator last);
// size_type erase(const key_type&);
//
// void swap(tree&);
//
// // TODO: count
//                   size_type count(const key_type&) const;
// template<class K> size_type count(const K&       ) const;
//
// // find
                        // iterator find(const key_type&)      ;
                  // const_iterator find(const key_type&) const;
// template<class K>       iterator find(const K&       )      ;
// template<class K> const_iterator find(const K&       ) const;
//
// // lower_bound
                        // iterator lower_bound(const key_type&)      ;
                  // const_iterator lower_bound(const key_type&) const;
// template<class K>       iterator lower_bound(const K&       )      ;
// template<class K> const_iterator lower_bound(const K&       ) const;
//
// // upper_bound
                        // iterator upper_bound(const key_type&)      ;
                  // const_iterator upper_bound(const key_type&) const;
// template<class K>       iterator upper_bound(const K&       )      ;
// template<class K> const_iterator upper_bound(const K&       ) const;
//
// // equal_range
                  // std::pair<      iterator,       iterator> equal_range(const key_type&)      ;
                  // std::pair<const_iterator, const_iterator> equal_range(const key_type&) const;
// template<class K> std::pair<      iterator,       iterator> equal_range(const K&       )      ;
// template<class K> std::pair<const_iterator, const_iterator> equal_range(const K&       ) const;
//
// key_compare      key_comp     () const;
// random_generator random_gen   () const;
// allocator_type   get_allocator() const;

// // TODO: comparators
// bool operator== (const tree&);
// bool operator!= (const tree&);
// bool operator<  (const tree&);
// bool operator<= (const tree&);
// bool operator>  (const tree&);
// bool operator>= (const tree&);

protected: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PROTECTED
class tree_node_base;
class tree_node;

using base_allocator = typename allocator_type::template rebind<tree_node_base>::other;
using link_allocator = typename allocator_type::template rebind<tree_node>::other;

using base_ptr = typename std::allocator_traits<base_allocator>::pointer;
using link_ptr = typename std::allocator_traits<link_allocator>::pointer;
///////////////////////////////////////////////// END DEFINITIONS /////////////////////////////////////////////////
///////////////////////////////////////////////// BEGIN VARIABLES /////////////////////////////////////////////////
private: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PRIVATE
// header invariant:
// header.left == lowest value (begin)
// header.right == highest value (rbegin)
// header.parent == tree root
tree_node_base   header;
key_compare      _key_compare;
random_generator _random_generator;
link_allocator   _link_allocator;
size_type        node_count;
///////////////////////////////////////////////// END VARIABLES /////////////////////////////////////////////////
///////////////////////////////////////////////// BEGIN CONSTRUCTORS /////////////////////////////////////////////////
public: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PUBLIC
explicit tree(
    const key_compare&      __key_compare,
    const random_generator& __random_generator,
    const allocator_type&   __allocator
)
    : header            (nullptr, &header, &header),
      _key_compare      (__key_compare),
      _random_generator (__random_generator),
      _link_allocator   (__allocator),
      node_count        (0)
{ }

~tree()
{
    clear();
}

tree(const tree& other)
    : header            (nullptr, &header, &header),
      _key_compare      (other._key_compare),
      _random_generator (other._random_generator),
      _link_allocator   (other._link_allocator),
      node_count        (other.node_count)
{
    copy_tree(other);
}

tree(tree&& other)
    : header            (std::move             (other.header           ) ),
      _key_compare      (std::move_if_noexcept (other._key_compare     ) ),
      _random_generator (std::move_if_noexcept (other._random_generator) ),
      _link_allocator   (std::move_if_noexcept (other._link_allocator  ) ),
      node_count        (std::move             (other.node_count       ) )
{
    other.reset_header();
}

tree& operator= (const tree& other)
{
    if (this == &other) { return *this; }

    clear();

    _key_compare      = other._key_compare;
    _random_generator = other._random_generator;
    _link_allocator   = other._link_allocator;
    node_count        = other.node_count;

    copy_tree(other);

    return *this;
}

tree& operator= (tree&& other)
{
    if (this == &other) { return *this; }
    // XXX: check get_allocator() == other.get_allocator()

    clear();

    header            = std::move             (other.header);
    _key_compare      = std::move_if_noexcept (other._key_compare);
    _random_generator = std::move_if_noexcept (other._random_generator);
    _link_allocator   = std::move_if_noexcept (other._link_allocator);
    node_count        = std::move             (other.node_count);

    other.reset_header();

    return *this;
}

void swap(tree& other) {
    swap(header, other.header);
    swap(_key_compare, other._key_compare);
    swap(_random_generator, other._random_generator);
    swap(_link_allocator, other._link_allocator);
    swap(node_count, other.node_count);
}
///////////////////////////////////////////////// END CONSTRUCTORS /////////////////////////////////////////////////
///////////////////////////////////////////////// BEGIN CAPACITY /////////////////////////////////////////////////
public: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PUBLIC
bool empty() const
{
    return node_count == 0;
}

size_type size() const
{
    return node_count;
}

size_type max_size() const
{
    return get_allocator().max_size();
}
///////////////////////////////////////////////// END CAPACITY /////////////////////////////////////////////////
///////////////////////////////////////////////// BEGIN MODIFIERS /////////////////////////////////////////////////
public: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PUBLIC
void clear()
{
    delete_node_tree(get_root());
    reset_header();
}

iterator insert_equal(const value_type& val)
{
    link_ptr node(create_node( val ));
    insert_equal(node);
    return iterator(node);
}

iterator insert_equal(value_type&& val)
{
    link_ptr node(create_node( std::forward(val) ));
    insert_equal(node);
    return iterator(node);
}

iterator insert_equal(const_iterator hint, const value_type& val)
{
    link_ptr node(create_node( val ));

    if (correct_hint_equal(hint, node)) {
        insert_left(hint, node);
    }

    insert_equal(node);
    return iterator(node);
}

iterator insert_equal(const_iterator hint, value_type&& val)
{
    link_ptr node(create_node( std::forward(val) ));

    if (correct_hint_equal(hint, node)) {
        insert_left(hint, node);
    }

    insert_equal(node);
    return iterator(node);
}

iterator insert_unique(const value_type& val)
{
    link_ptr node(create_node( val ));
    return iterator(insert_unique(node));
}

iterator insert_unique(value_type&& val)
{
    link_ptr node(create_node( std::forward<value_type>(val) ));
    return iterator(insert_unique(node));
}

iterator insert_unique(const_iterator hint, const value_type& val)
{
    link_ptr node(create_node( val ));

    if (correct_hint_unique(hint, node)) {
        insert_left(hint, node);
    }

    return iterator(insert_unique(node));
}

iterator insert_unique(const_iterator hint, value_type&& val)
{
    link_ptr node(create_node( std::forward(val) ));

    if (correct_hint_unique(hint, node)) {
        insert_left(hint, node);
    }

    return iterator(insert_unique(node));
}

template <class... Args>
iterator emplace_equal(Args&&... args)
{
    link_ptr node(create_node( std::forward<Args>(args)... ));
    insert_equal(node);
    return iterator(node);
}

template <class... Args>
iterator emplace_equal_hint(const_iterator hint, Args&&... args)
{
    link_ptr node(create_node( std::forward<Args>(args)... ));

    if (correct_hint_equal(hint, node)) {
        insert_left(hint, node);
    }

    insert_equal(node);
    return iterator(node);
}

template <class... Args>
iterator emplace_unique(Args&&... args)
{
    link_ptr node(create_node( std::forward<Args>(args)... ));
    return iterator(insert_unique(node));
}

template <class... Args>
iterator emplace_unique_hint(const_iterator hint, Args&&... args)
{
    link_ptr node(create_node( std::forward<Args>(args)... ));

    if (correct_hint_unique(hint, node)) {
        insert_left(hint, node);
    }

    return iterator(insert_unique(node));
}

protected: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PROTECTED
bool correct_hint_equal(base_ptr)
{
    // TODO
    return false;
}

bool correct_hint_unique(base_ptr)
{
    // TODO
    return false;
}

void insert_equal(link_ptr new_node)
{
    insert_left(find_w_check(get_root(), new_node->get_key(), get_header()), new_node);
}

link_ptr insert_unique(link_ptr new_node)
{
    base_ptr found_node = find_w_check(get_root(), new_node->get_key(), get_header());

    if (found_node != get_header() && key_comp_le(found_node, new_node)) {
        return (link_ptr)found_node;
    }

    insert_left(found_node, new_node);
    return new_node;
}

void insert_left(base_ptr insertion_point, link_ptr new_node)
{
    if (insertion_point == get_header()) {
        new_node->right = get_header();
        new_node->parent = get_header()->right;
        get_header()->right->right = new_node;
        get_header()->right = new_node;
    } else {
        new_node->left = insertion_point->left;
        new_node->parent = insertion_point;
        insertion_point->left = new_node;
        if (new_node->left == get_header()) {
            get_header()->left = new_node;
        }
    }

    rebalance_new_node(new_node);
}

private: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PRIVATE
// TODO: copy tree with different Key and Value
inline void copy_tree(const tree& other)
{
    copy_tree(get_root(), other.get_root(), nullptr, other.get_header());
}

void copy_tree(link_ptr& node, link_ptr other_node, link_ptr parent, base_ptr other_header)
{
    while (other_node != nullptr && other_node != other_header) {
        node = create_node(*other_node);
        node->parent = parent;

        copy_tree(node->right, other_node->right, node, other_header);

        parent = node;
        node = node->left;
        other_node = other_node->left;
    }
}

inline link_ptr get_root()
{
    return (link_ptr)(get_header()->parent);
}

inline base_ptr get_header()
{
    return &header;
}

inline void reset_header()
{
    header.parent = nullptr;
    header.left   = &header;
    header.right  = &header;
    node_count    = 0;
}
///////////////////////////////////////////////// END MODIFIERS /////////////////////////////////////////////////
///////////////////////////////////////////////// BEGIN QUERIES /////////////////////////////////////////////////
public: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PUBLIC
iterator find(const key_type& key)
{
    if (node_count == 0) {
        return iterator(get_header());
    } else {
        return iterator(find_equal(get_root(), key));
    }
}

const_iterator find(const key_type& key) const
{
    if (node_count == 0) {
        return iterator(get_header());
    } else {
        return iterator(find(get_root(), key));
    }
}

// finds any node with key equal to lower bound or best_node if no such node exists
base_ptr find(base_ptr node, const key_type& key, base_ptr best_node)
{
    while (true) {
        if (key_comp_lt(node, key)) {
            if (node->right == nullptr || node->right == get_header()) {
                return best_node;
            } else {
                node = node->right;
            }
        } else if (key_comp_le(node, key)) {
            return node;
        } else {
            if (node->left == nullptr || node->left == get_header()) {
                return node;
            } else {
                best_node = node;
                node = node->left;
            }
        }
    }
}

base_ptr find_w_check(base_ptr node, const key_type& key, base_ptr best_node)
{
    if (node == nullptr || node == get_header()) {
        return best_node;
    } else {
        return find(node, key, best_node);
    }
}

// finds first node with key greater or equal to given key or best_node if no such node exists
base_ptr lower_bound(base_ptr node, const key_type& key, base_ptr best_node)
{
    while (true) {
        if (key_comp_lt(node, key)) {
            if (node->right == nullptr || node->right == get_header()) {
                return best_node;
            } else {
                node = node->right;
            }
        } else {
            if (node->left == nullptr || node->left == get_header()) {
                return node;
            } else {
                best_node = node;
                node = node->left;
            }
        }
    }
}

base_ptr lower_bound_w_check(base_ptr node, const key_type& key, base_ptr best_node)
{
    if (node == nullptr || node == get_header()) {
        return best_node;
    } else {
        return lower_bound(node, key, best_node);
    }
}

// finds first node with key greater to given key or best_node if no such node exists
base_ptr upper_bound(base_ptr node, const key_type& key, base_ptr best_node)
{
    while (true) {
        if (key_comp_le(node, key)) {
            if (node->right == nullptr || node->right == get_header()) {
                return best_node;
            } else {
                node = node->right;
            }
        } else {
            if (node->left == nullptr || node->left == get_header()) {
                return node;
            } else {
                best_node = node;
                node = node->left;
            }
        }
    }
}

base_ptr upper_bound_w_check(base_ptr node, const key_type& key, base_ptr best_node)
{
    if (node == nullptr || node == get_header()) {
        return best_node;
    } else {
        return upper_bound(node, key, best_node);
    }
}

// return lower and upper bound
std::pair<base_ptr, base_ptr> equal_range(base_ptr node, const key_type& key, base_ptr best_node)
{
    while (true) {
        if (key_comp_lt(node, key)) {
            if (node->right == nullptr || node->right == get_header()) {
                return std::make_pair(best_node, best_node);
            } else {
                node = node->right;
            }
        } else if (key_comp_le(node, key)) {
            return std::make_pair(lower_bound_w_check(node->left, key, node), upper_bound_w_check(node->right, key, best_node));
        } else {
            if (node->left == nullptr || node->left == get_header()) {
                return std::make_pair(node, node);
            } else {
                best_node = node;
                node = node->left;
            }
        }
    }
}

std::pair<base_ptr, base_ptr> equal_range_w_check(base_ptr node, const key_type& key, base_ptr best_node)
{
    if (node == nullptr || node == get_header()) {
        return std::make_pair(best_node, best_node);
    } else {
        return equal_range(node, key, best_node);
    }
}
///////////////////////////////////////////////// END QUERIES /////////////////////////////////////////////////
///////////////////////////////////////////////// BEGIN OBSERVERS /////////////////////////////////////////////////
public: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PUBLIC
key_compare      key_comp     () const { return _key_compare;      }
random_generator random_gen   () const { return _random_generator; }
allocator_type   get_allocator() const { return _link_allocator;   }
///////////////////////////////////////////////// END OBSERVERS /////////////////////////////////////////////////
///////////////////////////////////////////////// BEGIN ROTATION /////////////////////////////////////////////////
private: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PRIVATE
void rebalance_new_node(base_ptr node)
{
    if (node->parent != nullptr && priority_comp_lt(node, node->parent)) {
        rebalance_to_root_wo_check(node);
    } else {
        rebalance_to_leaf(node);
    }
}

void rebalance_to_root_wo_check(base_ptr node)
{
    do {
        rotate_parent(node);
    } while (node->parent != nullptr && priority_comp_lt(node, node->parent));
}

inline void rebalance_to_root(base_ptr node)
{
    if (node->parent != nullptr && priority_comp_lt(node, node->parent)) {
        rebalance_to_root_wo_check(node);
    }
}

void rebalance_to_leaf(base_ptr node)
{
    left_lt:
        if (node->right != nullptr && priority_comp_lt(node->right, node)) {
            rotate_right(node);
            goto left_lt;
        } else {
            rotate_left(node);
            goto unknown;
        }

    right_lt:
        if (node->left != nullptr && priority_comp_lt(node->left, node)) {
            rotate_left(node);
            goto right_lt;
        } else {
            rotate_right(node);
            goto unknown;
        }

    unknown:
        if (node->left != nullptr && priority_comp_lt(node->left, node)) {
            if (node->right != nullptr && priority_comp_lt(node->right, node->left)) {
                rotate_right(node);
                goto left_lt;
            } else {
                rotate_left(node);
                goto right_lt;
            }
        } else if (node->right != nullptr && priority_comp_lt(node->right, node)) {
            rotate_right(node);
            goto left_lt;
        }
}

// rotates node with its left child
void rotate_left(base_ptr y)
{
    base_ptr x        (y->left  );
    base_ptr y_parent (y->parent);
    base_ptr B        (x->right );

	if (y_parent != nullptr) {
        if (y_parent->left == y) { y_parent->left  = x; }
        else                     { y_parent->right = x; }
    }
    x->parent = y_parent;

    x->right  = y;
    y->parent = x;

    y->left = B;
    if (B != nullptr) { B->parent = y; }
}

// rotates node with its right child
void rotate_right(base_ptr y)
{
    base_ptr x        (y->right );
    base_ptr y_parent (y->parent);
    base_ptr B        (x->left  );

	if (y_parent != nullptr) {
        if (y_parent->left == y) { y_parent->left  = x; }
        else                     { y_parent->right = x; }
    }
    x->parent = y_parent;

    x->right  = y;
    y->parent = x;

    y->left = B;
    if (B != nullptr) { B->parent = y; }
}

// rotates node with its parent
void rotate_parent(base_ptr node)
{
    if (node == node->parent->left) {
        rotate_left (node->parent);
    } else {
        rotate_right(node->parent);
    }
}
///////////////////////////////////////////////// END ROTATION /////////////////////////////////////////////////
///////////////////////////////////////////////// BEGIN COMPARISION /////////////////////////////////////////////////
protected: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PROTECTED
inline bool key_comp_lt(link_ptr lhs, link_ptr rhs) { return key_comp_lt(lhs->get_key(), rhs->get_key()); }
inline bool key_comp_le(link_ptr lhs, link_ptr rhs) { return key_comp_le(lhs->get_key(), rhs->get_key()); }

inline bool key_comp_lt(link_ptr lhs, const key_type& rhs) { return key_comp_lt(lhs->get_key(), rhs); }
inline bool key_comp_le(link_ptr lhs, const key_type& rhs) { return key_comp_le(lhs->get_key(), rhs); }

inline bool key_comp_lt(const key_type& lhs, link_ptr rhs) { return key_comp_lt(lhs, rhs->get_key()); }
inline bool key_comp_le(const key_type& lhs, link_ptr rhs) { return key_comp_le(lhs, rhs->get_key()); }

inline bool key_comp_lt(base_ptr lhs, base_ptr rhs) { return key_comp_lt((link_ptr)lhs, (link_ptr)rhs); }
inline bool key_comp_le(base_ptr lhs, base_ptr rhs) { return key_comp_le((link_ptr)lhs, (link_ptr)rhs); }

inline bool key_comp_lt(base_ptr lhs, const key_type& rhs) { return key_comp_lt((link_ptr)lhs, rhs); }
inline bool key_comp_le(base_ptr lhs, const key_type& rhs) { return key_comp_le((link_ptr)lhs, rhs); }

inline bool key_comp_lt(const key_type& lhs, base_ptr rhs) { return key_comp_lt(lhs, (link_ptr)rhs); }
inline bool key_comp_le(const key_type& lhs, base_ptr rhs) { return key_comp_le(lhs, (link_ptr)rhs); }

inline bool key_comp_lt(const key_type& lhs, const key_type& rhs) { return     _key_compare(lhs, rhs); }
inline bool key_comp_le(const key_type& lhs, const key_type& rhs) { return not _key_compare(rhs, lhs); }

inline bool priority_comp_lt(link_ptr lhs, link_ptr rhs) { return lhs->priority < rhs->priority; }

inline bool priority_comp_lt(base_ptr lhs, base_ptr rhs) { return priority_comp_lt((link_ptr)lhs, (link_ptr)rhs); }
///////////////////////////////////////////////// END COMPARISION /////////////////////////////////////////////////
///////////////////////////////////////////////// BEGIN NODE /////////////////////////////////////////////////
protected: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PROTECTED
inline link_ptr allocate_node()
{
    return _link_allocator.allocate(1);
}

inline void deallocate_node(link_ptr node)
{
    _link_allocator.deallocate(node, 1);
}

template <class... Args>
inline void construct_node(link_ptr node, Args&&... args)
{
    get_allocator().construct(&node->value, std::forward<Args>(args)...);
}

inline void destroy_node(link_ptr node)
{
    get_allocator().destroy(&node->value);
}

template <class... Args>
link_ptr create_node(Args&&... args)
{
    link_ptr node(allocate_node());
    try {
        construct_node(node, std::forward<Args>(args)...);
    } catch (...) {
        deallocate_node(node);
        throw;
    }
    return node;
}

inline void delete_node(link_ptr node)
{
    destroy_node(node);
    deallocate_node(node);
}

void delete_node_tree(base_ptr node)
{
    while (node != nullptr && node != get_header()) {
        destroy_node((link_ptr)node);

        delete_node_tree(node->right);

        node = node->left;
        deallocate_node((link_ptr)(node->parent));
    }
}

protected: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PROTECTED
class tree_node_base
{
    friend tree;

    private: ///////////////////////////////////////////////////////////////////////////// PRIVATE
    base_ptr parent;
    base_ptr left;
    base_ptr right;

    protected: ///////////////////////////////////////////////////////////////////////////// PROTECTED
    tree_node_base()                                  = delete;
    tree_node_base(const tree_node_base&)             = delete;
    tree_node_base(tree_node_base&&)                  = delete;
    tree_node_base& operator= (const tree_node_base&) = delete;
    tree_node_base& operator= (tree_node_base&&)      = delete;

    tree_node_base(
        base_ptr _parent = nullptr,
        base_ptr _left   = nullptr,
        base_ptr _right  = nullptr
    )
        : parent(_parent), left(_left), right(_right)
    { }
}; // class tree_node_base

class tree_node : protected tree_node_base
{
    friend tree;

    private: ///////////////////////////////////////////////////////////////////////////// PRIVATE
    priority_type priority;
    value_type    value;

    protected: ///////////////////////////////////////////////////////////////////////////// PROTECTED
    tree_node()                             = delete;
    tree_node(const tree_node&)             = delete;
    tree_node(tree_node&&)                  = delete;
    tree_node& operator= (const tree_node&) = delete;
    tree_node& operator= (tree_node&&)      = delete;

    template <class... Args>
    tree_node(priority_type _priority, Args&&... args)
        : tree_node_base(), priority(_priority), value(std::forward<Args>(args)...)
    { }

    const key_type& get_key() const { return KeyOfValue()(value); }

          value_type&& get_value()       { return value; }
    const value_type&  get_value() const { return value; }
}; // class tree_node

private: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PRIVATE
static priority_type generate_random(const random_generator& _random_generator)
{
    return _random_generator();
}
///////////////////////////////////////////////// END NODE /////////////////////////////////////////////////

///////////////////////////////////////////////// BEGIN ITERATOR /////////////////////////////////////////////////
public: ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PUBLIC
class const_iterator
{
    friend tree;

    ///////////////////////////// BEGIN DEFINITIONS /////////////////////////////
    public: ///////////////////////////////////////////////////////////////////////////// PUBLIC
    using difference_type   = tree::difference_type;
    using value_type        = tree::value_type;
    using pointer           = tree::const_pointer;
    using reference         = tree::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;

    const_iterator();
    const_iterator(const const_iterator&);

    reference operator* () const;
    pointer operator-> () const;

    const_iterator& operator++ ();
    const_iterator& operator++ (int);
    const_iterator& operator-- ();
    const_iterator& operator-- (int);

    bool operator== (const const_iterator&);
    bool operator!= (const const_iterator&);
    ///////////////////////////// END DEFINITIONS /////////////////////////////
    protected: ///////////////////////////////////////////////////////////////////////////// PROTECTED
    explicit const_iterator(base_ptr);
}; // class const_iterator
///////////////////////////////////////////////// END ITERATOR /////////////////////////////////////////////////
}; // class tree




// namespace std {
//     template <class Key, class KeyCompareType, class RNG, class PriorityCompareType, class Allocator>
//     void swap(const treap<Key, KeyCompareType, RNG, PriorityCompareType, Allocator>& lhs,
//         const treap<Key, KeyCompareType, RNG, PriorityCompareType, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs)));
// } // namespace std
//
// #if __cplusplus >= 201700L
// template<
//     class InputIt,
//     class KeyCompareType = std::less<typename std::iterator_traits<InputIt>::value_type>,
//     class RNG = std::default_random_engine,
//     class PriorityCompareType = std::less<RNG::result_type>,
//     class Allocator = std::allocator<typename std::iterator_traits<InputIt>::value_type>
// >
// treap(InputIt, InputIt,
//         KeyCompareType = KeyCompareType(), RNG = RNG(), PriorityCompareType = PriorityCompareType(), Allocator = Allocator()
//     )
//     -> treap<typename std::iterator_traits<InputIt>::value_type, KeyCompareType, RNG, PriorityCompareType, Allocator>;
//
// template<
//     class InputIt,
//     class KeyCompareType = std::less<typename std::iterator_traits<InputIt>::value_type>,
//     class RNG = std::default_random_engine,
//     class PriorityCompareType = std::less<RNG::result_type>,
//     class Allocator = std::allocator<typename std::iterator_traits<InputIt>::value_type>
// >
// treap(InputIt, InputIt, RNG = RNG(), PriorityCompareType = PriorityCompareType(), Allocator = Allocator())
//     -> treap<typename std::iterator_traits<InputIt>::value_type, KeyCompareType, RNG, PriorityCompareType, Allocator>;
//
// template<
//     class InputIt,
//     class KeyCompareType = std::less<typename std::iterator_traits<InputIt>::value_type>,
//     class RNG = std::default_random_engine,
//     class PriorityCompareType = std::less<RNG::result_type>,
//     class Allocator = std::allocator<typename std::iterator_traits<InputIt>::value_type>
// >
// treap(InputIt, InputIt, KeyCompareType = KeyCompareType(), Allocator = Allocator())
//     -> treap<typename std::iterator_traits<InputIt>::value_type, KeyCompareType, RNG, PriorityCompareType, Allocator>;
//
// template<
//     class InputIt,
//     class KeyCompareType = std::less<typename std::iterator_traits<InputIt>::value_type>,
//     class RNG = std::default_random_engine,
//     class PriorityCompareType = std::less<RNG::result_type>,
//     class Allocator = std::allocator<typename std::iterator_traits<InputIt>::value_type>
// >
// treap(InputIt, InputIt, Allocator = Allocator())
//     -> treap<typename std::iterator_traits<InputIt>::value_type, KeyCompareType, RNG, PriorityCompareType, Allocator>;
//
// template<
//     class Key,
//     class KeyCompareType = std::less<Key>,
//     class RNG = std::default_random_engine,
//     class PriorityCompareType = std::less<RNG::result_type>,
//     class Allocator = std::allocatorKey>
// >
// treap(std::initializer_list<Key>,
//         KeyCompareType = KeyCompareType(), RNG = RNG(), PriorityCompareType = PriorityCompareType(), Allocator = Allocator()
//     )
//     -> treap<Key, KeyCompareType, RNG, PriorityCompareType, Allocator>;
//
// template<
//     class Key,
//     class KeyCompareType = std::less<Key>,
//     class RNG = std::default_random_engine,
//     class PriorityCompareType = std::less<RNG::result_type>,
//     class Allocator = std::allocator<Key>
// >
// treap(std::initializer_list<Key>, RNG = RNG(), PriorityCompareType = PriorityCompareType(), Allocator = Allocator())
//     -> treap<Key, KeyCompareType, RNG, PriorityCompareType, Allocator>;
//
// template<
//     class Key,
//     class KeyCompareType = std::less<Key>,
//     class RNG = std::default_random_engine,
//     class PriorityCompareType = std::less<RNG::result_type>,
//     class Allocator = std::allocator<Key>
// >
// treap(std::initializer_list<Key>, KeyCompareType = KeyCompareType(), Allocator = Allocator())
//     -> treap<Key, KeyCompareType, RNG, PriorityCompareType, Allocator>;
//
// template<
//     class Key,
//     class KeyCompareType = std::less<Key>,
//     class RNG = std::default_random_engine,
//     class PriorityCompareType = std::less<RNG::result_type>,
//     class Allocator = std::allocator<Key>
// >
// treap(std::initializer_list<Key>, Allocator = Allocator())
//     -> treap<Key, KeyCompareType, RNG, PriorityCompareType, Allocator>;
// #endif //  __cplusplus >= 201700L


// std::equal(a.begin(), a.end(), b.begin(), b.end(), comp) // operator==

