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

// treap<T, CompareType, URNG>
template< class T, class CompareType = std::less<T>, class URNG = std::default_random_engine >
class treap {

///////////////////////////// BEGIN TYPEDEFS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
typedef T           key_type;
typedef T           value_type;
typedef CompareType key_compare;
typedef CompareType value_compare;
typedef URNG        random_generator;

typedef unsigned long long size_type;
///////////////////////////// END TYPEDEFS /////////////////////////////

///////////////////////////// BEGIN DECLARATIONS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
class iterator;
class const_iterator;
class reverse_iterator;
class const_reverse_iterator;
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
class       node;
node *      root;
node *      guard;
CompareType compare;
URNG        radom;
///////////////////////////// END DECLARATIONS /////////////////////////////
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
///////////////////////////// BEGIN ERRORS AND WARNINGS /////////////////////////////
static constexpr char const * const _err  = static_cast<char const * const>( "FatalError: " );
static constexpr char const * const _warn = static_cast<char const * const>( "Warn: "       );
static constexpr char const * const _note = static_cast<char const * const>( "Note: "       );
static constexpr void _err_lowest_on_empty() {
    std::cerr << _err << "Using lowest() on empty treap" << std::endl;
    exit( EXIT_FAILURE );
}
static constexpr void _err_highest_on_empty() {
    std::cerr << _err << "Using highest() on empty treap" << std::endl;
    exit( EXIT_FAILURE );
}
static constexpr void _err_dereference_past_the_end() {
	std::cerr << _err << "Attempt to dereference past-the-end iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static constexpr void _err_dereference_past_the_rend() {
	std::cerr << _err << "Attempt to dereference past-the-rend reverse iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static constexpr void _err_incrementing_past_the_end() {
	std::cerr << _err << "Attempt to increment past-the-end iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static constexpr void _err_incrementing_past_the_rend() {
	std::cerr << _err << "Attempt to increment past-the-rend reverse iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static constexpr void _err_decrementing_begin() {
    std::cerr << _err << "Attempt to decrement iterator pointing to begin" << std::endl;
    exit( EXIT_FAILURE );
}
static constexpr void _err_decrementing_rbegin() {
    std::cerr << _err << "Attempt to decrement reverse iterator pointing to rbegin" << std::endl;
    exit( EXIT_FAILURE );
}
static constexpr void _err_dereference_invalid() {
	std::cerr << _err << "Attempt to dereference no more valid iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static constexpr void _err_dereference_invalid_r() {
	std::cerr << _err << "Attempt to dereference no more valid reverse iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static constexpr void _err_incrementing_invalid() {
	std::cerr << _err << "Attemp to increment no more valid iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static constexpr void _err_decrementing_invalid() {
	std::cerr << _err << "Attemp to decrement no more valid iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static constexpr void _err_incrementing_invalid_r() {
	std::cerr << _err << "Attemp to increment no more valid reverse iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static constexpr void _err_decrementing_invalid_r() {
	std::cerr << _err << "Attemp to decrement no more valid reverse iterator" << std::endl;
	exit( EXIT_FAILURE );
}
static constexpr void _err_erase_past_the_end() {
    std::cerr << _err << "Attempt to erase past-the-end iterator" << std::endl;
    exit( EXIT_FAILURE );
}
static constexpr void _err_erase_past_the_rend() {
    std::cerr << _err << "Attempt to erase past-the-rend reverse iterator" << std::endl;
    exit( EXIT_FAILURE );
}
static constexpr void _err_erase_invalid_iterator() {
	std::cerr << _err << "Attempt to erase no more valid iterator" << std::endl;
    exit( EXIT_FAILURE );
}
static constexpr void _err_erase_invalid_iterator_r() {
	std::cerr << _err << "Attempt to erase no more valid reverse iterator" << std::endl;
    exit( EXIT_FAILURE );
}
static constexpr void _err_can_not_merge() {
	std::cerr << _err  << "Treaps can not be merged," << std::endl;
	std::cerr << _err  << "None of two treaps have higher lowest value than higher value of other treap," << std::endl;
	std::cerr << _note << "Use function treap::can_merge to check if treaps can be merged," << std::endl;
	std::cerr << _note << "Instead try to iterate over values from one treap and insert them to second treap" << std::endl;
	exit( EXIT_FAILURE );
}
static constexpr void _warn_merge_inequal_compare() {
	std::cerr << _warn << "Merging two treaps with inequal objects of type CompareType," << std::endl;
}
static constexpr void _warn_merge_inequal_compare_using_first() {
	_warn_merge_inequal_compare();
	std::cerr << _warn << "Using object of type CompareType from first treap" << std::endl;
}
static constexpr void _warn_merge_inequal_compare_using_second() {
	_warn_merge_inequal_compare();
	std::cerr << _warn << "Using object of type CompareType from second treap" << std::endl;
}
static constexpr void _warn_merge_inequal_radom() {
	std::cerr << _warn << "Merging two treaps with inequal objects of type URNG," << std::endl;
}
static constexpr void _warn_merge_inequal_radom_using_first() {
	_warn_merge_inequal_radom();
	std::cerr << _warn << "Using object of type URNG from first treap" << std::endl;
}
static constexpr void _warn_merge_inequal_radom_using_second() {
	_warn_merge_inequal_radom();
	std::cerr << _warn << "Using object of type URNG from second treap" << std::endl;
}
static constexpr void handle_exception() {
	std::cerr << _err << "Caught unrecognised exception!" << std::endl;
	exit( EXIT_FAILURE );
}
static void handle_exception( std::exception & e ) {
	std::cerr << _err << "Caught exception," << std::endl;
	std::cerr << _err << e.what() << std::endl;
	exit( EXIT_FAILURE );
}
///////////////////////////// END ERRORS AND WARNINGS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
///////////////////////////// BEGIN CONSTRUCTORS /////////////////////////////
constexpr treap()
	: treap( {} ) {}
explicit constexpr treap( std::initializer_list<T> const & _il )
	try: treap( _il, CompareType(), URNG( 720553160 ) ) {}
	catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

explicit treap( CompareType const & _compare )
	try: treap( {}, _compare, URNG( 755858114 ) ) {}
	catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

explicit treap( URNG && _radom )
	try: treap( {}, CompareType(), std::move( _radom ) ) {}
	catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

explicit treap( std::initializer_list<T> const & _il, CompareType const & _compare )
	try: treap( _il, _compare, URNG( 20553730 ) ) {}
	catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

explicit treap( std::initializer_list<T> const & _il, URNG && _radom )
	try: treap( _il, CompareType(), std::move( _radom ) ) {}
	catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

explicit treap( CompareType const & _compare, URNG && _radom )
	try: treap( {}, _compare, std::move( _radom ) ) {}
	catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

// main constructor
explicit treap( std::initializer_list<T> const & _il, CompareType const & _compare, URNG && _radom )
	try: root( nullptr ), guard( nullptr ), compare( _compare ), radom( std::move( _radom ) ) {
		for( T const & val : _il ) insert( val );
}   catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

// copy contructor
treap( treap const & _other )
	try : root( nullptr ), guard( nullptr ), compare( _other.compare ), radom( _other.radom ) {
		copy( _other );
}   catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }
// copy assigment
treap & operator=( treap const & _other ) try {
	clear();
	compare = CompareType( _other.compare );
	radom   = URNG( _other.radom );
	copy( _other );
	return * this;
} catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

