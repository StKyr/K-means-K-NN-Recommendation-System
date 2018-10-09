#ifndef ERGASIA1_NDVECTOR_H
#define ERGASIA1_NDVECTOR_H

#include <vector>
#include <ctgmath>
#include <algorithm>
#include <numeric>
#include <cstdarg>


class NDVector{

public:
    // member functions
                  NDVector();
    explicit      NDVector(std::vector<double>& coords);
                  NDVector(std::initializer_list<double> list);
    unsigned long dim     ();
    double        norm    (unsigned long p=2);



    // member variables
    std::vector<double> coords;
};




#endif //ERGASIA1_NDVECTOR_H
