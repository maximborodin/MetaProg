#include <iostream>
#include "typelist.h"
#include "fibonacciNumber.h"


template <typename Root, typename Branch> struct Vertex;

template <typename Root, typename Branch>
struct Vertex : public Branch {};


class EmptyClass {};

// линейная иерархия
template <typename Arr, template<typename Root, typename Branch> typename Vertex>
struct Bamboo;

template <typename Head, typename ...Tail, template<typename Root, typename Branch> typename Vertex>
struct Bamboo<TypeList<Head, Tail...>, Vertex> : public Vertex<Head, Bamboo<TypeList<Tail...>, Vertex> > {};

template <template <typename Root, typename Branch> typename Vertex>
struct Bamboo<EmptyTypeList, Vertex> : public Vertex<EmptyClass, EmptyClass> {};


//Основная иерархия
template <typename Arr, typename StoredArr, int remainingSize, int branchNumber,
template <typename Root, typename Branch> typename Vertex>
struct Hierarchy;


template <typename Head, typename ...Tail, typename ...StoredArr, typename ...Tail, int remainingSize,
        int branchNumber, template <typename Root, typename Branch> typename Vertex>
struct Hierarchy<TypeList<Head, Tail...>, TypeList<StoredArr...>, remainingSize, branchNumber, Vertex> :
        public Hierarchy<TypeList<Tail...>, TypeList<Head, StoredArr...>, remainingSize - 1, branchNumber, Vertex>
{};

template <typename Head, typename ... Tail, typename ...StoredArr, int branchNumber,
template <typename Root, typename Branch> class Vertex>
        struct Hierarchy<TypeList<Head, Tail...>, typename <StoredArr...>, 0, branchNumber, Vertex> :
                public Bamboo<TypeList<StoredArr...>, Vertex>,
public Hierarchy<TypeList<Head, Tail...> EmptyTypeList, branchNumber + 1, Fibonacci<branchNumber + 1>::number + 1, Vertex>
{};


int main()
{
    // std::cout << Fibonacci<5>::number;
}