// move contructor
treap( treap && _other ) try
	: root( std::move( _other.root ) ),
	  guard( nullptr ),
	  compare( std::move( _other.compare ) ),
	  radom( std::move( _other.radom ) ) {
		_other.root = nullptr;
} catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }
// move assigment
treap & operator=( treap && _other ) try {
	clear();
	root        = std::move( _other.root );
	compare     = std::move( _other.compare );
	radom       = std::move( _other.radom );
	_other.root = nullptr;
	return * this;
} catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

// destructor
~treap() { clear(); }

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
bool empty() const { return root == nullptr; }

T lowest() const {
    if( empty() ) _err_lowest_on_empty();
    return * begin();
}
T highest() const {
    if( empty() ) _err_highest_on_empty();
    return * rbegin();
}

bool contains( T const & val ) const { return find( val ) != cend(); }

iterator       find( T const & val )       { return find_internal( val ); }
const_iterator find( T const & val ) const { return find_internal( val ); }
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
iterator find_internal( T const & val ) const {
    node const * _node( root );
    while( _node != nullptr )
        if( m_compare( _node->key, val ) ) {
			if( m_compare( val, _node->key ) ) return iterator( _node );
			_node = _node->right;
        } else {
			if( ! m_compare( val, _node->key ) ) return iterator( _node );
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
		if( m_compare( val, _node->key ) ) {
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
		if( ! m_compare( _node->key, val ) ) {
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

std::pair<treap, treap> split( T const & val ) {
	node * const _new_node( new node( val, 1.1 ) );
    insert_node( _new_node );
    treap	treap_first( compare, URNG( radom ) ),
			treap_second( compare, URNG( radom ) );
    copy_recursion( treap_first .root, root->left );
    copy_recursion( treap_second.root, root->right );
    erase( iterator( _new_node ) );
    return std::make_pair( treap_first, treap_second );
}
std::pair<treap, treap> split( T const & val ) const {
	treap treap_copy = treap( * this );
    return treap_copy.split( val );
}

static bool can_merge( treap const & treap_first, treap const & treap_second ) {
    return (
		! treap_first .m_compare( treap_second.lowest(), treap_first .highest() )
		||
		! treap_second.m_compare( treap_first .lowest(), treap_second.highest() )
    );
}

static treap merge( treap const & treap_first, treap const & treap_second ) {
	if( treap_first .empty() ) return treap_second;
	if( treap_second.empty() ) return treap_first;

	if( ! treap_first.m_compare( treap_second.lowest(), treap_first.highest() ) ) {
		return merge_fast( treap_first, treap_second );
    } else if( ! treap_second.m_compare( treap_first.lowest(), treap_second.highest() ) ) {
		return merge_fast( treap_second, treap_first );
    } else _err_can_not_merge();
}
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
static treap merge_fast( treap const & treap_first, treap const & treap_second ) try {
    treap treap_merged( treap_first.compare, URNG( treap_first.radom ) );
    treap_merged.root = new node( *treap_first.ncrbegin().elem );
    copy_recursion( treap_merged.root->left , treap_first .root, treap_merged.root );
    copy_recursion( treap_merged.root->right, treap_second.root, treap_merged.root );
    treap_merged.erase( iterator( treap_merged.root ) );
    return treap_merged;
} catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC

key_compare      key_comp  () const try { return compare; } catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }
value_compare    value_comp() const try { return compare; } catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }
random_generator random_gen() const try { return radom  ; } catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }
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
	node const * _node( root );
	while( _node->has_right() ) _node = _node->right;
	return reverse_iterator( _node );
}
const_reverse_iterator rbegin() const {
	if( empty() ) return crend();
	node const * _node( root );
	while( _node->has_right() ) _node = _node->right;
	return const_reverse_iterator( _node );
}
const_reverse_iterator crbegin() const {
	if( empty() ) return crend();
	node const * _node( root );
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
	node const * _node( root );
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
iterator insert( const_iterator const & it, T const & val ) { insert( val ); }

template< typename... Args >
iterator emplace( Args... _args ) {
	node * const _new_node( new node( radom, _args... ) );
	if( empty() ) return iterator( root = _new_node );
    insert_node( _new_node );
    return iterator( _new_node );
}
template< typename... Args >
iterator emplace_hint( const_iterator const & it, Args... _args ) { emplace( _args... ); }

private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
void insert_node( node * const _new_node ) {
	node * _node( root );
    while( ! _new_node->has_ancestor() )
		if( m_compare( _node->key, _new_node->key ) )
			if( _node->has_right() ) _node = _node->right;
			else                     _node->insert_right( _new_node );
		else
			if( _node->has_left() ) _node = _node->left;
			else                    _node->insert_left( _new_node );
	rotation( _new_node );
}
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC

size_type erase( T const & val ) {
	size_type count = 0;
    while( true ) {
		const_iterator it = find( val );
		if( it == cend() ) return count;
		++count;
		erase( it );
    }
}

// returns copy of erased element
T erase( const_iterator it ) try {
	if( it == cend() ) _err_erase_past_the_end();
	if( iterator_invalid( it ) ) _err_erase_invalid_iterator();
	T value_copy = T( *it );
	erase_internal( it.elem );
	return value_copy;
} catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
void erase_internal( node * const _node ) {
	insert_guard( _node );
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
	erase_guard();
}
bool iterator_invalid( const_iterator it ) { return iterator_elem_invalid( it.elem ); }
bool iterator_elem_invalid( node const * elem ) {
	if( elem == nullptr ) return true;
	while( elem->has_ancestor() ) elem = elem->ancestor;
	if( elem != root ) return true;
	return false;
}
void insert_guard( node const * const _node ) {
	guard = new node( *_node );
	guard->priority = 1.2;
	guard->left     = root;
	if( root != nullptr ) root->ancestor = guard;
	root = guard;
}
void erase_guard() {
    root = guard->left;
    if( root != nullptr ) root->ancestor = nullptr;
    delete guard;
    guard = nullptr;
}
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC

void clear() {
    node * _node( root );
    while( _node != nullptr )
		if( _node->has_left() )       _node = _node->left;
		else if( _node->has_right() ) _node = _node->right;
		else {
			node * _delete = _node;
			_node = _node->ancestor;
			delete _delete;
		}
	root = nullptr;
}
///////////////////////////// END CHANGING METHODS /////////////////////////////
private: /////////////////////////////////////////////////////////////////////////////////// PRIVATE
///////////////////////////// BEGIN COMPARE METHODS /////////////////////////////
bool equal( T const & first, T const & second ) const { return ( compare( first, second ) == compare( second, first ) ); }
bool m_compare( T const & first, T const & second ) const try {
    if( equal( first, second ) ) return true;
    return compare( first, second );
} catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }
///////////////////////////// END COMPARE METHODS /////////////////////////////
///////////////////////////// BEGIN ROTATION /////////////////////////////
void rotation( node * const _node ) {
    while( _node->has_ancestor() && _node->priority > _node->ancestor->priority )
		if( _node == _node->ancestor->right ) rotate_right( _node );
		else                                  rotate_left( _node );
    if( ! _node->has_ancestor() ) root = _node;
}
void rotate_left( node * const _node ) {
	node * const x         ( _node       );
	node * const y         ( x->ancestor );
	node * const y_ancestor( y->ancestor );
	node * const B         ( x->right    );

	if( y_ancestor != nullptr )
		if( y_ancestor->left == y )	y_ancestor->left  = x;
		else						y_ancestor->right = x;
    x->ancestor = y_ancestor;

    x->right    = y;
    y->ancestor = x;

    y->left = B;
    if( B != nullptr ) B->ancestor = y;
}
void rotate_right( node * const _node ) {
	node * const y         ( _node       );
	node * const x         ( y->ancestor );
	node * const x_ancestor( x->ancestor );
	node * const B         ( y->left     );

	if( x_ancestor != nullptr )
		if( x_ancestor->left == x )	x_ancestor->left  = y;
		else						x_ancestor->right = y;
	y->ancestor = x_ancestor;

    y->left     = x;
    x->ancestor = y;

	x->right = B;
	if( B != nullptr ) B->ancestor = x;
}
void rotate_one_of_heirs_with_highest_priority( node * const _node ) {
	if( _node->left->priority > _node->right->priority ) rotate_left( _node->left );
	else                                                 rotate_right( _node->right );
}
///////////////////////////// END ROTATION /////////////////////////////
///////////////////////////// BEGIN NODE CLASS /////////////////////////////
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
	try: node(
		_key,
		std::generate_canonical<float, 0>( _radom )
	) {}
	catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

