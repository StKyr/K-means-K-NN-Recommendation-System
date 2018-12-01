#ifndef ERGASIA2_STOPPINGCRITERION_H
#define ERGASIA2_STOPPINGCRITERION_H

#include <vector>
#include "cluster.hpp"
#include "utils.hpp"
#include <sstream>


/* Note: For facilitating multiple criteria in an AND or OR fashion - along with combinations
 * of those, in this file, the "Composite Design Pattern" is implemented.
 * For more information, see: https://en.wikipedia.org/wiki/Composite_pattern
 * */


/* Component */
class Criterion{
public:
    explicit Criterion(bool passive): passive(passive){}
    virtual bool should_stop(std::vector<Cluster>& clustering) = 0;
    virtual std::string what() = 0;
    virtual std::string state() = 0;
    virtual ~Criterion() = default;

protected:
    bool passive;
};


/* Leaf */
class IteratorCounter : public Criterion {
public:
    IteratorCounter(unsigned long max_iterations, bool passive=false) :Criterion(passive), max_iterations(max_iterations), curr_iterations(0) {};
    bool should_stop(std::vector<Cluster>& unused) override {return (passive)?false:!(++curr_iterations < max_iterations);}
    std::string what() {return "max iterations reached";}
    std::string state() {return "Iteration " + str(curr_iterations);}
private:
    unsigned long curr_iterations;
    unsigned long max_iterations;
};

/* Leaf */
class CentroidsDisplacement : public Criterion {
public:
    explicit CentroidsDisplacement(bool passive=false) :Criterion(passive){};
    bool should_stop(std::vector<Cluster>& clusters);
    std::string what() {return "centroids did not change";}
    std::string state() override;
protected:
    std::vector<Cluster> previous;
};

/* Leaf */
class ToleranceCentroidsDisplacement : public CentroidsDisplacement {
public:
    explicit ToleranceCentroidsDisplacement(double tol_percentage, bool passive=false) :CentroidsDisplacement(passive), tol(tol_percentage), previous_displacement(0){}
    bool should_stop(std::vector<Cluster>& clusters) override;
    std::string what() { return "centroids changed very little";}
    std::string state() override {return "Displacement: "+str(previous_displacement);}
protected:
    double previous_displacement;
    double tol;
};

/* Leaf */
class ObjectiveFunctionMinimization : public Criterion {
public:
    ObjectiveFunctionMinimization(double tol_percentage, Dataset& X, bool passive=false)
            : Criterion(passive), X(X), tol(tol_percentage), previous_objective(0){}

    bool should_stop(std::vector<Cluster>& clusters) override;
    std::string what() { return "objective function reduced by very little";}
    std::string state() override {return "Objective Function: "+str(previous_objective);}

protected:
    double previous_objective;
    double tol;
    Dataset& X;
};


/* Leaf */
class AssignmentChanges : public Criterion {
public:
    explicit AssignmentChanges(bool passive=false) :Criterion(passive), clusters_changed(-1){};
    bool should_stop(std::vector<Cluster>& clusters) override;
    std::string what() { return "no differences in assignment";}
    std::string state() override {return "Number of clusters changed: "+str(clusters_changed);}

protected:
    int clusters_changed;
    std::vector<Cluster> previous_clusters;
};




/* Abstract Composite */
class CriteriaOrchestrator : public Criterion {
public:
    CriteriaOrchestrator():Criterion(false){};
    void add_criterion(Criterion* c) {criteria.emplace_back(c);};
    std::string what() {return message.str();}
    std::string state() override;
    virtual ~CriteriaOrchestrator() override {for (auto c: criteria) delete c;}
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
