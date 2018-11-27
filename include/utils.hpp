
#ifndef ERGASIA2_UTILS_HPP
#define ERGASIA2_UTILS_HPP

#include <vector>
#include <set>
#include <sstream>
#include <iostream>

template <typename T>
std::string str(T &obj){ std::stringstream ss;ss << obj;return ss.str(); }


template <typename T>
std::set<T> vec2set(std::vector<T> &vector){return std::set<T>(vector.begin(), vector.end());}

template <typename T>
void flog(T message) { return;std::clog <<message <<std::endl;}

#endif //ERGASIA2_UTILS_HPP