explicit constexpr node( T const & _key, float _priority )
	try: key( _key ),
	     priority( _priority ),
	     ancestor( nullptr ),
	     left( nullptr ),
         right( nullptr ) {}
	catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

template< typename... Args >
explicit node( URNG & _radom, Args... _args )
	try: node(
		std::generate_canonical<float, 0>( _radom ),
		_args...
	) {}
	catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

template< typename... Args >
explicit constexpr node( const float _priority, Args... _args )
	try: key     ( _args... ),
	     priority( _priority ),
	     ancestor( nullptr ),
	     left    ( nullptr ),
	     right   ( nullptr ) {}
	catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

node( node const & _other )
	try: key     ( _other.key ),
         priority( _other.priority ),
         ancestor( nullptr ),
         left    ( nullptr ),
		 right   ( nullptr ) {}
	catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

node& operator=( node const & ) = delete;
node( node && )                 = delete;
node& operator=( node && )      = delete;
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN DESTRUCTOR ////////////////
~node() {
	if( has_ancestor() )
		if( this == ancestor->left ) ancestor->left  = nullptr;
		else                         ancestor->right = nullptr;
}
//////////////// END DESTRUCTOR ////////////////
//////////////// BEGIN METHODS ////////////////
bool has_left    () const { return left   != nullptr; }
bool has_right   () const { return right  != nullptr; }
bool has_ancestor() const { return ancestor != nullptr; }

