#include <iostream>
#include <string>
#include <set>
#include <chrono>
#include <stdexcept>
#include <algorithm>

#include "treap.h"

#include <sys/resource.h>
#include <sys/time.h>

#include <gtest/gtest.h>

#define DEBUG false
#if DEBUG == true
	#define dout std::clog
#else
	#define dout if( false ) std::clog
#endif

long long count_constructed = 0, count_destroyed = 0;
struct B {
	std::string sd;
	B( std::string _sd ): sd( _sd ) {}

	bool operator==( B const & _other ) const {
		return ( sd == _other.sd );
	}
};
struct C {
	std::string sd;
	C( std::string _sd ): sd( _sd ) {}

		bool operator==( C const & _other ) const {
		return ( sd == _other.sd );
	}
};
struct A {
    B b;
    C c;
    std::string sd;
    A(std::string sd1, std::string sd2, std::string sd3): b( sd1 ), c( sd2 ), sd( sd3 ) { ++count_constructed; }
    A( A const & _other ): b( _other.b ), c( _other.c ), sd( _other.sd ) { ++count_constructed; }
    ~A() noexcept { ++count_destroyed; }
    A operator=( A const & ) = delete;
    A operator=( A && ) = delete;
    A( A && _other ): b( std::move( _other.b ) ), c( std::move( _other.c ) ), sd( std::move( _other.sd ) ) { ++count_constructed; }

	bool operator==( A const & _other ) const {
		return ( b == _other.b && c == _other.c && sd == _other.sd );
	}

	bool operator<( A const & _other ) const {
		return ( sd < _other.sd );
	}

    friend std::ostream& operator<<( std::ostream &, A const & );
};
std::ostream& operator<<( std::ostream & wyjscie, A const & a ) {
	return wyjscie << a.b.sd << " " << a.c.sd << " " << a.sd;
}
struct cmp_t {
    bool operator() ( treap<int> const & a, treap<int> const & b ) const {

		if( a.empty() ) {
			if( b.empty() )
				return false;
			return b.highest() < 0;
		}

		if( b.empty() ) {
			return a.highest() < 0;
		}

		return a.highest() < b.highest();

	}
	bool operator() ( treap<A> const & a, treap<A> const & b ) const {

		if( a.empty() ) {
			if( b.empty() )
				return false;
			return b.highest().sd < "";
		}

		if( b.empty() ) {
			return a.highest().sd < "";
		}

		return a.highest() < b.highest();

	}
};

struct cmp_A {
	int num;
    bool operator() ( A const & a, A const & b ) const {
        return a < b;
    }
    cmp_A() = delete;
    cmp_A(int a): num(a) {}
    bool operator == ( cmp_A const & _other ) const {
		return ( num == _other.num );
    }
    bool operator != ( cmp_A const & _other ) const {
		return ( num != _other.num );
    }
};

TEST( TREAP, REVERSE_ITERATOR_BASE ) {
	dout << __LINE__ << std::endl;

	treap<int> abba( {1, 2, 3, 5, 6, 7, 8, 9, 0, 0, 9, 8, 7, 6, 5, 3, 2, 1} );

	dout << __LINE__ << std::endl;

	for( int i = 1 ; i <= 1000 ; ++i )
		abba.emplace( 4 );

	dout << __LINE__ << std::endl;

	for( int i = 1 ; i <= 10000 ; ++i ) {
		abba.emplace( i );
		abba.emplace( 4 );
		abba.emplace( i );
	}

	dout << __LINE__ << std::endl;

	for( int i = 1 ; i <= 1000 ; ++i )
		abba.emplace( 4 );

	dout << __LINE__ << std::endl;

	auto r_it = abba.rbegin();

	dout << __LINE__ << std::endl;

	for( int i = 1 ; i <= 2000 ; ++i ) {
		r_it++;
		r_it--;
		++r_it;
		--r_it;
		++r_it;
	}

	dout << __LINE__ << std::endl;

	EXPECT_EQ( *r_it, 9000 );

	dout << __LINE__ << std::endl;

	auto it = r_it.base();

	dout << __LINE__ << std::endl;

	EXPECT_EQ( *it, 9001 );

	dout << __LINE__ << std::endl;
//
//	EXPECT_FALSE( (it--).elem == r_it.elem );
//
//	dout << __LINE__ << std::endl;
//
//	EXPECT_TRUE( (it++).elem == r_it.elem );
//
//	dout << __LINE__ << std::endl;
//
//	EXPECT_TRUE( (--it).elem == r_it.elem );
//
//	dout << __LINE__ << std::endl;
}

