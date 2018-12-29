
#ifndef ERGASIA2_UTILS_HPP
#define ERGASIA2_UTILS_HPP

#include <vector>
#include <set>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string trim(std::string s) {
    return ltrim(rtrim(s));
}

template <typename T>
std::string str(T &obj){ std::stringstream ss;ss << obj;return ss.str(); }

template <typename T>
std::set<T> vec2set(std::vector<T> &vector){return std::set<T>(vector.begin(), vector.end());}

template <typename T>
void flog(T message) {std::cout <<message <<std::endl;}


template <typename T>
T str_convert(std::string& value) {std::stringstream ss(value); T t; ss>>t; return t;}



#endif //ERGASIA2_UTILS_HPP