void insert_left( node * const & _son ) {
    left           = _son;
    _son->ancestor = this;
}
void insert_right( node * const & _son ) {
    right          = _son;
    _son->ancestor = this;
}
//////////////// END METHODS ////////////////

};
///////////////////////////// END NODE CLASS /////////////////////////////
public: /////////////////////////////////////////////////////////////////////////////////// PUBLIC
///////////////////////////// BEGIN ITERATOR CLASS /////////////////////////////
// default iterator
class iterator : public std::iterator<std::input_iterator_tag, T> {

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
iterator( iterator const & _other )
	: elem( _other.elem ), is_end( _other.is_end ) {}
iterator & operator=( iterator const & _other ) {
	elem   = _other.elem;
	is_end = _other.is_end;
	return * this;
}
iterator( iterator && _other )
	: elem( _other.elem ), is_end( _other.is_end ) {
		_other.elem = nullptr;
}
iterator & operator=( iterator && _other ) {
	elem        = _other.elem;
	is_end      = _other.is_end;
	_other.elem = nullptr;
	return * this;
}
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN OPERATORS ////////////////
bool operator==( iterator const & _other ) const { return is_end == _other.is_end && elem == _other.elem; }
bool operator!=( iterator const & _other ) const { return ! operator==( _other ); }

iterator & operator++() {
	if( elem == nullptr ) _err_incrementing_invalid();
	if( is_end ) _err_incrementing_past_the_end();
    node * _node( elem );
    elem = nullptr;
    if( _node->has_right() ) {
		_node = _node->right;
		while( _node->has_left() ) _node = _node->left;
		elem = _node;
    } else while( elem == nullptr )
		if( _node->has_ancestor() )
			if( _node == _node->ancestor->left ) elem  = _node->ancestor;
			else                                 _node = _node->ancestor;
		else {
			elem   = _node;
			is_end = true;
		}
    return * this;
}
iterator operator++( int ) {
	iterator self( * this );
	operator++();
	return self;
}

iterator & operator--() {
	if( elem == nullptr ) _err_decrementing_invalid();
	node * _node( elem );
	elem = nullptr;
	if( is_end ) {
		is_end = false;
		while( _node->has_right() ) _node = _node->right;
		elem = _node;
	} else if( _node->has_left() ) {
		_node = _node->left;
		while( _node->has_right() ) _node = _node->right;
		elem = _node;
	} else while( elem == nullptr )
		if( _node->has_ancestor() )
			if( _node == _node->ancestor->right ) elem  = _node->ancestor;
			else                                  _node = _node->ancestor;
		else _err_decrementing_begin();
	return * this;
}
iterator operator--( int ) {
	iterator self( * this );
	operator--();
	return self;
}

T const & operator*() const try {
	if( is_end ) _err_dereference_past_the_end();
	if( elem == nullptr ) _err_dereference_invalid();
	return elem->key;
} catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

operator const_iterator() { return const_iterator( * this ); }
//////////////// END OPERATORS ////////////////

};

