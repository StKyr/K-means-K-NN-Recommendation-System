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
                  NDVector()                                      = default;
                  NDVector(std::vector<double>& coords, long id);
                  NDVector(std::initializer_list<double> list);
    unsigned long dim     ()                                       const;
    double        norm    (unsigned long p=2)                      const;
    double        norm_sq ()                                       const;
    double        dot     (const NDVector& rhs)                    const;

    long          getId   () const {return id;}

    NDVector&     operator =  (const NDVector& rhs) = default;
    NDVector&     operator =  (std::initializer_list<double> list);
    inline bool   operator == (const NDVector& rhs)       const          {return this->coords == rhs.coords; }
    inline bool   operator != (const NDVector& rhs)                {return ! (*this == rhs);}

    inline bool   operator <  (const NDVector& rhs)       const    {return this->coords < rhs.coords;}//{throw std::logic_error("Nonsensical vector comparison");}
    inline bool   operator <= (const NDVector& rhs)       const    {return this->operator<(rhs) || this->coords == rhs.coords;}//{throw std::logic_error("Nonsensical vector comparison");}
    inline bool   operator >  (const NDVector& rhs)       const    {return !this->operator<=(rhs);}//{throw std::logic_error("Nonsensical vector comparison");}
    inline bool   operator >= (const NDVector& rhs)       const    {return !this->operator<(rhs);}//{throw std::logic_error("Nonsensical vector comparison");}

    NDVector      operator +  (const NDVector& rhs);
    NDVector      operator -  (const NDVector& rhs);
  //NDVector      operator *  (double l, NDVector& rhs);           // Non class member function - see below
    NDVector&     operator += (const NDVector& rhs);
    NDVector&     operator -= (const NDVector& rhs);
    NDVector&     operator *= (double l);

    const double  operator [] (int i)                              const;
  //std::ostream& operator << (std::ostream& os, NDVector& rhs);   // Non class member function - see below
    std::string   toString    ()                                   const;

    static NDVector random_vector(int dimension);
    static NDVector zero_vector(int dimension);

    static NDVector null_vector;
private:
    long id = -1;
    std::vector<double> coords;
};

NDVector      operator *  (double l, NDVector& rhs);
std::ostream& operator << (std::ostream& os, NDVector& rhs);


/*bool NDVector::operator==(const NDVector rhs){
    return lhs.coords == rhs.coords;
}*/

#endif //ERGASIA1_NDVECTOR_H
