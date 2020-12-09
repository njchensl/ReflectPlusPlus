#pragma once
#include <functional>

template<typename T>
struct StaticExec
{
    StaticExec(T fn)
    {
        fn();
    }
};