// const default iterator
class const_iterator : public std::iterator<std::input_iterator_tag, T>  {

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
const_iterator( iterator const & _it )
	: elem( _it.elem ), is_end( _it.is_end ) {}
const_iterator( const_iterator const & _other )
	: elem( _other.elem ), is_end( _other.is_end ) {}
const_iterator & operator=( const_iterator const & _other ) {
	elem   = _other.elem;
	is_end = _other.is_end;
	return * this;
}
const_iterator( const_iterator && _other )
	: elem( _other.elem ), is_end( _other.is_end ) {
		_other.elem = nullptr;
}
const_iterator & operator=( const_iterator && _other ) {
	elem        = _other.elem;
	is_end      = _other.is_end;
	_other.elem = nullptr;
	return * this;
}
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN OPERATORS ////////////////
bool operator==( const_iterator const & _other ) const { return is_end == _other.is_end && elem == _other.elem; }
bool operator!=( const_iterator const & _other ) const { return ! operator==( _other ); }

const_iterator & operator++() {
	if( elem == nullptr ) _err_incrementing_invalid();
	if( is_end ) _err_incrementing_past_the_end();
    node * _node( elem );
    elem = nullptr;
    if( _node->has_right() ) {
		_node = _node->right;
		while( _node->has_left() ) _node = _node->left;
		elem = _node;
    } else while( elem == nullptr )
		if( _node->has_ancestor() )
			if( _node == _node->ancestor->left ) elem  = _node->ancestor;
			else                                 _node = _node->ancestor;
		else {
			elem   = _node;
			is_end = true;
		}
    return * this;
}
const_iterator operator++( int ) {
	const_iterator self( * this );
	operator++();
	return self;
}

const_iterator & operator--() {
	if( elem == nullptr ) _err_decrementing_invalid();
	node * _node( elem );
	elem = nullptr;
	if( is_end ) {
		is_end = false;
		while( _node->has_right() ) _node = _node->right;
		elem = _node;
	} else if( _node->has_left() ) {
		_node = _node->left;
		while( _node->has_right() ) _node = _node->right;
		elem = _node;
	} else while( elem == nullptr )
		if( _node->has_ancestor() )
			if( _node == _node->ancestor->right ) elem  = _node->ancestor;
			else                                  _node = _node->ancestor;
		else _err_decrementing_begin();
	return * this;
}
const_iterator operator--( int ) {
	const_iterator self( * this );
	operator--();
	return self;
}

T const & operator*() const try {
	if( is_end ) _err_dereference_past_the_end();
	if( elem == nullptr ) _err_dereference_invalid();
	return elem->key;
} catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }
//////////////// END OPERATORS ////////////////

};

