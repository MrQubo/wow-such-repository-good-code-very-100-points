/*!
 *
 * Jakub Nowak
 *
 * Zadanie 3: Drzepiec
 *
 * Hint in insert and emplace_hint must be element that should FOLLOW inserting element.
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
#include <iterator>
#include <algorithm>
#include <limits>
#include <stdexcept>

// treap<T, CompareType, URNG>
template< class T, class CompareType = std::less<T>, class URNG = std::default_random_engine >
class treap {

///////////////////////////// BEGIN TYPEDEFS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
typedef T           key_type        ;
typedef T           value_type      ;
typedef CompareType key_compare     ;
typedef CompareType value_compare   ;
typedef URNG        random_generator;

typedef unsigned long long size_type;
///////////////////////////// END TYPEDEFS /////////////////////////////

///////////////////////////// BEGIN DECLARATIONS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
class               iterator;
class         const_iterator;
class       reverse_iterator;
class const_reverse_iterator;

struct empty_treap;
struct iterator_error;
	struct past_the_end_error;
		struct dereference_past_the_end;
		struct incrementing_past_the_end;
		struct erase_past_the_end;
	struct before_begin_error;
		struct decrementing_before_begin;
	struct invalid_iterator;
		struct operation_invalid;
		struct erase_invalid_iterator;
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
class  node;
struct node_emplace;

node *      root   ;
CompareType compare;
URNG        radom  ;
///////////////////////////// END DECLARATIONS /////////////////////////////
///////////////////////////// BEGIN ERRORS AND WARNINGS /////////////////////////////
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
static std::string get_err_str( std::string const & return_type, std::string const & function_name, std::string const & message ) {
	return "In function '" + return_type + " treap<T, CompareType, URNG>::" + function_name + "':\n    " + message + "\n";
}
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
struct empty_treap: std::runtime_error {
	explicit empty_treap( std::string const & return_type, std::string const & function_name )
		: std::runtime_error( get_err_str( return_type, function_name, "Treap is empty" ) ) {}
};
struct iterator_error: std::runtime_error {
	explicit iterator_error( std::string const & return_type, std::string const & function_name, std::string const & message )
		: std::runtime_error( get_err_str( return_type, function_name, message ) ) {}
};
struct past_the_end_error: iterator_error {
	explicit past_the_end_error( std::string const & return_type, std::string const & function_name, std::string const & message )
		: iterator_error( return_type, function_name, message ) {}
};
struct dereference_past_the_end: past_the_end_error {
	explicit dereference_past_the_end( std::string const & return_type, std::string const & function_name )
		: past_the_end_error( return_type, function_name, "Attempt to dereference past-the-end iterator" ) {}
};
struct incrementing_past_the_end: past_the_end_error {
	explicit incrementing_past_the_end( std::string const & return_type, std::string const & function_name )
		: past_the_end_error( return_type, function_name, "Attempt to increment past-the-end iterator" ) {}
};
struct erase_past_the_end: past_the_end_error {
	explicit erase_past_the_end( std::string const & return_type, std::string const & function_name )
		: past_the_end_error( return_type, function_name, "Attempt to erase past-the-end iterator" ) {}
};
struct before_begin_error: iterator_error {
	explicit before_begin_error( std::string const & return_type, std::string const & function_name, std::string const & message )
		: iterator_error( return_type, function_name, message ) {}
};
struct decrementing_before_begin: before_begin_error {
	explicit decrementing_before_begin( std::string const & return_type, std::string const & function_name )
		: before_begin_error( return_type, function_name, "Attempt to decrement iterator pointing to begin" ) {}
};
struct invalid_iterator: iterator_error {
	explicit invalid_iterator( std::string const & return_type, std::string const & function_name, std::string const & message )
		: iterator_error( return_type, function_name, message ) {}
};
struct operation_invalid: invalid_iterator {
	explicit operation_invalid( std::string const & return_type, std::string const & function_name )
		: invalid_iterator( return_type, function_name, "Invalid use of invalid iterator" ) {}
};
struct erase_invalid_iterator: invalid_iterator {
	explicit erase_invalid_iterator( std::string const & return_type, std::string const & function_name )
		: invalid_iterator( return_type, function_name, "Attempt to erase invalid iterator" ) {}
};
///////////////////////////// END ERRORS AND WARNINGS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
///////////////////////////// BEGIN CONSTRUCTORS /////////////////////////////
treap()
	: treap( {} ) {}
explicit treap( std::initializer_list<T> const & _il )
	: treap( _il, CompareType(), URNG( 720553160 ) ) {}

explicit treap( CompareType const & _compare )
	: treap( {}, _compare, URNG( 755858114 ) ) {}

explicit treap( URNG && _radom )
	: treap( {}, CompareType(), std::move( _radom ) ) {}

explicit treap( std::initializer_list<T> const & _il, CompareType const & _compare )
	: treap( _il, _compare, URNG( 20553730 ) ) {}

explicit treap( std::initializer_list<T> const & _il, URNG && _radom )
	: treap( _il, CompareType(), std::move( _radom ) ) {}

explicit treap( CompareType const & _compare, URNG && _radom )
	: treap( {}, _compare, std::move( _radom ) ) {}

// main constructor
explicit treap( std::initializer_list<T> const & _il, CompareType const & _compare, URNG && _radom )
	: root( nullptr ), compare( _compare ), radom( std::move( _radom ) ) {
		for( T const & val : _il ) insert( val );
}

// copy contructor
treap( treap const & _other )
	: root( nullptr ), compare( _other.compare ), radom( _other.radom ) {
		copy( _other );
}

// copy assigment
treap & operator=( treap const & _other ) {
	if( this == &_other ) return *this;
	clear();
	compare = CompareType( _other.compare );
	radom   = URNG( _other.radom );
	copy( _other );
	return * this;
}

// move contructor
treap( treap && _other )
	noexcept(
		noexcept( CompareType( std::move( _other.compare ) ) ) &&
		noexcept( URNG( std::move( _other.radom ) ) )
	)
	: root   ( std::move( _other.root ) ),
	  compare( std::move( _other.compare ) ),
	  radom  ( std::move( _other.radom ) ) {
		_other.root = nullptr;
}

// move assigment
treap & operator=( treap && _other )
	noexcept(
		noexcept( CompareType::operator=( std::move( _other.compare ) ) ) &&
		noexcept( URNG::operator=( std::move( _other.radom ) ) )
	) {
	if( this == &_other ) return *this;
	std::cout << "foo" << std::endl;
	clear();
	root        = std::move( _other.root );
	compare     = std::move( _other.compare );
	radom       = std::move( _other.radom );
	_other.root = nullptr;
	return * this;
}

// destructor
~treap() noexcept { clear(); }

private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
void copy( treap const & _other ) { copy_recursion( root, _other.root ); }
static void copy_recursion( node * & _node, node const * const _other_node, node * const _node_ancestor = nullptr ) {
	if( _other_node == nullptr ) return;
	_node			= new node( * _other_node );
	_node->ancestor	= _node_ancestor;
	copy_recursion( _node->left , _other_node->left , _node );
	copy_recursion( _node->right, _other_node->right, _node );
}
///////////////////////////// END CONSTRUCTORS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
///////////////////////////// BEGIN ASK METHODS /////////////////////////////
bool empty() const noexcept { return root == nullptr; }

size_type size() const noexcept {
	if( empty() ) return 0;
	return root->count();
}

T lowest() const {
    if( empty() ) throw empty_treap( "T", "lowest()" );
    return * cbegin();
}
T highest() const {
    if( empty() ) throw empty_treap( "T", "highest()" );
    return * crbegin();
}

bool contains( T const & val ) const { return find( val ) != cend(); }

iterator       find( T const & val )       { return find_internal( val ); }
const_iterator find( T const & val ) const { return find_internal( val ); }
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
iterator find_internal( T const & val ) const {
    node * _node( root );
    while( _node != nullptr )
        if( eq_compare( _node->key, val ) ) {
			if( eq_compare( val, _node->key ) ) return iterator( _node );
			_node = _node->right;
        } else {
			if( ! eq_compare( val, _node->key ) ) return iterator( _node );
			_node = _node->left;
        }
	return ncend();
}
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC

iterator       lower_bound()       { return ncbegin(); }
const_iterator lower_bound() const { return cbegin (); }
// returns first element greater then or equal val ( or past-the-end when no hits )
// if there are no equal elements this function returns same as upper_bound
iterator       lower_bound( T const & val )       { return lower_bound_internal( val ); }
const_iterator lower_bound( T const & val ) const { return lower_bound_internal( val ); }
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
iterator lower_bound_internal( T const & val ) const {
	if( empty() ) return ncend();
	node * _node( root );
	node * best_so_far( nullptr );
	while( true ) {
		if( eq_compare( val, _node->key ) ) {
			if( ! _node->has_left() ) return iterator( _node );
			best_so_far = _node;
			_node = _node->left;
		} else {
			if( ! _node->has_right() ) return lower_bound_internal_best_so_far( best_so_far );
			_node = _node->right;
		}
	}
}
iterator lower_bound_internal_best_so_far( node * const best_so_far ) const {
    if( best_so_far == nullptr ) return ncend();
    return iterator( best_so_far );
}
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC

iterator       upper_bound()       { return ncend(); }
const_iterator upper_bound() const { return cend (); }
// returns first element greater then val ( or past-the-end when no hits )
// if there are some equal elements this function returns iterator afterwards last equal element
iterator       upper_bound( T const & val )       { return upper_bound_internal( val ); }
const_iterator upper_bound( T const & val ) const { return upper_bound_internal( val ); }
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
iterator upper_bound_internal( T const & val ) const {
	if( empty() ) return ncend();
	node * _node( root );
	node * best_so_far( nullptr );
	while( true ) {
		if( ! eq_compare( _node->key, val ) ) {
			if( ! _node->has_left() ) return iterator( _node );
			best_so_far = _node;
			_node = _node->left;
		} else {
			if( ! _node->has_right() ) return upper_bound_internal_best_so_far( best_so_far );
			_node = _node->right;
		}
	}
}
iterator upper_bound_internal_best_so_far( node * const best_so_far ) const {
    if( best_so_far == nullptr ) return ncend();
    return iterator( best_so_far );
}
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC

std::pair<treap, treap> split( T const & val ) try {
	node * _new_node( new node( val, 1.1 ) );
    insert_node( _new_node );
    treap	treap_first( compare, URNG( radom ) ),
			treap_second( compare, URNG( radom ) );
    copy_recursion( treap_first .root, root->left );
    copy_recursion( treap_second.root, root->right );
    erase_internal( _new_node );
    return std::make_pair( treap_first, treap_second );
} catch( ... ) {
	if( root != nullptr && root->priority > 1.01 ) erase_internal( root );
	throw;
}
std::pair<treap, treap> split( T const & val ) const {
	treap treap_copy = treap( * this );
    return treap_copy.split( val );
}

// if true, than merge is logarithmic ( move ) or linear ( copy ), otherwise linear-logarithmic
static bool can_fast_merge( treap const & treap_first, treap const & treap_second ) {
    return (
		! treap_first .ne_compare( treap_second.lowest(), treap_first .highest() ) ||
		! treap_second.ne_compare( treap_first .lowest(), treap_second.highest() )
    );
}

static treap merge( treap const & treap_first, treap const & treap_second ) {
	if( treap_first .empty() ) return treap_second;
	if( treap_second.empty() ) return treap_first;

	if( ! treap_first.ne_compare( treap_second.lowest(), treap_first.highest() ) ) {
		return merge_fast( treap_first, treap_second );
    } else if( ! treap_second.ne_compare( treap_first.lowest(), treap_second.highest() ) ) {
		return merge_fast( treap_second, treap_first );
    } else
		return merge_insert( treap_first, treap_second );
}
static treap merge( treap && treap_first, treap && treap_second ) {
	if( treap_first .empty() ) return treap_second;
	if( treap_second.empty() ) return treap_first;

	if( ! treap_first.ne_compare( treap_second.lowest(), treap_first.highest() ) ) {
		return merge_fast( std::move( treap_first ), std::move( treap_second ) );
    } else if( ! treap_second.ne_compare( treap_first.lowest(), treap_second.highest() ) ) {
		return merge_fast( std::move( treap_second ), std::move( treap_first ) );
    } else
		return merge_insert( treap_first, treap_second );
}
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
static treap merge_fast( treap const & treap_first, treap const & treap_second ) {
    treap treap_merged( treap_first.compare, URNG( treap_first.radom ) );
    treap_merged.root = new node( *treap_first.ncrbegin().elem );
    copy_recursion( treap_merged.root->left , treap_first .root, treap_merged.root );
    copy_recursion( treap_merged.root->right, treap_second.root, treap_merged.root );
    treap_merged.erase_internal( treap_merged.root );
    return treap_merged;
}
static treap merge_fast( treap && treap_first, treap && treap_second ) {
    treap treap_merged( treap_first.compare, URNG( treap_first.radom ) );
    treap_merged.root = new node( *treap_first.ncrbegin().elem );
    treap_merged.root->insert_left ( treap_first .root );
    treap_merged.root->insert_right( treap_second.root );
    treap_first .root = nullptr;
    treap_second.root = nullptr;
    treap_merged.erase_internal( treap_merged.root );
    return treap_merged;
}
static treap merge_insert( treap treap_merged, treap const & treap_second ) {
	for( T const & val : treap_second )
		treap_merged.insert( val );
	return treap_merged;
}
static treap merge_insert( treap treap_merged, treap && treap_second ) {
	for( T const & val : treap_second )
		treap_merged.insert( val );
	treap_second.root = nullptr;
	return treap_merged;
}
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC

key_compare      key_comp  () const { return compare; }
value_compare    value_comp() const { return compare; }
random_generator random_gen() const { return radom  ; }
///////////////////////////// END ASK METHODS /////////////////////////////
///////////////////////////// BEGIN ITERATORS METHODS /////////////////////////////
iterator begin() {
	if( empty() ) return ncend();
	node * _node( root );
	while( _node->has_left() ) _node = _node->left;
	return iterator( _node );
}
const_iterator begin() const {
	if( empty() ) return cend();
	node * _node( root );
	while( _node->has_left() ) _node = _node->left;
	return const_iterator( _node );
}
const_iterator cbegin() const {
	if( empty() ) return cend();
	node * _node( root );
	while( _node->has_left() ) _node = _node->left;
	return const_iterator( _node );
}
reverse_iterator rbegin() {
	if( empty() ) return ncrend();
	node * _node( root );
	while( _node->has_right() ) _node = _node->right;
	return reverse_iterator( _node );
}
const_reverse_iterator rbegin() const {
	if( empty() ) return crend();
	node * _node( root );
	while( _node->has_right() ) _node = _node->right;
	return const_reverse_iterator( _node );
}
const_reverse_iterator crbegin() const {
	if( empty() ) return crend();
	node * _node( root );
	while( _node->has_right() ) _node = _node->right;
	return const_reverse_iterator( _node );
}
iterator               end  ()       { return iterator               ( root, true ); }
const_iterator         end  () const { return const_iterator         ( root, true ); }
const_iterator         cend () const { return const_iterator         ( root, true ); }
reverse_iterator       rend ()       { return reverse_iterator       ( root, true ); }
const_reverse_iterator rend () const { return const_reverse_iterator ( root, true ); }
const_reverse_iterator crend() const { return const_reverse_iterator ( root, true ); }
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
iterator ncbegin() const {
	if( empty() ) return ncend();
	node * _node( root );
	while( _node->has_left() ) _node = _node->left;
	return iterator( _node );
}
reverse_iterator ncrbegin() const {
	if( empty() ) return ncrend();
	node * _node( root );
	while( _node->has_right() ) _node = _node->right;
	return reverse_iterator( _node );
}
iterator         ncend () const { return iterator        ( root, true ); }
reverse_iterator ncrend() const { return reverse_iterator( root, true ); }
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
///////////////////////////// END ITERATORS METHODS /////////////////////////////
///////////////////////////// BEGIN CHANGING METHODS /////////////////////////////
iterator insert( T const & val ) {
	node * const _new_node( new node( val, radom ) );
	if( empty() ) return iterator( root = _new_node );
    insert_node( _new_node );
    return iterator( _new_node );
}
iterator insert( const_iterator const & it, T const & val ) { return insert( val ); }

template< typename... Args >
iterator emplace( Args... _args ) {
	node * const _new_node( new node( node_emplace(), radom, _args... ) );
	if( empty() ) return iterator( root = _new_node );
    insert_node( _new_node );
    return iterator( _new_node );
}
template< typename... Args >
iterator emplace_hint( const_iterator const & it, Args... _args ) { return emplace( _args... ); }

private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
void insert_node( node * const _new_node ) noexcept( noexcept( compare( _new_node->key, _new_node->key ) ) ) {
	node * _node( root );
    while( ! _new_node->has_ancestor() )
		if( eq_compare( _node->key, _new_node->key ) )
			if( _node->has_right() ) _node = _node->right;
			else                     _node->insert_right( _new_node );
		else
			if( _node->has_left() ) _node = _node->left;
			else                    _node->insert_left( _new_node );
	rotation( _new_node );
}
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC

size_type erase( T const & val ) try {
	size_type count = 0;
    while( true ) {
		const_iterator it = find( val );
		if( it == cend() ) return count;
		++count;
		erase( it );
    }
} catch( erase_past_the_end const & ) {
	throw erase_past_the_end( "size_type", "erase(T const &)" );
} catch( erase_invalid_iterator const & ) {
	throw erase_invalid_iterator( "size_type", "erase(T const &)" );
}

void erase( const_iterator & it ) {
	if( it == cend() ) throw erase_past_the_end( "void", "erase(const_iterator &)" );
	if( iterator_invalid( it ) ) throw erase_invalid_iterator( "void", "erase(const_iterator &)" );
	erase_internal( it.elem );
}
void erase( const_iterator const & it ) {
	if( it == cend() ) throw erase_past_the_end( "void", "erase(const_iterator const &)" );
	if( iterator_invalid( it ) ) throw erase_invalid_iterator( "void", "erase(const_iterator const &)" );
	erase_internal( it.elem );
}
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
void erase_internal( node * const _node ) noexcept {
	if( ! ( _node->has_ancestor() || _node->has_left() || _node->has_right() ) ) {
		delete _node;
		root = nullptr;
		return;
	}
	while( true ) {
		if( _node->has_left() )
			if( _node->has_right() ) rotate_one_of_heirs_with_highest_priority( _node );
			else                     rotate_left( _node->left );
		else if( _node->has_right() ) rotate_right( _node->right );
		else {
			delete _node;
			break;
		}
	}
}
bool iterator_invalid( const_iterator const & it ) noexcept {
	if( it.elem == nullptr ) return true;
	if( it.is_end && it.elem != root ) return true;
	if( get_root( it.elem ) != root ) return true;
	return false;
}
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC

void clear() noexcept {
	delete root;
	root = nullptr;
}
///////////////////////////// END CHANGING METHODS /////////////////////////////
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
///////////////////////////// BEGIN COMPARE METHODS /////////////////////////////
bool equal( T const & first, T const & second ) const noexcept( noexcept( compare( first, second ) ) )
	{ return ( compare( first, second ) == compare( second, first ) ); }
bool eq_compare( T const & first, T const & second ) const noexcept( noexcept( compare( first, second ) ) ) {
    if( equal( first, second ) ) return true;
    return compare( first, second );
}
bool ne_compare( T const & first, T const & second ) const noexcept( noexcept( compare( first, second ) ) ) {
    if( equal( first, second ) ) return false;
    return compare( first, second );
}
///////////////////////////// END COMPARE METHODS /////////////////////////////
///////////////////////////// BEGIN ROTATION /////////////////////////////
void rotation( node * const & _node ) noexcept {
    while( _node->has_ancestor() && _node->priority > _node->ancestor->priority )
		if( _node == _node->ancestor->right ) rotate_right( _node );
		else                                  rotate_left( _node );
    if( ! _node->has_ancestor() ) root = _node;
}
void rotate_left( node * const & _node ) noexcept {
	node * const x         ( _node       );
	node * const y         ( x->ancestor );
	node * const y_ancestor( y->ancestor );
	node * const B         ( x->right    );

	if( y_ancestor != nullptr )
		if( y_ancestor->left == y )	y_ancestor->left  = x;
		else						y_ancestor->right = x;
	else
		root = x;
    x->ancestor = y_ancestor;

    x->right    = y;
    y->ancestor = x;

    y->left = B;
    if( B != nullptr ) B->ancestor = y;
}
void rotate_right( node * const & _node ) noexcept {
	node * const y         ( _node       );
	node * const x         ( y->ancestor );
	node * const x_ancestor( x->ancestor );
	node * const B         ( y->left     );

	if( x_ancestor != nullptr )
		if( x_ancestor->left == x )	x_ancestor->left  = y;
		else						x_ancestor->right = y;
	else
		root = y;
	y->ancestor = x_ancestor;

    y->left     = x;
    x->ancestor = y;

	x->right = B;
	if( B != nullptr ) B->ancestor = x;
}
void rotate_one_of_heirs_with_highest_priority( node * const & _node ) noexcept {
	if( _node->left->priority > _node->right->priority ) rotate_left( _node->left );
	else                                                 rotate_right( _node->right );
}
///////////////////////////// END ROTATION /////////////////////////////
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
///////////////////////////// BEGIN NODE CLASS /////////////////////////////
struct node_emplace {};
class node {

public: //////////////////////////////////////////////////// PUBLIC
//////////////// BEGIN DECLARATIONS ////////////////
friend class treap;

const T key;
float   priority;
node *  ancestor;
node *  left;
node *  right;
//////////////// END DECLARATIONS ////////////////
//////////////// BEGIN CONSTRUCTORS ////////////////
explicit node( T const & _key, URNG & _radom )
	: node(
		_key,
		std::generate_canonical<float, std::numeric_limits<float>::digits>( _radom )
	) {}

explicit node( T const & _key, float _priority )
	: key( _key ),
	  priority( _priority ),
	  ancestor( nullptr ),
	  left( nullptr ),
      right( nullptr ) {}

template< typename... Args >
explicit node( node_emplace const &, URNG & _radom, Args... _args )
	: node(
		node_emplace(),
		std::generate_canonical<float, std::numeric_limits<float>::digits>( _radom ),
		_args...
	) {}

template< typename... Args >
explicit node( node_emplace const &, const float _priority, Args... _args )
	: key     ( _args... ),
      priority( _priority ),
	  ancestor( nullptr ),
	  left    ( nullptr ),
	  right   ( nullptr ) {}

node( node const & _other )
	: key     ( _other.key ),
      priority( _other.priority ),
      ancestor( nullptr ),
      left    ( nullptr ),
	  right   ( nullptr ) {}

node& operator=( node const & ) = delete;
node( node && )                 = delete;
node& operator=( node && )      = delete;
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN DESTRUCTOR ////////////////
~node() noexcept {
	if( has_ancestor() )
		if( this == ancestor->left ) ancestor->left  = nullptr;
		else                         ancestor->right = nullptr;
	if( has_left() )
		delete left;
	if( has_right() )
		delete right;
}
//////////////// END DESTRUCTOR ////////////////
//////////////// BEGIN METHODS ////////////////
size_type count() const {
    return count( left ) + count( right ) + 1;
}

static size_type count( node const * const & heir ) {
    if( heir == nullptr ) return 0;
    return heir->count();
}

bool has_left    () const noexcept { return left   != nullptr; }
bool has_right   () const noexcept { return right  != nullptr; }
bool has_ancestor() const noexcept { return ancestor != nullptr; }

void insert_left( node * const & _son ) noexcept {
    left           = _son;
    _son->ancestor = this;
}
void insert_right( node * const & _son ) noexcept {
    right          = _son;
    _son->ancestor = this;
}
//////////////// END METHODS ////////////////

};
///////////////////////////// END NODE CLASS /////////////////////////////
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
static node * iterator_increment_internal( node * _node, bool & is_end ) {
	if( is_end ) throw incrementing_past_the_end( "", "" );
	if( _node == nullptr ) throw operation_invalid( "", "" );
    if( _node->has_right() ) {
		_node = _node->right;
		while( _node->has_left() ) _node = _node->left;
		return _node;
    } else while( true )
		if( _node->has_ancestor() )
			if( _node == _node->ancestor->left ) return _node->ancestor;
			else                                 _node = _node->ancestor;
		else {
			is_end = true;
			return _node;
		}
}
static node * iterator_decrement_internal( node * _node, bool & is_end ) {
	if( _node == nullptr )
		if( is_end ) throw decrementing_before_begin( "", "" );
		else         throw operation_invalid( "", "" );
	if( is_end ) {
		is_end = false;
		while( _node->has_right() ) _node = _node->right;
		return _node;
	} else if( _node->has_left() ) {
		_node = _node->left;
		while( _node->has_right() ) _node = _node->right;
		return _node;
	} else while( true )
		if( _node->has_ancestor() )
			if( _node == _node->ancestor->right ) return _node->ancestor;
			else                                  _node = _node->ancestor;
		else throw decrementing_before_begin( "", "" );
}
static node * reverse_iterator_increment_internal( node * _node, bool & is_end ) {
	if( is_end ) throw incrementing_past_the_end( "", "" );
	if( _node == nullptr ) throw operation_invalid( "", "" );
    if( _node->has_left() ) {
		_node = _node->left;
		while( _node->has_right() ) _node = _node->right;
		return _node;
	} else while( true )
		if( _node->has_ancestor() )
			if( _node == _node->ancestor->right ) return _node->ancestor;
			else                                  _node = _node->ancestor;
		else {
			is_end = true;
			return _node;
		}
}
static node * reverse_iterator_decrement_internal( node * _node, bool & is_end ) {
	if( _node == nullptr )
		if( is_end ) throw decrementing_before_begin( "", "" );
		else         throw operation_invalid( "", "" );
	if( is_end ) {
		is_end = false;
		while( _node->has_left() ) _node = _node->left;
		return _node;
	} else if( _node->has_right() ) {
		_node = _node->right;
		while( _node->has_left() ) _node = _node->left;
		return _node;
	} else while( true )
		if( _node->has_ancestor() )
			if( _node == _node->ancestor->left ) return _node->ancestor;
			else                                 _node = _node->ancestor;
		else throw decrementing_before_begin( "", "" );
}
static node * get_root( node * _node ) {
	while( _node->has_ancestor() ) _node = _node->ancestor;
	return _node;
}
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
///////////////////////////// BEGIN ITERATOR CLASS /////////////////////////////
// default iterator
class iterator : public std::iterator<std::input_iterator_tag, const T> {

//////////////// BEGIN DECLARATIONS ////////////////
private: //////////////////////////////////////////////////// PRIVATE
friend class treap;

node * elem;
bool   is_end;
//////////////// END DECLARATIONS ////////////////
//////////////// BEGIN CONSTRUCTORS ////////////////
private: //////////////////////////////////////////////////// PRIVATE
explicit iterator( node * const _elem_ptr )
	: elem( _elem_ptr ), is_end( false ) {}
explicit iterator( node * const _elem_ptr, bool _is_end )
	: elem( _elem_ptr ), is_end( _is_end ) {}
public: //////////////////////////////////////////////////// PUBLIC
iterator()
	: elem( nullptr ), is_end( false ) {}

iterator( iterator const & _other )
	: elem( _other.elem ), is_end( _other.is_end ) {}
iterator & operator=( iterator const & _other ) {
	elem   = _other.elem;
	is_end = _other.is_end;
	return * this;
}

iterator( iterator && _other ) noexcept
	: elem( _other.elem ), is_end( _other.is_end ) {
		_other.elem = nullptr;
}
iterator & operator=( iterator && _other ) noexcept {
	elem        = _other.elem;
	_other.elem = nullptr;
	is_end      = _other.is_end;
	return * this;
}
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN OPERATORS ////////////////
bool operator==( iterator const & _other ) const noexcept { return is_end == _other.is_end && elem == _other.elem; }
bool operator!=( iterator const & _other ) const noexcept { return ! operator==( _other ); }
bool operator==( const_iterator const & _other ) const noexcept { return is_end == _other.is_end && elem == _other.elem; }
bool operator!=( const_iterator const & _other ) const noexcept { return ! operator==( _other ); }

iterator & operator++() try {
	elem = iterator_increment_internal( elem, is_end );
	return * this;
} catch( incrementing_past_the_end const & ) {
	throw incrementing_past_the_end( "iterator &", "iterator::operator++()" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "iterator &", "iterator::operator++()" );
}
iterator operator++( int ) try {
	iterator self( * this );
	operator++();
	return self;
} catch( incrementing_past_the_end const & ) {
	throw incrementing_past_the_end( "iterator", "iterator::operator++(int)" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "iterator", "iterator::operator++(int)" );
}

iterator & operator--() try {
	elem = iterator_decrement_internal( elem, is_end );
	return * this;
} catch( decrementing_before_begin const & ) {
	throw decrementing_before_begin( "iterator &", "iterator::operator--()" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "iterator &", "iterator::operator--()" );
}
iterator operator--( int ) try {
	iterator self( * this );
	operator--();
	return self;
} catch( decrementing_before_begin const & ) {
	throw decrementing_before_begin( "iterator", "iterator::operator--(int)" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "iterator", "iterator::operator--(int)" );
}

T const & operator*() const {
	if( is_end ) throw dereference_past_the_end( "T const &", "iterator::operator*()" );
	if( elem == nullptr ) throw operation_invalid( "T const &", "iterator::operator*()" );
	return elem->key;
}
//////////////// END OPERATORS ////////////////

};

// const default iterator
class const_iterator : public std::iterator<std::input_iterator_tag, const T>  {

//////////////// BEGIN DECLARATIONS ////////////////
private: //////////////////////////////////////////////////// PRIVATE
friend class treap;

node * elem;
bool   is_end;
//////////////// END DECLARATIONS ////////////////
//////////////// BEGIN CONSTRUCTORS ////////////////
private: //////////////////////////////////////////////////// PRIVATE
explicit const_iterator( node * const _elem_ptr )
	: elem( _elem_ptr ), is_end( false ) {}
explicit const_iterator( node * const _elem_ptr, bool _is_end )
	: elem( _elem_ptr ), is_end( _is_end ) {}
public: //////////////////////////////////////////////////// PUBLIC
const_iterator()
	: elem( nullptr ), is_end( false ) {}

const_iterator( iterator const & _it )
	: elem( _it.elem ), is_end( _it.is_end ) {}

const_iterator( const_iterator const & _other )
	: elem( _other.elem ), is_end( _other.is_end ) {}
const_iterator & operator=( const_iterator const & _other ) {
	elem   = _other.elem;
	is_end = _other.is_end;
	return * this;
}

const_iterator( const_iterator && _other ) noexcept
	: elem( _other.elem ), is_end( _other.is_end ) {
		_other.elem = nullptr;
}
const_iterator & operator=( const_iterator && _other ) noexcept {
	elem        = _other.elem;
	_other.elem = nullptr;
	is_end      = _other.is_end;
	return * this;
}
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN OPERATORS ////////////////
bool operator==( const_iterator const & _other ) const noexcept { return is_end == _other.is_end && elem == _other.elem; }
bool operator!=( const_iterator const & _other ) const noexcept { return ! operator==( _other ); }

const_iterator & operator++() try {
	elem = iterator_increment_internal( elem, is_end );
	return * this;
} catch( incrementing_past_the_end const & ) {
	throw incrementing_past_the_end( "const_iterator &", "const_iterator::operator++()" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "const_iterator &", "const_iterator::operator++()" );
}
const_iterator operator++( int ) try {
	const_iterator self( * this );
	operator++();
	return self;
} catch( incrementing_past_the_end const & ) {
	throw incrementing_past_the_end( "const_iterator", "const_iterator::operator++(int)" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "const_iterator", "const_iterator::operator++(int)" );
}

const_iterator & operator--() try {
	elem = iterator_decrement_internal( elem, is_end );
	return * this;
} catch( decrementing_before_begin const & ) {
	throw decrementing_before_begin( "const_iterator &", "const_iterator::operator--()" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "const_iterator &", "const_iterator::operator--()" );
}
const_iterator operator--( int ) try {
	const_iterator self( * this );
	operator--();
	return self;
} catch( decrementing_before_begin const & ) {
	throw decrementing_before_begin( "const_iterator", "const_iterator::operator--(int)" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "const_iterator", "const_iterator::operator--(int)" );
}

T const & operator*() const {
	if( is_end ) throw dereference_past_the_end( "T const &", "const_iterator::operator*()" );
	if( elem == nullptr ) throw operation_invalid( "T const &", "const_iterator::operator*()" );
	return elem->key;
}
//////////////// END OPERATORS ////////////////

};

// reverse iterator
class reverse_iterator : public std::iterator<std::input_iterator_tag, const T> {

//////////////// BEGIN DECLARATIONS ////////////////
private: //////////////////////////////////////////////////// PRIVATE
friend class treap;

node * elem;
bool         is_end;
//////////////// END DECLARATIONS ////////////////
//////////////// BEGIN CONSTRUCTORS ////////////////
private: //////////////////////////////////////////////////// PRIVATE
explicit reverse_iterator( node * const _elem_ptr )
	: elem( _elem_ptr ), is_end( false ) {}
explicit reverse_iterator( node * const _elem_ptr, bool _is_end )
	: elem( _elem_ptr ), is_end( _is_end ) {}
public: //////////////////////////////////////////////////// PUBLIC
reverse_iterator()
	: elem( nullptr ), is_end( false ) {}

reverse_iterator( reverse_iterator const & _other )
	: elem( _other.elem ), is_end( _other.is_end ) {}
reverse_iterator & operator=( reverse_iterator const & _other ) {
	elem   = _other.elem;
	is_end = _other.is_end;
}

reverse_iterator( reverse_iterator && _other ) noexcept
	: elem( _other.elem ), is_end( _other.is_end ) {
		_other.elem = nullptr;
}
reverse_iterator & operator=( reverse_iterator && _other ) noexcept {
	elem        = _other.elem;
	is_end      = _other.is_end;
	_other.elem = nullptr;
}
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN OPERATORS ////////////////
bool operator==( reverse_iterator const & _other ) const noexcept { return is_end == _other.is_end && elem == _other.elem; }
bool operator!=( reverse_iterator const & _other ) const noexcept { return ! operator==( _other ); }
bool operator==( const_reverse_iterator const & _other ) const noexcept { return is_end == _other.is_end && elem == _other.elem; }
bool operator!=( const_reverse_iterator const & _other ) const noexcept { return ! operator==( _other ); }

reverse_iterator & operator++() try {
	elem = reverse_iterator_increment_internal( elem, is_end );
	return * this;
} catch( incrementing_past_the_end const & ) {
	throw incrementing_past_the_end( "reverse_iterator &", "reverse_iterator::operator++()" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "reverse_iterator &", "reverse_iterator::operator++()" );
}
reverse_iterator operator++( int ) try {
	reverse_iterator self( * this );
	operator++();
	return self;
} catch( incrementing_past_the_end const & ) {
	throw incrementing_past_the_end( "reverse_iterator", "reverse_iterator::operator++(int)" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "reverse_iterator", "reverse_iterator::operator++(int)" );
}

reverse_iterator & operator--() try {
	elem = reverse_iterator_decrement_internal( elem, is_end );
    return * this;
} catch( decrementing_before_begin const & ) {
	throw decrementing_before_begin( "reverse_iterator &", "reverse_iterator::operator--()" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "reverse_iterator &", "reverse_iterator::operator--()" );
}
reverse_iterator operator--( int ) try {
	reverse_iterator self( * this );
	operator--();
	return self;
} catch( decrementing_before_begin const & ) {
	throw decrementing_before_begin( "reverse_iterator", "reverse_iterator::operator--(int)" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "reverse_iterator", "reverse_iterator::operator--(int)" );
}

T const & operator*() const {
	if( is_end ) throw dereference_past_the_end( "T const &", "reverse_iterator::operator*()" );
	if( elem == nullptr ) throw operation_invalid( "T const &", "reverse_iterator::operator*()" );
	return elem->key;
}

iterator base() const {
	if( is_end && elem == nullptr ) throw decrementing_before_begin( "iterator", "reverse_iterator::base()" );
	bool temp_is_end = is_end;
	try {
		return iterator( reverse_iterator_decrement_internal( elem, temp_is_end ), temp_is_end );
    } catch( decrementing_before_begin const & ) {
		return iterator( get_root( elem ), true );
    } catch( operation_invalid const & ) {
		throw operation_invalid( "iterator", "reverse_iterator::base()" );
    }
}
//////////////// END OPERATORS ////////////////

};

// const reverse iterator
class const_reverse_iterator : public std::iterator<std::input_iterator_tag, const T> {

//////////////// BEGIN DECLARATIONS ////////////////
private: //////////////////////////////////////////////////// PRIVATE
friend class treap;

node * elem;
bool         is_end;
//////////////// END DECLARATIONS ////////////////
//////////////// BEGIN CONSTRUCTORS ////////////////
private: //////////////////////////////////////////////////// PRIVATE
explicit const_reverse_iterator( node * const _elem_ptr )
	: elem( _elem_ptr ), is_end( false ) {}
explicit const_reverse_iterator( node * const _elem_ptr, bool _is_end )
	: elem( _elem_ptr ), is_end( _is_end ) {}
public: //////////////////////////////////////////////////// PUBLIC
const_reverse_iterator()
	: elem( nullptr ), is_end( false ) {}

const_reverse_iterator( reverse_iterator const & _it )
	: elem( _it.elem ), is_end( _it.is_end ) {}

const_reverse_iterator( const_reverse_iterator const & _other )
	: elem( _other.elem ), is_end( _other.is_end ) {}
const_reverse_iterator & operator=( const_reverse_iterator const & _other ) {
	elem   = _other.elem;
	is_end = _other.is_end;
}

const_reverse_iterator( const_reverse_iterator && _other ) noexcept
	: elem( _other.elem ), is_end( _other.is_end ) {
		_other.elem = nullptr;
}
const_reverse_iterator & operator=( const_reverse_iterator && _other ) noexcept {
	elem        = _other.elem;
	is_end      = _other.is_end;
	_other.elem = nullptr;
}
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN OPERATORS ////////////////
bool operator==( const_reverse_iterator const & _other ) const noexcept { return is_end == _other.is_end && elem == _other.elem; }
bool operator!=( const_reverse_iterator const & _other ) const noexcept { return ! operator==( _other ); }

const_reverse_iterator & operator++() try {
	elem = reverse_iterator_increment_internal( elem, is_end );
	return * this;
} catch( incrementing_past_the_end const & ) {
	throw incrementing_past_the_end( "const_reverse_iterator &", "const_reverse_iterator::operator++()" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "const_reverse_iterator &", "const_reverse_iterator::operator++()" );
}
const_reverse_iterator operator++( int ) try {
	const_reverse_iterator self( * this );
	operator++();
	return self;
} catch( incrementing_past_the_end const & ) {
	throw incrementing_past_the_end( "const_reverse_iterator", "const_reverse_iterator::operator++(int)" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "const_reverse_iterator", "const_reverse_iterator::operator++(int)" );
}

const_reverse_iterator & operator--() try {
	elem = reverse_iterator_decrement_internal( elem, is_end );
    return * this;
} catch( decrementing_before_begin const & ) {
	throw decrementing_before_begin( "const_reverse_iterator &", "const_reverse_iterator::operator--()" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "const_reverse_iterator &", "const_reverse_iterator::operator--()" );
}
const_reverse_iterator operator--( int ) try {
	const_reverse_iterator self( * this );
	operator--();
	return self;
} catch( decrementing_before_begin const & ) {
	throw decrementing_before_begin( "const_reverse_iterator", "const_reverse_iterator::operator--(int)" );
} catch( operation_invalid const & ) {
	throw operation_invalid( "const_reverse_iterator", "const_reverse_iterator::operator--(int)" );
}

T const & operator*() const {
	if( is_end ) throw dereference_past_the_end( "T const &", "const_reverse_iterator::operator*()" );
	if( elem == nullptr ) throw operation_invalid( "T const &", "const_reverse_iterator::operator*()" );
	return elem->key;
}

const_iterator base() const {
	if( is_end && elem == nullptr ) throw decrementing_before_begin( "const_iterator", "const_reverse_iterator::base()" );
	bool temp_is_end = is_end;
	try {
		return const_iterator( reverse_iterator_decrement_internal( elem, temp_is_end ), temp_is_end );
    } catch( decrementing_before_begin const & ) {
		return const_iterator( get_root( elem ), true );
    } catch( operation_invalid const & ) {
		throw operation_invalid( "const_iterator", "const_reverse_iterator::base()" );
    }
}
//////////////// END OPERATORS ////////////////

};
///////////////////////////// END ITERATOR CLASS /////////////////////////////

};

#endif
