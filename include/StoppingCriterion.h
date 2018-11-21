#ifndef ERGASIA2_STOPPINGCRITERION_H
#define ERGASIA2_STOPPINGCRITERION_H

#include <vector>
#include <kmeans.hpp>

/* Note: For facilitating multiple criteria in an AND or OR fashion - along with combinations
 * of those, in this file, the "Composite Design Pattern" is implemented.
 * For more information, see: https://en.wikipedia.org/wiki/Composite_pattern
 * */


/* Component */
class Criterion{
public:
    virtual bool should_stop(std::vector<Cluster>& clustering) = 0;
};


/* Leaf */
class IteratorCounter : public Criterion {
public:
    explicit IteratorCounter(unsigned long max_iterations) :max_iterations(max_iterations), curr_iterations(0) {};
    bool should_stop(std::vector<Cluster>& unused) override {return !(++curr_iterations < max_iterations);}
private:
    unsigned long curr_iterations;
    unsigned long max_iterations;
};

/* Leaf */
class CentroidsDisplacement : public Criterion {
public:
    bool should_stop(std::vector<Cluster>& clusters);

private:
    std::vector<Cluster> previous;
};

/* Abstract Composite */
class CriteriaOrchestrator : public Criterion {
public:
    void add_criterion(Criterion& c) {criteria.push_back(c)};

protected:
    std::vector<Criterion&> criteria;
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
};






#endif //ERGASIA2_STOPPINGCRITERION_H