TEST( TREAP, INSERT ) {
	dout << __LINE__ << std::endl;

    treap<const int> abba;

	dout << __LINE__ << std::endl;

    for( int i = 1 ; i <= 10000 ; ++i ) {
        auto it = abba.insert( i );

        ASSERT_EQ( it, --abba.end() );

        it = abba.insert( --abba.end(), i );

        ASSERT_EQ( i, *it );
    }

	dout << __LINE__ << std::endl;

	treap<int>::size_type count = 0;

    for( const int a : abba )
		++count;

	dout << __LINE__ << std::endl;

	EXPECT_EQ( count, 20000 );

	dout << __LINE__ << std::endl;
}

TEST( TREAP, EMPLACE ) {
	{
	dout << __LINE__ << std::endl;

    treap<A, std::less<A>, std::mt19937> abba;

	dout << __LINE__ << std::endl;

	std::string d = "";

	dout << __LINE__ << std::endl;

    for( int i = 1 ; i <= 10000 ; ++i ) {
        auto it = abba.emplace( d, d += "a", d );

        ASSERT_EQ( it, --abba.end() );

		d += "a";

        it = abba.emplace_hint( --abba.end(), d, d, d );

        ASSERT_EQ( A( d, d, d ), *it );
    }

	dout << __LINE__ << std::endl;

	treap<int>::size_type count = 0;

	dout << __LINE__ << std::endl;

    for( const A a : abba )
		++count;

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 20000, count );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 20000, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;
	}

	EXPECT_EQ( 0, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;
}

TEST( TREAP, LOWER_AND_UPPER_BOUND ) {
	dout << __LINE__ << std::endl;

    treap<int> abba;

	dout << __LINE__ << std::endl;

    for( int i = 1 ; i <= 10000 ; ++i ) {
        abba.insert( i );
        abba.insert( 4 );
        abba.emplace( i );
    }

	dout << __LINE__ << std::endl;

    auto it_s = abba.lower_bound( 4 );

    dout << __LINE__ << std::endl;

    auto it_e = abba.upper_bound( 4 );

    dout << __LINE__ << std::endl;

    EXPECT_EQ( 4, *it_s );

    dout << __LINE__ << std::endl;

    EXPECT_EQ( 5, *it_e );

    dout << __LINE__ << std::endl;

    treap<int>::size_type count = 0;

	dout << __LINE__ << std::endl;

    for( ; it_s != it_e ; ++it_s )
		count++;

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 10002, count );

	dout << __LINE__ << std::endl;
}

TEST( TREAP, BEGIN_AND_END ) {
	{
	dout << __LINE__ << std::endl;

    treap<A> abba{ A("a", "b", "c"), A("a", "b", "d") };

    dout << __LINE__ << std::endl;

	EXPECT_EQ( A("a", "b", "c"), *abba.begin() );

    dout << __LINE__ << std::endl;

	EXPECT_EQ( A("a", "b", "d"), *(--abba.cend()) );

    dout << __LINE__ << std::endl;

	EXPECT_EQ( ++abba.cbegin(), --abba.end() );

    dout << __LINE__ << std::endl;

    EXPECT_EQ( 2, count_constructed - count_destroyed );

    dout << __LINE__ << std::endl;
	}

	EXPECT_EQ( 0, count_constructed - count_destroyed );

    dout << __LINE__ << std::endl;
}

TEST( TREAP, COPY_AND_DESTROY ) {
	{
	dout << __LINE__ << std::endl;

	treap<A> abba{ A("a", "b", "c"), A("a", "b", "d"), A("a", "b", "e") };

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 3, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;

	treap<A> abbb( abba );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 6, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;

	treap<A> abbc = abbb;

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 9, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;

	treap<A> * p = new treap<A>( abbc );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 12, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 3, p->size() );

	dout << __LINE__ << std::endl;

	delete p;

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 9, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;
	}

	EXPECT_EQ( 0, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;
}

