#ifndef DEBUG_FUNCTIONAL_H
#define DEBUG_FUNCTIONAL_H

#include <iostream>
#include <string>
#include <sstream>

template <typename T>
std::string toString(T val)
{
    std::ostringstream oss;
    oss << val;
    return oss.str();
}

#endif // DEBUG_FUNCTIONAL_H