// reverse iterator
class reverse_iterator : public std::iterator<std::input_iterator_tag, T> {

//////////////// BEGIN DECLARATIONS ////////////////
private: //////////////////////////////////////////////////// PRIVATE
friend class treap;

node const * elem;
bool         is_end;
//////////////// END DECLARATIONS ////////////////
//////////////// BEGIN CONSTRUCTORS ////////////////
private: //////////////////////////////////////////////////// PRIVATE
explicit reverse_iterator( node const * const _elem_ptr )
	: elem( _elem_ptr ), is_end( false ) {}
explicit reverse_iterator( node const * const _elem_ptr, bool _is_end )
	: elem( _elem_ptr ), is_end( _is_end ) {}
public: //////////////////////////////////////////////////// PUBLIC
reverse_iterator( reverse_iterator const & _other )
	: elem( _other.elem ), is_end( _other.is_end ) {}
reverse_iterator & operator=( reverse_iterator const & _other ) {
	elem   = _other.elem;
	is_end = _other.is_end;
}
reverse_iterator( reverse_iterator && _other )
	: elem( _other.elem ), is_end( _other.is_end ) {
		_other.elem = nullptr;
}
reverse_iterator & operator=( reverse_iterator && _other ) {
	elem        = _other.elem;
	is_end      = _other.is_end;
	_other.elem = nullptr;
}
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN OPERATORS ////////////////
bool operator==( reverse_iterator const & _other ) const { return is_end == _other.is_end && elem == _other.elem; }
bool operator!=( reverse_iterator const & _other ) const { return ! operator==( _other ); }

reverse_iterator & operator++() {
	if( elem == nullptr ) _err_incrementing_invalid_r();
	if( is_end ) _err_incrementing_past_the_end();
	node const * _node( elem );
	elem = nullptr;
    if( _node->has_left() ) {
		_node = _node->left;
		while( _node->has_right() ) _node = _node->right;
		elem = _node;
	} else while( elem == nullptr )
		if( _node->has_ancestor() )
			if( _node == _node->ancestor->right ) elem  = _node->ancestor;
			else                                  _node = _node->ancestor;
		else {
			elem   = _node;
			is_end = true;
		}
	return * this;
}
reverse_iterator operator++( int ) {
	reverse_iterator self( * this );
	operator++();
	return self;
}

reverse_iterator & operator--() {
	if( elem == nullptr ) _err_decrementing_invalid_r();
	node const * _node( elem );
	elem = nullptr;
	if( is_end ) {
		is_end = false;
		while( _node->has_left() ) _node = _node->left;
		elem = _node;
	} else if( _node->has_right() ) {
		_node = _node->right;
		while( _node->has_left() ) _node = _node->left;
		elem = _node;
	} else while( elem == nullptr )
		if( _node->has_ancestor() )
			if( _node == _node->ancestor->left ) elem  = _node->ancestor;
			else                                 _node = _node->ancestor;
		else _err_decrementing_rbegin();
    return * this;
}
reverse_iterator operator--( int ) {
	reverse_iterator self( * this );
	operator--();
	return self;
}

T const & operator*() const try {
	if( is_end ) _err_dereference_past_the_rend();
	if( elem == nullptr ) _err_dereference_invalid_r();
	return elem->key;
} catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }

