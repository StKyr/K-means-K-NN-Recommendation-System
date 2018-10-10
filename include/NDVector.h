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
                  NDVector()                                       = default;
    explicit      NDVector(std::vector<double>& coords);
                  NDVector(std::initializer_list<double> list);
    unsigned long dim     ()                                       const;
    double        norm    (unsigned long p=2)                      const;
    double        dot     (const NDVector& rhs)                    const;


    NDVector&     operator =  (const NDVector& rhs)                = default;
    NDVector&     operator =  (std::initializer_list<double> list);
    inline bool   operator == (const NDVector& rhs)                {return this->coords == rhs.coords; }
    inline bool   operator != (const NDVector& rhs)                {return ! (*this == rhs);}

    inline bool   operator <  (const NDVector& rhs)                {throw std::logic_error("Nonsensical vector comparison");}
    inline bool   operator <= (const NDVector& rhs)                {throw std::logic_error("Nonsensical vector comparison");}
    inline bool   operator >  (const NDVector& rhs)                {throw std::logic_error("Nonsensical vector comparison");}
    inline bool   operator >= (const NDVector& rhs)                {throw std::logic_error("Nonsensical vector comparison");}

    NDVector      operator +  (const NDVector& rhs);
    NDVector      operator -  (const NDVector& rhs);
  //NDVector      operator *  (double l, NDVector& rhs);           // Non class member function - see below
    NDVector&     operator += (const NDVector& rhs);
    NDVector&     operator -= (const NDVector& rhs);
    NDVector&     operator *= (double l);

    const double  operator [] (int i)                              const;
  //std::ostream& operator << (std::ostream& os, NDVector& rhs);   // Non class member function - see below



    // member variables
    std::vector<double> coords;
};

NDVector      operator *  (double l, NDVector& rhs);
std::ostream& operator << (std::ostream& os, NDVector& rhs);


#endif //ERGASIA1_NDVECTOR_H