TEST( TREAP, MOVE_AND_DESTROY ) {
	{
	dout << __LINE__ << std::endl;

	treap<A> abba{ A("a", "b", "c"), A("a", "b", "d"), A("a", "b", "e") };

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 3, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;

	treap<A> abbb( std::move( abba ) );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 3, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;

	treap<A> abbc = std::move( abbb );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 3, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;

	treap<A> * p = new treap<A>( std::move( abbc ) );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 3, p->size() );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 3, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;

	delete p;

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 0, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;
	}

	EXPECT_EQ( 0, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;
}

TEST( TREAP, CONSTRUCTORS ) {
	{
	dout << __LINE__ << std::endl;

	std::mt19937 gen( 2536345732744LL ), gen_copy( gen );
	cmp_A cmpA1( 4 ), cmpA2( 5 );

	dout << __LINE__ << std::endl;

    treap<treap<A>, cmp_t, std::mt19937> abba( { treap<A>(), treap<A>{ A("","",""), A("","","a"), A("","","b") }, treap<A>{ A("","","d"), A("","","d"), A("","","d") } }, cmp_t() );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 6, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;

	treap<A, cmp_A, std::mt19937> abbb( cmpA1, std::move( gen ) );

    dout << __LINE__ << std::endl;

	treap<A, cmp_A> abbc( cmpA1 );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( abbb.key_comp(), abbc.key_comp() );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( abbc.value_comp(), abbb.value_comp() );

	dout << __LINE__ << std::endl;

	treap<A, cmp_A, std::mt19937> abbd( cmpA2, std::move( gen_copy ) );

	dout << __LINE__ << std::endl;

	EXPECT_NE( abbd.key_comp(), abbc.key_comp() );

	dout << __LINE__ << std::endl;

	EXPECT_NE( abbb.value_comp(), abbd.value_comp() );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( abbb.random_gen(), abbd.random_gen() );

	dout << __LINE__ << std::endl;
    }

	EXPECT_EQ( 0, count_constructed - count_destroyed );

	dout << __LINE__ << std::endl;
}

TEST( MEMORY, CLEAR ) {
	dout << __LINE__ << std::endl;

	rlimit a;
	a.rlim_cur = rlim_t( 0 );
	a.rlim_max = rlim_t( 0 );
	if( setrlimit( RLIMIT_AS, &a ) != 0 ) {
		if( errno == EFAULT ) std::cerr << __LINE__ << ": EFAULT" << std::endl;
		else if( errno == EINVAL ) std::cerr << __LINE__ << ": EINVAL" << std::endl;
		else if( errno == EPERM ) std::cerr << __LINE__ << ": EPERM" << std::endl;
		else if( errno == ESRCH ) std::cerr << __LINE__ << ": ESRCH" << std::endl;
		else std::cerr << __LINE__ << ": SOME ERROR" << std::endl;
	}

	dout << __LINE__ << std::endl;

	treap<A> abba;

	dout << __LINE__ << std::endl;

	try {
		std::string d = "";
		while( true ) {
			d += "a";
			abba.emplace(d, d + "b", d + "c");
		}

		dout << __LINE__ << std::endl;
	} catch( std::bad_alloc const & ) {
		dout << __LINE__ << std::endl;

		try {
			while( true )
				new bool( true );

			dout << __LINE__ << std::endl;
		} catch( std::bad_alloc const & ) {
			dout << __LINE__ << std::endl;

			EXPECT_NO_THROW( abba.clear() );

			dout << __LINE__ << std::endl;

			EXPECT_EQ( 0, count_constructed - count_destroyed );

			dout << __LINE__ << std::endl;
		}

		dout << __LINE__ << std::endl;
	}

	dout << __LINE__ << std::endl;
}

