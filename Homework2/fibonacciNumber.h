#pragma once

template<unsigned int i>
struct Fibonacci
{
    static const int number = Fibonacci<i - 1>::number + Fibonacci<i - 2>::number;
};

template<>
struct Fibonacci<1>
{
    static const int number = 1;
};

template<>
struct Fibonacci<0>
{
    static const int number = 1;
};