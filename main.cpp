#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <vector>

void print( const std::vector<int>& vec, const std::map<size_t, int>& map )
{
    std::cout << "vec:";
    for ( const auto& n : vec )
    {
        std::cout << ' ' << n;
    }
    std::cout << std::endl;

    std::cout << "map:";
    for ( const auto& [_, n] : map )
    {
        std::cout << ' ' << n;
    }
    std::cout << std::endl;
}

template<class Cont>
void eraseNthItem( Cont& c, size_t n )
{
    auto it = std::begin( c );
    std::advance( it, n );
    c.erase( it );
}

/*
 * Simplified version of std::erase_if for std::map from C++20
 */
template<class Map>
void eraseIf( Map& map, std::function<bool( typename Map::value_type const& )> pred )
{
    for ( auto it = map.begin(), last = map.end(); it != last; )
    {
        if ( pred( *it ) )
        {
            it = map.erase( it );
        }
        else
        {
            ++it;
        }
    }
}

void eraseIf( std::vector<int>& vec, std::function<bool( const int& )> pred )
{
    vec.erase( std::remove_if( vec.begin(), vec.end(), std::move( pred ) ), vec.end() );
}

int main( int argc, char const *argv[] )
{
    std::random_device rd;
    std::mt19937 generator( rd() );

    std::cout << "1. Generate" << std::endl;
    std::uniform_int_distribution<> randomNumber( 1, 9 );
    std::vector<int> vec( 20 + randomNumber( generator ) );
    std::map<size_t, int> map;
    for ( size_t i = 0; i < vec.size(); ++i )
    {
        vec[i] = randomNumber( generator );
        map[i] = randomNumber( generator );
    }
    print( vec, map );

    std::cout << "2. Delete" << std::endl;
    const int itemsToDelete = std::uniform_int_distribution<>( 1, 15 )( generator );
    for ( int i = 0; i < itemsToDelete; ++i )
    {
        std::uniform_int_distribution<size_t> dis( 0, vec.size() - 1 );
        eraseNthItem( vec, dis( generator ) );
        eraseNthItem( map, dis( generator ) );
    }
    print( vec, map );

    std::cout << "3. Syncronize" << std::endl;
    std::array<bool, 9> vecSet;
    vecSet.fill( false );
    for ( const auto& n : vec )
    {
        vecSet[n - 1] = true;
    }

    std::array<bool, 9> mapSet;
    mapSet.fill( false );
    eraseIf( map, [&vecSet, &mapSet]( decltype( map )::value_type const& item )
    {
        const auto n = item.second - 1;
        mapSet[n] = vecSet[n];
        return !mapSet[n];
    } );

    if ( mapSet != vecSet )
    {
        eraseIf( vec, [&mapSet]( const int& n )
        {
            return !mapSet[n - 1];
        } );
    }
    print( vec, map );

    return 0;
}