TEST( MEMORY, ERASE_INSERT_CLEAR ) {
	dout << __LINE__ << std::endl;

	rlimit a;
	a.rlim_cur = rlim_t( 0 );
	a.rlim_max = rlim_t( 0 );
	if( setrlimit( RLIMIT_AS, &a ) != 0 ) {
		if( errno == EFAULT ) std::cerr << __LINE__ << ": EFAULT" << std::endl;
		else if( errno == EINVAL ) std::cerr << __LINE__ << ": EINVAL" << std::endl;
		else if( errno == EPERM ) std::cerr << __LINE__ << ": EPERM" << std::endl;
		else if( errno == ESRCH ) std::cerr << __LINE__ << ": ESRCH" << std::endl;
		else std::cerr << __LINE__ << ": SOME ERROR" << std::endl;
	}

	dout << __LINE__ << std::endl;

	treap<A> abba;

	dout << __LINE__ << std::endl;

	auto it = abba.emplace("a", "ab", "ac");

	dout << __LINE__ << std::endl;

	try {
		std::string d = "";
		while( true ) {
			d += "a";
			abba.emplace(d, d + "b", d + "c");
		}

		dout << __LINE__ << std::endl;
	} catch( std::bad_alloc const & ) {
		dout << __LINE__ << std::endl;

		try {
			while( true )
				new bool( true );

			dout << __LINE__ << std::endl;
		} catch( std::bad_alloc const & ) {
			dout << __LINE__ << std::endl;

			unsigned long long count_now = count_constructed - count_destroyed;

			dout << __LINE__ << std::endl;

			EXPECT_NO_THROW( abba.erase( it ) );

			dout << __LINE__ << std::endl;

			EXPECT_EQ( count_now - 1, count_constructed - count_destroyed );

			dout << __LINE__ << std::endl;

			EXPECT_NO_THROW( abba.emplace("a", "ab", "ac") );

			dout << __LINE__ << std::endl;

			EXPECT_EQ( count_now, count_constructed - count_destroyed );

			dout << __LINE__ << std::endl;

			EXPECT_NO_THROW( abba.clear() );

			dout << __LINE__ << std::endl;

			EXPECT_EQ( 0, count_constructed - count_destroyed );

			dout << __LINE__ << std::endl;
		}

		dout << __LINE__ << std::endl;
	}

	dout << __LINE__ << std::endl;
}

TEST( MEMORY, ERASE_INSERT_THROW_CLEAR ) {
	dout << __LINE__ << std::endl;

	rlimit a;
	a.rlim_cur = rlim_t( 0 );
	a.rlim_max = rlim_t( 0 );
	if( setrlimit( RLIMIT_AS, &a ) != 0 ) {
		if( errno == EFAULT ) std::cerr << __LINE__ << ": EFAULT" << std::endl;
		else if( errno == EINVAL ) std::cerr << __LINE__ << ": EINVAL" << std::endl;
		else if( errno == EPERM ) std::cerr << __LINE__ << ": EPERM" << std::endl;
		else if( errno == ESRCH ) std::cerr << __LINE__ << ": ESRCH" << std::endl;
		else std::cerr << __LINE__ << ": SOME ERROR" << std::endl;
	}

	dout << __LINE__ << std::endl;

	treap<A> abba;

	dout << __LINE__ << std::endl;

	auto it = abba.emplace("a", "ab", "ac");

	dout << __LINE__ << std::endl;

	try {
		std::string d = "";
		while( true ) {
			d += "a";
			abba.emplace(d, d + "b", d + "c");
		}

		dout << __LINE__ << std::endl;
	} catch( std::bad_alloc const & ) {
		dout << __LINE__ << std::endl;

		try {
			while( true )
				new bool( true );

			dout << __LINE__ << std::endl;
		} catch( std::bad_alloc const & ) {
			dout << __LINE__ << std::endl;

			unsigned long long count_now = count_constructed - count_destroyed;

			dout << __LINE__ << std::endl;

			EXPECT_NO_THROW( abba.erase( it ) );

			dout << __LINE__ << std::endl;

			EXPECT_EQ( count_now - 1, count_constructed - count_destroyed );

			dout << __LINE__ << std::endl;

			EXPECT_THROW( abba.emplace("dsgdasbtdfgbsvakhgljhljhsdfbhfbdsfa", "dsgdasbtafshdfhdfdfgbsvasdfbhfbdsfa", "dsgdasbtdfghdsjgfbsvasdfbhfkhgbdsfa"), std::bad_alloc );

			dout << __LINE__ << std::endl;

			EXPECT_EQ( count_now - 1, count_constructed - count_destroyed );

			dout << __LINE__ << std::endl;

			EXPECT_NO_THROW( abba.emplace("a", "ab", "ac") );

			dout << __LINE__ << std::endl;

			EXPECT_EQ( count_now, count_constructed - count_destroyed );

			dout << __LINE__ << std::endl;

			EXPECT_NO_THROW( abba.clear() );

			dout << __LINE__ << std::endl;

			EXPECT_EQ( 0, count_constructed - count_destroyed );

			dout << __LINE__ << std::endl;
		}

		dout << __LINE__ << std::endl;
	}

	dout << __LINE__ << std::endl;
}