operator const_reverse_iterator() { return const_reverse_iterator( * this ); }
//////////////// END OPERATORS ////////////////

};

// const reverse iterator
class const_reverse_iterator : public std::iterator<std::input_iterator_tag, T> {

//////////////// BEGIN DECLARATIONS ////////////////
private: //////////////////////////////////////////////////// PRIVATE
friend class treap;

node const * elem;
bool         is_end;
//////////////// END DECLARATIONS ////////////////
//////////////// BEGIN CONSTRUCTORS ////////////////
private: //////////////////////////////////////////////////// PRIVATE
explicit const_reverse_iterator( node const * const _elem_ptr )
	: elem( _elem_ptr ), is_end( false ) {}
explicit const_reverse_iterator( node const * const _elem_ptr, bool _is_end )
	: elem( _elem_ptr ), is_end( _is_end ) {}
public: //////////////////////////////////////////////////// PUBLIC
const_reverse_iterator( reverse_iterator const & _it )
	: elem( _it.elem ), is_end( _it.is_end ) {}
const_reverse_iterator( const_reverse_iterator const & _other )
	: elem( _other.elem ), is_end( _other.is_end ) {}
const_reverse_iterator & operator=( const_reverse_iterator const & _other ) {
	elem   = _other.elem;
	is_end = _other.is_end;
}
const_reverse_iterator( const_reverse_iterator && _other )
	: elem( _other.elem ), is_end( _other.is_end ) {
		_other.elem = nullptr;
}
const_reverse_iterator & operator=( const_reverse_iterator && _other ) {
	elem        = _other.elem;
	is_end      = _other.is_end;
	_other.elem = nullptr;
}
//////////////// END CONSTRUCTORS ////////////////
//////////////// BEGIN OPERATORS ////////////////
bool operator==( const_reverse_iterator const & _other ) const { return is_end == _other.is_end && elem == _other.elem; }
bool operator!=( const_reverse_iterator const & _other ) const { return ! operator==( _other ); }

const_reverse_iterator & operator++() {
	if( elem == nullptr ) _err_incrementing_invalid_r();
	if( is_end ) _err_incrementing_past_the_end();
	node const * _node( elem );
	elem = nullptr;
    if( _node->has_left() ) {
		_node = _node->left;
		while( _node->has_right() ) _node = _node->right;
		elem = _node;
	} else while( elem == nullptr )
		if( _node->has_ancestor() )
			if( _node == _node->ancestor->right ) elem  = _node->ancestor;
			else                                  _node = _node->ancestor;
		else {
			elem   = _node;
			is_end = true;
		}
	return * this;
}
const_reverse_iterator operator++( int ) {
	const_reverse_iterator self( * this );
	operator++();
	return self;
}

const_reverse_iterator & operator--() {
	if( elem == nullptr ) _err_decrementing_invalid_r();
	node const * _node( elem );
	elem = nullptr;
	if( is_end ) {
		is_end = false;
		while( _node->has_left() ) _node = _node->left;
		elem = _node;
	} else if( _node->has_right() ) {
		_node = _node->right;
		while( _node->has_left() ) _node = _node->left;
		elem = _node;
	} else while( elem == nullptr )
		if( _node->has_ancestor() )
			if( _node == _node->ancestor->left ) elem  = _node->ancestor;
			else                                 _node = _node->ancestor;
		else _err_decrementing_rbegin();
    return * this;
}
const_reverse_iterator operator--( int ) {
	const_reverse_iterator self( * this );
	operator--();
	return self;
}

T const & operator*() const try {
	if( is_end ) _err_dereference_past_the_rend();
	if( elem == nullptr ) _err_dereference_invalid_r();
	return elem->key;
} catch( std::exception & e ) { handle_exception( e ); } catch(...) { handle_exception(); }
//////////////// END OPERATORS ////////////////

};
///////////////////////////// END ITERATOR CLASS /////////////////////////////

void wypisz_spacje( int ile ) const {
    while( ile-- ) std::cout << "  ";
}

void wypisz_node( node const * const _node, int poziom, std::string d ) const {
	if( _node == nullptr ) return;

	wypisz_spacje( poziom );

	std::cout << d << _node->key << " " << _node->priority;
	if( _node->has_ancestor() ) std::cout << " " << _node->ancestor->key;
	else std::cout << " nptr";
	if( _node->has_left() ) std::cout << " " << _node->left->key;
	else std::cout << " nptr";
	if( _node->has_right() ) std::cout << " " << _node->right->key;
	else std::cout << " nptr";
	std::cout << std::endl;

	wypisz_node( _node->left, poziom+1, "l " );
	wypisz_node( _node->right, poziom+1, "p " );
}

void wypisz() const {
	if( empty() ) std::cout << "empty" << std::endl;
	wypisz_node( root, 0, "" );
	std::cout << std::endl;
}

};

#endif
