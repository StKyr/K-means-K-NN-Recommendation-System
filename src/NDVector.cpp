#include <iostream>

#include "NDVector.h"

NDVector::NDVector(std::vector<double>& coords){
    this->coords = coords;
}

NDVector::NDVector(std::initializer_list<double> list){
    this->coords = std::vector<double>();
    for (auto x : list) this->coords.push_back(x);
}

unsigned long NDVector::dim() const{
    if (this->coords.empty()) throw std::range_error("Unitialized Vector Exception");

    return this->coords.size();
}

double NDVector::norm(unsigned long p) const{

    double sum = 0;
    for (auto x: this->coords){
        sum += pow(x,p);
    }

    double rootPower = (1.0/p);
    double n = pow(sum, rootPower);
    return n;
}


double NDVector::dot(const NDVector& rhs)const{
    if (this->coords.empty()) throw std::range_error("Unitialized Vector Exception");
    if (rhs.dim() != this->dim() ) throw std::logic_error("Nonsensical dot product of vectors of different dimensions");

    double prev = 0;

    double p = 0;
    for (int i=0; i<this->dim(); i++){
        double factor = (*this)[i] * rhs[i];
        p += factor;

        if ((factor > 0 && p < prev) || (factor < 0 && p > prev)){
            throw std::range_error("double overflow while computing dot product");
        }
        prev = p;
    }
    return p;

}




NDVector&   NDVector::operator = (std::initializer_list<double> list){
    for (auto x : list) this->coords.push_back(x);
    return *this;
}



NDVector NDVector::operator +  (const NDVector& rhs){
    if (rhs.dim() != this->dim() ) throw std::logic_error("Nonsensical vector addition of different dimensions");

    std::vector<double> result = std::vector<double>();
    for (int i=0; i<this->dim(); i++){
        result.push_back(this->coords[i] + rhs.coords[i]);
    }
    return NDVector(result);
}
NDVector   NDVector::operator -  (const NDVector& rhs){
    if (rhs.dim() != this->dim() ) throw std::logic_error("Nonsensical vector subtraction of different dimensions");

    std::vector<double> result = std::vector<double>();
    for (int i=0; i<this->dim(); i++){
        result.push_back(this->coords[i] - rhs.coords[i]);
    }
    return NDVector(result);
}


NDVector   operator *  (double l, NDVector& rhs){
    if (!rhs.dim()) throw std::range_error("Unitialized Vector Exception");

    std::vector<double> result = std::vector<double>();
    for (int i=0; i<rhs.dim(); i++) {
        double x = rhs[i];
        result.push_back(l * x);
    }
    return NDVector(result);
}


NDVector&  NDVector::operator += (const NDVector& rhs){
    if (rhs.dim() != this->dim() ) throw std::logic_error("Nonsensical vector addition of different dimensions");

    std::vector<double> result = std::vector<double>();
    for (int i=0; i<this->dim(); i++){
        result.push_back(this->coords[i] + rhs.coords[i]);
    }
    this->coords = result;
    return *this;

}


NDVector&  NDVector::operator -= (const NDVector& rhs){
    if (rhs.dim() != this->dim() ) throw std::logic_error("Nonsensical vector subtraction of different dimensions");

    std::vector<double> result = std::vector<double>();
    for (int i=0; i<this->dim(); i++){
        result.push_back(this->coords[i] - rhs.coords[i]);
    }
    this->coords = result;
    return *this;
}


NDVector&  NDVector::operator *= (double l){
    if (this->coords.empty()) throw std::range_error("Unitialized Vector Exception");

    std::vector<double> result = std::vector<double>();
    for (auto x: this->coords){
        result.push_back(l * x);
    }
    this->coords = result;
    return *this;
}


const double   NDVector::operator [] (int i) const{
    if (i<0 || i>= this->dim()) {
        std::stringstream stringstream = std::stringstream();
        stringstream << "Invalid subscript index: " << i << " (dimension is: " << this->dim() <<")";
        throw std::range_error(stringstream.str());
    }

    return this->coords[i];
}



std::ostream& operator << (std::ostream& os, NDVector& rhs)
{
    os << "{";
    for (int i=0; i<rhs.dim()-1; i++){
        os << rhs[i] << ", ";
    }
    os << rhs[rhs.dim()-1] << "}";
    return os;
}