TEST( TREAP, FIND_AND_ERASE ) {
	dout << __LINE__ << std::endl;

	treap<int> abba{1, 1, 2, 3, 4, 6, 7, 7, 7, 7, 7, 7, 8, 9, 0};

	dout << __LINE__ << std::endl;

	EXPECT_EQ( abba.find(5), abba.end() );

	dout << __LINE__ << std::endl;

	EXPECT_NE( abba.find(7), abba.cend() );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( abba.find(0), abba.begin() );

	dout << __LINE__ << std::endl;

	auto it1 = abba.find( 1 );

	dout << __LINE__ << std::endl;

	abba.erase( it1 );

	dout << __LINE__ << std::endl;

	auto it2 = abba.find( 1 );

	EXPECT_NE( it2, abba.end() );

	dout << __LINE__ << std::endl;

	EXPECT_NE( it1, it2 );

	dout << __LINE__ << std::endl;

	EXPECT_THROW( *( abba.find(5) ), std::runtime_error );

	dout << __LINE__ << std::endl;

	abba.erase( 7 );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( abba.find(7), abba.cend() );

	dout << __LINE__ << std::endl;
}

TEST( TREAP, EMPTY ) {
	dout << __LINE__ << std::endl;

    treap<int> abba;

    dout << __LINE__ << std::endl;

    EXPECT_TRUE( abba.empty() );

    dout << __LINE__ << std::endl;

    abba.insert( 1 );

    dout << __LINE__ << std::endl;

    EXPECT_FALSE( abba.empty() );

    dout << __LINE__ << std::endl;

    abba.erase( 2 );

    dout << __LINE__ << std::endl;

	EXPECT_FALSE( abba.empty() );

	dout << __LINE__ << std::endl;

    abba.erase( 1 );

    dout << __LINE__ << std::endl;

	EXPECT_TRUE( abba.empty() );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 0, abba.size() );

	dout << __LINE__ << std::endl;
}

TEST( TREAP, LOWEST_AND_HIGHEST ) {
	dout << __LINE__ << std::endl;

    treap<int> abba{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 9, 9 };

    dout << __LINE__ << std::endl;

	EXPECT_EQ( 0, abba.lowest() );

    dout << __LINE__ << std::endl;

	EXPECT_EQ( 9, abba.highest() );

    dout << __LINE__ << std::endl;

    abba.erase( abba.find( 0 ) );

    dout << __LINE__ << std::endl;

    abba.erase( abba.find( 9 ) );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 1, abba.lowest() );

    dout << __LINE__ << std::endl;

	EXPECT_EQ( 9, abba.highest() );

    dout << __LINE__ << std::endl;

    abba.erase( 9 );

    dout << __LINE__ << std::endl;

    EXPECT_EQ( 8, abba.highest() );

    dout << __LINE__ << std::endl;

    abba.insert( -1 );

    dout << __LINE__ << std::endl;

    EXPECT_EQ( -1, abba.lowest() );

    dout << __LINE__ << std::endl;

    abba.insert( 0 );

    dout << __LINE__ << std::endl;

    EXPECT_EQ( -1, abba.lowest() );

    dout << __LINE__ << std::endl;

    treap<int> abbb;

	dout << __LINE__ << std::endl;

    EXPECT_THROW( abbb.lowest(), std::runtime_error );

	dout << __LINE__ << std::endl;

    EXPECT_THROW( abbb.highest(), std::runtime_error );

    dout << __LINE__ << std::endl;
}

