#ifndef ERGASIA2_CLUSTER_H
#define ERGASIA2_CLUSTER_H

#include <vector>
#include "../ApproximateNeighborSearch/NDVector.h"
#include "../ApproximateNeighborSearch/VectorCSVReader.h"


/**
 * Implementation of the Cluster data type
 */
class Cluster{
public:
                            Cluster         ()                            : centroid(NDVector::zero_vector(1)){}
    explicit                Cluster         (NDVector& centroid)          : centroid(centroid) {}
    void                    assign          (const VectorId& vectorId)    {assigned_points.push_back(vectorId);}
    void                    clear_assignment()                            {assigned_points.clear();}
    NDVector&               get_centroid    ()                            {return centroid;}
    void                    update_centroid (const NDVector& new_centroid){centroid = new_centroid;};
    int                     num_points      () const                      {return assigned_points.size();}
    std::vector<VectorId>&  get_points      ()                            {return assigned_points;}
    bool                    has_point       (const VectorId& id) const    { return std::find(assigned_points.begin(),
                                                                            assigned_points.end(), id) != assigned_points.end();}

private:
    NDVector               centroid;
    std::vector<VectorId>  assigned_points;
};

#endif //ERGASIA2_CLUSTER_H
