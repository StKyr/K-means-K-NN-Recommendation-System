#ifndef ERGASIA2_STOPPINGCRITERION_H
#define ERGASIA2_STOPPINGCRITERION_H

#include <vector>
#include "cluster.hpp"
#include <sstream>

template <typename T>
std::string str(T &obj){
    std::stringstream ss;ss << obj;return ss.str();
}

/* Note: For facilitating multiple criteria in an AND or OR fashion - along with combinations
 * of those, in this file, the "Composite Design Pattern" is implemented.
 * For more information, see: https://en.wikipedia.org/wiki/Composite_pattern
 * */


/* Component */
class Criterion{
public:
    virtual bool should_stop(std::vector<Cluster>& clustering) = 0;
    virtual std::string what() = 0;
    virtual std::string state() = 0;
};


/* Leaf */
class IteratorCounter : public Criterion {
public:
    explicit IteratorCounter(unsigned long max_iterations) :max_iterations(max_iterations), curr_iterations(0) {};
    bool should_stop(std::vector<Cluster>& unused) override {return !(++curr_iterations < max_iterations);}
    std::string what() {return "Max number of iterations reached.";}
    std::string state() {return "Iteration " + str(curr_iterations);}
private:
    unsigned long curr_iterations;
    unsigned long max_iterations;
};

/* Leaf */
class CentroidsDisplacement : public Criterion {
public:
    bool should_stop(std::vector<Cluster>& clusters);
    std::string what() {return "Last iteration centroids did not changed position.";}
protected:
    std::vector<Cluster> previous;
};

/* Leaf */
class ToleranceCentroidsDisplacement : public CentroidsDisplacement {
public:
    explicit ToleranceCentroidsDisplacement(double tol_percentage) : tol(tol_percentage), previous_displacement(0){}
    bool should_stop(std::vector<Cluster>& clusters) override;
    std::string what() { return "Last iteration centroids changed very little.";}
protected:
    double previous_displacement;
    double tol;
};

/* Leaf */
class ObjectiveFunctionMinimization : public Criterion {
public:
    ObjectiveFunctionMinimization(double tol_percentage, Dataset& X, double (*dist_sq)(NDVector&, NDVector&))
            : X(X), dist_sq(dist_sq), tol(tol_percentage), previous_objective(0){}

    bool should_stop(std::vector<Cluster>& clusters) override;
    std::string what() { return "Objective function reduced by very little.";}
protected:
    double previous_objective;
    double tol;
    Dataset& X;
    double (*dist_sq)(NDVector&, NDVector&);
};





/* Abstract Composite */
class CriteriaOrchestrator : public Criterion {
public:
    void add_criterion(Criterion* c) {criteria.emplace_back(c);};
    std::string what() {return message.str();}

protected:
    std::vector<Criterion*> criteria;
    std::stringstream message;
};


/* Concrete Composite */
class OrCriteriaOrchestrator : public CriteriaOrchestrator{
public:
    bool should_stop(std::vector<Cluster>& clustering)  override;

};


/* Concrete Composite */
class AndCriteriaOrchestrator : public CriteriaOrchestrator{
public:
    bool should_stop(std::vector<Cluster>& clustering)  override;
    std::string what() override;
};






#endif //ERGASIA2_STOPPINGCRITERION_H