TEST( TREAP, RANGE_BASED_LOOP ) {
	dout << __LINE__ << std::endl;

    int t[10] = {3, 8, 2, 9, 1, 7, 2, 9, 4, 3};

	dout << __LINE__ << std::endl;

    treap<int> abba;

	dout << __LINE__ << std::endl;

    for( int i = 0 ; i < 10 ; ++i )
		abba.insert( t[i] );

	dout << __LINE__ << std::endl;

	int i = 0;
	std::sort( t, t+10 );

	dout << __LINE__ << std::endl;

	for( int a : abba ) {
		EXPECT_EQ( a, t[i] );
		++i;
	}

	dout << __LINE__ << std::endl;
}

TEST( TREAP, SIZE ) {
    dout << __LINE__ << std::endl;

    treap<int> abba{1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 0, 0};

	dout << __LINE__ << std::endl;

    EXPECT_EQ( 20, abba.size() );

	dout << __LINE__ << std::endl;

    abba.insert( 5 );

	dout << __LINE__ << std::endl;

    EXPECT_EQ( 21, abba.size() );

	dout << __LINE__ << std::endl;

    abba.erase( 5 );

	dout << __LINE__ << std::endl;

    EXPECT_EQ( 18, abba.size() );

	dout << __LINE__ << std::endl;

    for( int i = 1; i <= 10000 ; ++i )
		abba.emplace( i );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 10018, abba.size() );

	dout << __LINE__ << std::endl;
}

TEST( TREAP, SPLIT_AND_MERGE ) {
	dout << __LINE__ << std::endl;

	treap<int> abba{1, 2, 3, 4, 5, 5, 6, 7, 8, 9};

	dout << __LINE__ << std::endl;

    auto pair = abba.split( 5 );

    dout << __LINE__ << std::endl;

    EXPECT_EQ( 6, pair.first.size() );

    dout << __LINE__ << std::endl;

    EXPECT_EQ( 4, pair.second.size() );

    dout << __LINE__ << std::endl;

    EXPECT_TRUE( treap<int>::can_fast_merge( pair.first, pair.second ) );

    dout << __LINE__ << std::endl;

    auto abbb = treap<int>::merge( pair.first, pair.second );

    dout << __LINE__ << std::endl;

	EXPECT_EQ( 10, abbb.size() );

	dout << __LINE__ << std::endl;

    EXPECT_EQ( abba.size(), abbb.size() );

    dout << __LINE__ << std::endl;
}

TEST( TREAP, MOVE_MERGE ) {
	dout << __LINE__ << std::endl;

	int t[11] = {1, 2, 3, 6, 7, 7, 7, 8, 8, 8, 9};

	dout << __LINE__ << std::endl;

	treap<int> abba{1, 3, 6, 2, 7}, abbb{8, 9, 7, 8, 7, 8};

	dout << __LINE__ << std::endl;

	EXPECT_TRUE( treap<int>::can_fast_merge( abba, abbb ) );

    dout << __LINE__ << std::endl;

    auto abbc = treap<int>::merge( std::move( abba ), std::move( abbb ) );

	dout << __LINE__ << std::endl;

    EXPECT_EQ( 11, abbc.size() );

	dout << __LINE__ << std::endl;

    EXPECT_TRUE( abba.empty() );

	dout << __LINE__ << std::endl;

    EXPECT_TRUE( abbb.empty() );

    dout << __LINE__ << std::endl;

	int i = 0;

	dout << __LINE__ << std::endl;

    for( int a : abbc ) {
		EXPECT_EQ( a, t[i] );
		++i;
    }

    dout << __LINE__ << std::endl;
}

TEST( TREAP, INSERT_MERGE ) {
	dout << __LINE__ << std::endl;

	int t[13] = {1, 2, 3, 6, 7, 7, 7, 8, 8, 8, 8, 8, 9};

	dout << __LINE__ << std::endl;

	treap<int> abba{1, 3, 6, 2, 7, 8, 8}, abbb{8, 9, 7, 8, 7, 8};

	dout << __LINE__ << std::endl;

	EXPECT_FALSE( treap<int>::can_fast_merge( abba, abbb ) );

    dout << __LINE__ << std::endl;

    auto abbc = treap<int>::merge( abba, abbb );

	dout << __LINE__ << std::endl;

    EXPECT_EQ( 13, abbc.size() );

	dout << __LINE__ << std::endl;

    EXPECT_FALSE( abba.empty() );

	dout << __LINE__ << std::endl;

    EXPECT_FALSE( abbb.empty() );

    dout << __LINE__ << std::endl;

	int i = 0;

	dout << __LINE__ << std::endl;

    for( int a : abbc ) {
		EXPECT_EQ( a, t[i] );
		++i;
    }

    dout << __LINE__ << std::endl;
}

