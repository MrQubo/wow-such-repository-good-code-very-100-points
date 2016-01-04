/*!
 *
 * Jakub Nowak
 *
 * Zadanie 3: Drzepiec
 *
 * After any change made to treap iterators contructed before changes might become invalid.
 * Incrementing or decrementing invalid operators lead to undefined behavior.
 *
 */
#pragma once
#ifndef __JAKUB_NOWAK_TREAP_H
#define __JAKUB_NOWAK_TREAP_H

#include <random>
#include <iostream>
#include <utility>
#include <functional>
#include <initializer_list>

// treap<T, CompareType, URNG>
template<class T, class CompareType = std::less<T>, class URNG = std::default_random_engine>
class treap {

///////////////////////////// BEGIN TYPEDEFS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
typedef T		key_type;
typedef T		value_type;
typedef CompareType	key_compare;
typedef CompareType	value_compare;

typedef unsigned long long	size_type;
///////////////////////////// END TYPEDEFS /////////////////////////////

///////////////////////////// BEGIN DECLARATIONS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
class iterator;
class const_iterator;
class reverse_iterator;
class const_reverse_iterator;
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
class node;
node* root;
CompareType compare;
URNG radom;
///////////////////////////// END DECLARATIONS /////////////////////////////
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
///////////////////////////// BEGIN ERRORS AND WARNINGS /////////////////////////////
static void _err_lowest_on_empty() {
    std::cerr << "FatalError: Using lowest() on empty treap" << std::endl;
    exit( EXIT_FAILURE );
}
static void _err_highest_on_empty() {
    std::cerr << "FatalError: Using highest() on empty treap" << std::endl;
    exit( EXIT_FAILURE );
}
static void _err_dereference_past_the_end() {
	std::cerr << "FatalError: Attempt to dereference past-the-end iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static void _err_dereference_past_the_rend() {
	std::cerr << "FatalError: Attempt to dereference past-the-rend reverse iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static void _err_dereference_invalid() {
	std::cerr << "FatalError: Attempt to dereference no more valid iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static void _err_dereference_invalid_r() {
	std::cerr << "FatalError: Attempt to dereference no more valid reverse iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static void _err_incrementing_past_the_end() {
	std::cerr << "FatalError: Attempt to increment past-the-end iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static void _err_incrementing_past_the_rend() {
	std::cerr << "FatalError: Attempt to increment past-the-rend reverse iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static void _err_decrementing_begin() {
    std::cerr << "FatalError: Attempt to decrement iterator pointing to begin" << std::endl;
    exit( EXIT_FAILURE );
}
static void _err_decrementing_rbegin() {
    std::cerr << "FatalError: Attempt to decrement reverse iterator pointing to rbegin" << std::endl;
    exit( EXIT_FAILURE );
}
static void _err_erase_past_the_end() {
    std::cerr << "FatalError: Attempt to erase past-the-end iterator" << std::endl;
    exit( EXIT_FAILURE );
}
static void _err_can_not_merge() {
	std::cerr << "FatalError: Treaps can not be merged," << std::endl;
	std::cerr << "FatalError: None of two treaps have higher lowest value than higher value of other treap," << std::endl;
	std::cerr << "FatalError: Instead try to iterate over values from one treap and insert them to second treap" << std::endl;
	exit( EXIT_FAILURE );
}
static void _warn_merge_inequal_compare() {
	std::cerr << "Warn: Merging two treaps with inequal objects of type CompareType," << std::endl;
}
static void _warn_merge_inequal_compare_using_first() {
	_warn_merge_inequal_compare();
	std::cerr << "Warn: Using object of type CompareType from first treap" << std::endl;
}
static void _warn_merge_inequal_compare_using_second() {
	_warn_merge_inequal_compare();
	std::cerr << "Warn: Using object of type CompareType from second treap" << std::endl;
}
static void _warn_merge_inequal_radom() {
	std::cerr << "Warn: Merging two treaps with inequal objects of type URNG," << std::endl;
}
static void _warn_merge_inequal_radom_using_first() {
	_warn_merge_inequal_radom();
	std::cerr << "Warn: Using object of type URNG from first treap" << std::endl;
}
static void _warn_merge_inequal_radom_using_second() {
	_warn_merge_inequal_radom();
	std::cerr << "Warn: Using object of type URNG from second treap" << std::endl;
}
///////////////////////////// END ERRORS AND WARNINGS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
///////////////////////////// BEGIN CONSTRUCTORS /////////////////////////////
treap()
	: treap( {} ) {}
explicit treap( const std::initializer_list<T>& _il )
	: treap( _il, CompareType(), URNG( 720553160 ) ) {}
explicit treap( const CompareType& _compare )
	: treap( {}, _compare, URNG( 755858114 ) ) {}
explicit treap( URNG&& _radom )
	: treap( {}, CompareType(), std::move( _radom ) ) {}
explicit treap( const std::initializer_list<T>& _il, const CompareType& _compare )
	: treap( _il, _compare, URNG( 20553730 ) ) {}
explicit treap( const std::initializer_list<T>& _il, URNG&& _radom )
	: treap( _il, CompareType(), std::move( _radom ) ) {}
explicit treap( const CompareType& _compare, URNG&& _radom )
	: treap( {}, _compare, std::move( _radom ) ) {}

// main constructor
explicit treap( const std::initializer_list<T>& _il, const CompareType& _compare, URNG&& _radom)
	: root( nullptr ), compare( _compare ), radom( std::move( _radom ) ) {
		for( const T& val : _il ) insert( val );
}

// copy contructor
treap( const treap& _other )
	: root( nullptr ), compare( _other.compare ), radom( _other.radom ) {
		copy( _other );
}
// copy assigment
treap& operator=( const treap& _other ) {
	clear();
	compare = CompareType( _other.compare );
	radom = URNG( _other.radom );
	copy( _other );
	return *this;
}

// move contructor
treap( treap&& _other )
	: root( _other.root ), compare( _other.compare ), radom( _other.radom ) {
		_other.root = nullptr;
}
// move assigment
treap& operator=( treap&& _other ) {
	clear();
	compare = CompareType( _other.compare );
	radom = URNG( _other.radom );
	root = _other.root;
	_other.root = nullptr;
	return *this;
}

// destructor
~treap() { clear(); }
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
void copy( const treap& _other ) { copy_recursion( root, _other.root ); }
void copy_recursion( node* _node, node* _other_node ) {
	if( _other_node == nullptr ) return nullptr;
	_node = new node( *_other_node );
	_node->left = copy_recursion( _node->left, _other_node->left );
	_node->right = copy_recursion( _node->right, _other_node->right );
}
///////////////////////////// END CONSTRUCTORS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
///////////////////////////// BEGIN ASK METHODS /////////////////////////////
bool empty() const { return ( root == nullptr ); }

T lowest() const {
    if( empty() ) _err_lowest_on_empty();
    return *begin();
}
T highest() const {
    if( empty() ) _err_highest_on_empty();
    return *rbegin();
}

bool contains( const T& val ) const { return ( find( val ) != end() ); }

iterator find( const T& val ) {
	if( empty() ) return end();
    node* _node = root;
    while( true )
        if( compare( _node->key, val ) ) {
			if( _node->right == nullptr )
				if( compare( val, _node->key ) )	return iterator( _node );
				else								return end();
            _node = _node->right;
        } else {
			if( _node->left == nullptr )
				if( ! compare( val, _node->key ) )	return iterator( _node );
				else								return end();
			_node = _node->left;
        }
}
const_iterator find( const T& val ) const {
	if( empty() ) return end();
    node* _node = root;
    while( true )
        if( compare( _node->key, val ) ) {
			if( ! _node->has_right() )
				if( compare( val, _node->key ) )	return const_iterator( _node );
				else								return cend();
            _node = _node->right;
        } else {
			if( ! _node->has_left() )
				if( ! compare( val, _node->key ) )	return const_iterator( _node );
				else								return cend();
			_node = _node->left;
        }
}

std::pair<treap, treap> split( const T& val ) {
    iterator it = insert( val );
    treap treap_first( compare, URNG( radom ) ), treap_second( compare, URNG( radom ) );
    treap_first.root = root->left;
    treap_second.root = root->right;
    erase( it );
    return ( std::make_pair( treap_first, treap_second ) );
}

static treap merge( const treap& treap_first, const treap& treap_second ) {
	if( ! treap_first.compare( treap_second.lowest(), treap_first.highest() ) ) {
		if( treap_first.compare != treap_second.compare ) _warn_merge_inequal_compare_using_first();
		if( treap_first.radom != treap_second.radom ) _warn_merge_inequal_radom_using_first();
		return merge_fast( treap_first, treap_second );
    } else if( ! treap_second.compare( treap_first.lowest(), treap_second.highest() ) ) {
		if( treap_first.compare != treap_second.compare ) _warn_merge_inequal_compare_using_second();
		if( treap_first.radom != treap_second.radom ) _warn_merge_inequal_radom_using_second();
		return merge_fast( treap_second, treap_first );
    } else _err_can_not_merge();
}
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
static treap merge_fast( const treap& treap_first, const treap& treap_second ) {
    treap treap_merged( treap_first.compare, URNG( treap_first.radom ) );
    treap_merged.root = new node( treap_first.highest() );
    treap_merged.root->left = copy_recursion( treap_merged.root->left, treap_first.root );
    treap_merged.root->right = copy_recursion( treap_merged.root->right, treap_second.root );
    treap_merged.erase( iterator( treap_merged.root ) );
    return treap_merged;
}
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC

key_compare key_comp() const { return compare; }
value_compare value_comp() const { return compare; }
///////////////////////////// END ASK METHODS /////////////////////////////
///////////////////////////// BEGIN ITERATORS METHODS /////////////////////////////
iterator begin() {
	if( empty() ) return end();
	node* _node = root;
	while( _node->has_left() ) _node = _node->left;
	return iterator( _node );
}
const_iterator begin() const {
	if( empty() ) return cend();
	node* _node = root;
	while( _node->has_left() ) _node = _node->left;
	return const_iterator( _node );
}
reverse_iterator rbegin() {
	if( empty() ) return rend();
	node* _node = root;
	while( _node->has_right() ) _node = _node->right;
	return reverse_iterator( _node );
}
const_reverse_iterator rbegin() const {
	if( empty() ) return crend();
	node* _node = root;
	while( _node->has_right() ) _node = _node->right;
	return const_reverse_iterator( _node );
}
iterator end() { return iterator( root, true ); }
const_iterator end() const { return const_iterator( root, true ); }
const_iterator cend() const { return const_iterator( root, true ); }
reverse_iterator rend() { return reverse_iterator( root, true ); }
const_reverse_iterator rend() const { return const_reverse_iterator( root, true ); }
const_reverse_iterator crend() const { return const_reverse_iterator( root, true ); }
///////////////////////////// END ITERATORS METHODS /////////////////////////////
///////////////////////////// BEGIN CHANGING METHODS /////////////////////////////
iterator insert( const T& val ) {
    if( empty() ) return iterator( root = new node( val, radom ) );
    node* _node = root;
    while( true )
		if( compare( _node->key, val ) )
			if( _node->has_right() ) _node = _node->right;
			else {
				_node->insert_right( new node( val, radom ) );
				rotation( _node->right );
				return iterator( _node->right );
			}
		else
			if( _node->has_left() ) _node = _node->left;
			else {
				_node->insert_left( new node( val, radom ) );
				rotation( _node->left );
				return iterator( _node->left );
			}
}
iterator insert( const_iterator it, const T& val ) { insert( val ); }

template<typename... _Args>
iterator emplace( _Args... _args ) {
	node* _new_node = new node( radom, _args... );
    if( empty() ) return iterator( root = _new_node );
    node* _node = root;
    while( true )
		if( compare( _node->key, _new_node->key ) )
			if( _node->has_right() ) _node = _node->right;
			else {
				_node->insert_right( _new_node );
				rotation( _node->right );
				return iterator( _node->right );
			}
		else
			if( _node->has_left() ) _node = _node->left;
			else {
				_node->insert_left( _new_node );
				rotation( _node->left );
				return iterator( _node->left );
			}
}
template<typename... _Args>
iterator emplace_hint( const_iterator it, _Args... _args ) { emplace( _args... ); }

size_type erase( const T& val ) {
	size_type count = 0;
    while( true ) {
		iterator it = find( val );
		if( it == end() ) return count;
		++count;
		erase( it );
    }
}
iterator erase( iterator it ) {
	if( it == end() ) { _err_erase_past_the_end(); return it; }
	node* _node = it.elem;
	iterator it_return = ++it;
	while( true )
		if( _node->has_left() )
			if( _node->has_right() ) rotate_one_of_heirs_with_highest_priority( _node );
			else rotate_left( _node->left );
		else if( _node->has_right() ) rotate_right( _node->right );
		else {
			delete _node;
			return it_return;
		}
}
void clear() {
    node* _node = root;
    while( _node != nullptr )
		if( _node->has_left() ) _node = _node->left;
		else if( _node->has_right() ) _node = _node->right;
		else {
			node* _delete = _node;
			_node = _node->parent;
			delete _delete;
		}
}
///////////////////////////// END CHANGING METHODS /////////////////////////////
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
///////////////////////////// BEGIN ROTATION /////////////////////////////
void rotation( node* _node ) {
    while( _node->has_parent() && _node->priority > _node->parent->priority )
		if( equal( _node->key, _node->parent->key ) ) rotate_equal( _node );
		else if( _node == _node->parent->right ) rotate_right( _node );
		else rotate_left( _node );
    if( !_node->has_parent() ) root = _node;
}
void rotate_left( node* const _node ) {
	node * const x			= _node;
	node * const y			= x->parent;	x->parent	= nullptr;
	node * const y_parent	= y->parent;	y->parent	= nullptr;
	node * const B			= x->right;		x->right	= nullptr;
	if( y_parent != nullptr )
		if( y_parent->left == y )	y_parent->left = x;
		else						y_parent->right = x;
    x->parent = y_parent;
    x->right=y; y->parent=x;
    y->left=B; if( B != nullptr ) { B->parent=y; }
}
void rotate_right( node* const _node ) {
	node * const y			= _node;
	node * const x			= y->parent;	y->parent	= nullptr;
	node * const x_parent	= x->parent;	x->parent	= nullptr;
	node * const B			= y->left;		y->left		= nullptr;
	if( x_parent != nullptr )
		if( x_parent->left == x )	x_parent->left = y;
		else						x_parent->right = y;
	y->parent = x_parent;
    y->left=x; x->parent=y;
	x->right=B; if( B != nullptr ) { B->parent=x; }
}
void rotate_equal( node* const _node ) { swap( _node->priority, _node->parent->priority ); }
void rotate_one_of_heirs_with_highest_priority( node* const _node ) {
	if( _node->left->priority > _node->right->priority ) rotate_left( _node->left );
	else rotate_right( _node->right );
}
///////////////////////////// END ROTATION /////////////////////////////
///////////////////////////// BEGIN ADDITIONAL HELP METHODS /////////////////////////////
bool equal( const T& val1, const T& val2 ) { return ( compare( val1, val2 ) == compare( val2, val1 ) ); }
///////////////////////////// END ADDITIONAL HELP METHODS /////////////////////////////
///////////////////////////// BEGIN NODE CLASS /////////////////////////////
class node {

//////////////// BEGIN DECLARATIONS ////////////////
public: //////////////////////////////////////////////////// PUBLIC
T key;
float priority;
node* parent;
node* left;
node* right;
//////////////// END DECLARATIONS ////////////////
public: //////////////////////////////////////////////////// PUBLIC
//////////////// BEGIN CONSTRUCTORS ////////////////
explicit node( const T& _key, URNG& _radom )
	: node(
		_key,
		std::generate_canonical<float, 0>( _radom )
	) {}
explicit node( const T& _key, const float _priority )
	: key( _key ),
	  priority( _priority ),
	  parent( nullptr ),
	  left( nullptr ),
	  right( nullptr ) {}

template< typename... _Args >
explicit node( URNG& _radom, _Args...  _args )
	: node(
		std::generate_canonical<float, 0>( _radom ),
		_args...
	) {}
template< typename... _Args >
explicit node( const float _priority, _Args...  _args )
	: key( _args... ),
	  priority( _priority ),
	  parent( nullptr ),
	  left( nullptr ),
	  right( nullptr ) {}

node( const node& _other )
	: key( _other.key ),
	  priority( _other.priority ),
	  parent( nullptr ),
	  left( nullptr ),
	  right( nullptr ) {}
node& operator=( const node& ) = delete;
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN DESTRUCTOR ////////////////
~node() {
	if( has_parent() )
		if( this == parent->left )	parent->left = nullptr;
		else						parent->right = nullptr;
}
//////////////// END DESTRUCTOR ////////////////
//////////////// BEGIN METHODS ////////////////
bool has_left() const { return ( left != nullptr ); }
bool has_right() const { return ( right != nullptr ); }
bool has_parent() const { return ( parent != nullptr ); }

void insert_left( node* _son ) {
    left = _son;
    _son->parent = this;
}
void insert_right( node* _son ) {
    right = _son;
    _son->parent = this;
}
//////////////// END METHODS ////////////////

};
///////////////////////////// END NODE CLASS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
///////////////////////////// BEGIN ITERATOR CLASS /////////////////////////////
// default iterator
class iterator {

//////////////// BEGIN DECLARATIONS ////////////////
protected: //////////////////////////////////////////////////// PROTECTED
node const * elem;
bool is_end;
//////////////// END DECLARATIONS ////////////////
public: //////////////////////////////////////////////////// PUBLIC
//////////////// BEGIN CONSTRUCTORS ////////////////
explicit iterator( const node* _elem_ptr )
	: elem( _elem_ptr ), is_end( false ) {}
explicit iterator( const node* _elem_ptr, bool _is_end )
	: elem( _elem_ptr ), is_end( _is_end ) {}
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN OPERATORS ////////////////
bool operator==( const iterator& _other ) const { return ( is_end == _other.is_end && elem == _other.elem ); }
bool operator!=( const iterator& _other ) const { return !( operator==( _other ) ); }

iterator& operator++() {
	if( is_end ) _err_incrementing_past_the_end();
    const node* _node = elem;
    if( _node->has_right() ) {
		_node = _node->right;
		while( _node->has_left() ) _node = _node->left;
		elem = _node;
    } else while( true )
		if( _node->has_parent() )
			if( _node == _node->parent->left )	{
				elem = _node->parent;
				break;
			}
			else _node = _node->parent;
		else {
			elem = _node;
			is_end = true;
			break;
		}
    return ( *this );
}
iterator operator++( int ) {
	iterator self(*this);
	operator++();
	return self;
}

iterator& operator--() {
	const node* _node = elem;
	if( is_end ) {
		is_end = false;
		while( _node->has_right() ) _node = _node->right;
	} else if( _node->has_left() ) {
		_node = _node->left;
		while( _node->has_right() ) _node = _node->right;
	} else while( true )
		if( _node->has_parent() )
			if( _node == _node->parent->right )	break;
			else _node = _node->parent;
		else _err_decrementing_begin();
	elem = _node;
	return *this;
}
iterator operator--( int ) {
	iterator self(*this);
	operator--();
	return self;
}

const T& operator*() const {
	if( is_end ) _err_dereference_past_the_end();
	if( elem == nullptr ) _err_dereference_invalid();
	return elem->key;
}
//////////////// END OPERATORS ////////////////

};

// const default iterator
class const_iterator {

//////////////// BEGIN DECLARATIONS ////////////////
protected: //////////////////////////////////////////////////// PROTECTED
node const * elem;
bool is_end;
//////////////// END DECLARATIONS ////////////////
public: //////////////////////////////////////////////////// PUBLIC
//////////////// BEGIN CONSTRUCTORS ////////////////
const_iterator( const iterator& _it )
	: elem( _it.elem ), is_end( _it.is_end ) {}
explicit const_iterator( const node* _elem_ptr )
	: elem( _elem_ptr ), is_end( false ) {}
explicit const_iterator( const node* _elem_ptr, bool _is_end )
	: elem( _elem_ptr ), is_end( _is_end ) {}
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN OPERATORS ////////////////
bool operator==( const const_iterator& _other ) const { return ( is_end == _other.is_end && elem == _other.elem ); }
bool operator!=( const const_iterator& _other ) const { return !( operator==( _other ) ); }

const_iterator& operator++() {
	if( is_end ) _err_incrementing_past_the_end();
    const node* _node = elem;
    if( _node->has_right() ) {
		_node = _node->right;
		while( _node->has_left() ) _node = _node->left;
		elem = _node;
    } else while( true )
		if( _node->has_parent() )
			if( _node == _node->parent->left )	{
				elem = _node->parent;
				break;
			}
			else _node = _node->parent;
		else {
			elem = _node;
			is_end = true;
			break;
		}
    return ( *this );
}
const_iterator operator++( int ) {
	const_iterator self(*this);
	operator++();
	return self;
}

const_iterator& operator--() {
	const node* _node = elem;
	if( is_end ) {
		is_end = false;
		while( _node->has_right() ) _node = _node->right;
	} else if( _node->has_left() ) {
		_node = _node->left;
		while( _node->has_right() ) _node = _node->right;
	} else while( true )
		if( _node->has_parent() )
			if( _node == _node->parent->right )	break;
			else _node = _node->parent;
		else _err_decrementing_begin();
	elem = _node;
	return *this;
}
const_iterator operator--( int ) {
	const_iterator self(*this);
	operator--();
	return self;
}

const T& operator*() const {
	if( is_end ) _err_dereference_past_the_end();
	if( elem == nullptr ) _err_dereference_invalid();
	return elem->key;
}
//////////////// END OPERATORS ////////////////

};

// reverse iterator
class reverse_iterator {

//////////////// BEGIN DECLARATIONS ////////////////
protected: //////////////////////////////////////////////////// PROTECTED
node const * elem;
bool is_end;
//////////////// END DECLARATIONS ////////////////
public: //////////////////////////////////////////////////// PUBLIC
//////////////// BEGIN CONSTRUCTORS ////////////////
explicit reverse_iterator( const node* _elem_ptr )
	: elem( _elem_ptr ), is_end( false ) {}
explicit reverse_iterator( const node* _elem_ptr, bool _is_end )
	: elem( _elem_ptr ), is_end( _is_end ) {}
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN OPERATORS ////////////////
bool operator==( const reverse_iterator& _other ) const { return ( is_end == _other.is_end && elem == _other.elem ); }
bool operator!=( const reverse_iterator& _other ) const { return !( operator==( _other ) ); }

reverse_iterator& operator++() {
	if( is_end ) _err_incrementing_past_the_end();
	const node* _node = elem;
    if( _node->has_left() ) {
		_node = _node->left;
		while( _node->has_right() ) _node = _node->right;
		elem = _node;
	} else while( true )
		if( _node->has_parent() )
			if( _node == _node->parent->right )	{
				elem = _node->parent;
				break;
			}
			else _node = _node->parent;
		else {
			elem = _node;
			is_end = true;
			break;
		}
	return *this;
}
reverse_iterator operator++( int ) {
	reverse_iterator self(*this);
	operator++();
	return self;
}

reverse_iterator& operator--() {
	const node* _node = elem;
	if( is_end ) {
		is_end = false;
		while( _node->has_left() ) _node = _node->left;
	} else if( _node->has_right() ) {
		_node = _node->right;
		while( _node->has_left() ) _node = _node->left;
	} else while( true )
		if( _node->has_parent() )
			if( _node == _node->parent->left ) break;
			else _node = _node->parent;
		else _err_decrementing_rbegin();
	elem = _node;
    return ( *this );
}
reverse_iterator operator--( int ) {
	reverse_iterator self(*this);
	operator--();
	return self;
}

const T& operator*() const {
	if( is_end ) _err_dereference_past_the_rend();
	if( elem == nullptr ) _err_dereference_invalid_r();
	return elem->key;
}
//////////////// END OPERATORS ////////////////

};

// const reverse iterator
class const_reverse_iterator {

//////////////// BEGIN DECLARATIONS ////////////////
protected: //////////////////////////////////////////////////// PROTECTED
node const * elem;
bool is_end;
//////////////// END DECLARATIONS ////////////////
public: //////////////////////////////////////////////////// PUBLIC
//////////////// BEGIN CONSTRUCTORS ////////////////
const_reverse_iterator( const reverse_iterator& _it )
	: elem( _it.elem ), is_end( _it.is_end ) {}
explicit const_reverse_iterator( const node* _elem_ptr )
	: elem( _elem_ptr ), is_end( false ) {}
explicit const_reverse_iterator( const node* _elem_ptr, bool _is_end )
	: elem( _elem_ptr ), is_end( _is_end ) {}
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN OPERATORS ////////////////
bool operator==( const const_reverse_iterator& _other ) const { return ( is_end == _other.is_end && elem == _other.elem ); }
bool operator!=( const const_reverse_iterator& _other ) const { return !( operator==( _other ) ); }

const_reverse_iterator& operator++() {
	if( is_end ) _err_incrementing_past_the_end();
	const node* _node = elem;
    if( _node->has_left() ) {
		_node = _node->left;
		while( _node->has_right() ) _node = _node->right;
		elem = _node;
	} else while( true )
		if( _node->has_parent() )
			if( _node == _node->parent->right )	{
				elem = _node->parent;
				break;
			}
			else _node = _node->parent;
		else {
			elem = _node;
			is_end = true;
			break;
		}
	return *this;
}
const_reverse_iterator operator++( int ) {
	const_reverse_iterator self(*this);
	operator++();
	return self;
}

const_reverse_iterator& operator--() {
	const node* _node = elem;
	if( is_end ) {
		is_end = false;
		while( _node->has_left() ) _node = _node->left;
	} else if( _node->has_right() ) {
		_node = _node->right;
		while( _node->has_left() ) _node = _node->left;
	} else while( true )
		if( _node->has_parent() )
			if( _node == _node->parent->left ) break;
			else _node = _node->parent;
		else _err_decrementing_rbegin();
	elem = _node;
    return ( *this );
}
const_reverse_iterator operator--( int ) {
	const_reverse_iterator self(*this);
	operator--();
	return self;
}

const T& operator*() const {
	if( is_end ) _err_dereference_past_the_rend();
	if( elem == nullptr ) _err_dereference_invalid_r();
	return elem->key;
}
//////////////// END OPERATORS ////////////////

};
///////////////////////////// END ITERATOR CLASS /////////////////////////////

};

#endif
