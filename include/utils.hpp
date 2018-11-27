
#ifndef ERGASIA2_UTILS_HPP
#define ERGASIA2_UTILS_HPP

#include <vector>
#include <set>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

template <typename T>
std::string str(T &obj){ std::stringstream ss;ss << obj;return ss.str(); }


template <typename T>
std::set<T> vec2set(std::vector<T> &vector){return std::set<T>(vector.begin(), vector.end());}

template <typename T>
void flog(T message) { return;std::clog <<message <<std::endl;}

#endif //ERGASIA2_UTILS_HPP