TEST( TREAP, ITERATOR ) {
	dout << __LINE__ << std::endl;

    treap<int> abba{1, 2, 3, 4, 5, 6};

	dout << __LINE__ << std::endl;

    auto it = ++abba.begin();

	dout << __LINE__ << std::endl;

	auto it1 = ++it;

	dout << __LINE__ << std::endl;

	auto it2 = it++;

	dout << __LINE__ << std::endl;

	EXPECT_EQ( it1, it2 );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 4, *it );

	dout << __LINE__ << std::endl;

	auto it3 = --abba.end();

	dout << __LINE__ << std::endl;

	auto it4 = --it3;

	dout << __LINE__ << std::endl;

	auto it5 = it3--;

	dout << __LINE__ << std::endl;

	EXPECT_EQ( it4, it5 );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 4, *it3 );

	dout << __LINE__ << std::endl;

    EXPECT_THROW( *abba.end(), std::runtime_error );

    dout << __LINE__ << std::endl;

    EXPECT_THROW( --abba.begin(), std::runtime_error );

    dout << __LINE__ << std::endl;

    EXPECT_THROW( ++abba.end(), std::runtime_error );

	dout << __LINE__ << std::endl;

    treap<int> abbb{ 1, 1, 1, 1, 1, 1 };

    dout << __LINE__ << std::endl;

    EXPECT_NE( abba.end(), abbb.end() );

    dout << __LINE__ << std::endl;

	EXPECT_NE( abbb.begin(), ++abbb.begin() );

	dout << __LINE__ << std::endl;

	abbb.erase( 1 );

	dout << __LINE__ << std::endl;

	EXPECT_THROW( --abbb.end(), std::runtime_error );

	dout << __LINE__ << std::endl;
}

TEST( TREAP, REVERSE_ITERATOR ) {
	dout << __LINE__ << std::endl;

    treap<int> abba{1, 2, 3, 4, 5, 6};

	dout << __LINE__ << std::endl;

    auto it = ++abba.rbegin();

	dout << __LINE__ << std::endl;

	auto it1 = ++it;

	dout << __LINE__ << std::endl;

	auto it2 = it++;

	dout << __LINE__ << std::endl;

	EXPECT_EQ( it1, it2 );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 3, *it );

	dout << __LINE__ << std::endl;

	auto it3 = --abba.rend();

	dout << __LINE__ << std::endl;

	auto it4 = --it3;

	dout << __LINE__ << std::endl;

	auto it5 = it3--;

	dout << __LINE__ << std::endl;

	EXPECT_EQ( it4, it5 );

	dout << __LINE__ << std::endl;

	EXPECT_EQ( 3, *it3 );

	dout << __LINE__ << std::endl;

    EXPECT_THROW( *abba.rend(), std::runtime_error );

    dout << __LINE__ << std::endl;

    EXPECT_THROW( --abba.rbegin(), std::runtime_error );

    dout << __LINE__ << std::endl;

    EXPECT_THROW( ++abba.rend(), std::runtime_error );

	dout << __LINE__ << std::endl;

    treap<int> abbb{ 1, 1, 1, 1, 1, 1 };

    dout << __LINE__ << std::endl;

    EXPECT_NE( abba.rend(), abbb.rend() );

    dout << __LINE__ << std::endl;

	EXPECT_NE( abbb.rbegin(), ++abbb.rbegin() );

	dout << __LINE__ << std::endl;

	abbb.erase( 1 );

	dout << __LINE__ << std::endl;

	EXPECT_THROW( --abbb.rend(), std::runtime_error );

	dout << __LINE__ << std::endl;
}

int main( int ac, char* av[] ) {
	std::cout << std::boolalpha << std::fixed;

	testing::InitGoogleTest( &ac, av );
	return RUN_ALL_TESTS();
}
