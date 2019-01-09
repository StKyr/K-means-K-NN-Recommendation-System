#ifndef ERGASIA3_VALIDATIONS_HPP
#define ERGASIA3_VALIDATIONS_HPP

#include <string>
#include "ApproximateNeighborSearch/NDVector.h"
#include "UserVectors.hpp"
#include "ProximitySearch.hpp"

class Evaluation{
public:

    Evaluation() :totalUsersEvaluated(0), totalCryptosEvaluated(0), MAE(0), search(nullptr), dataset(nullptr),
                    ratings_function(nullptr) {}
    void evaluateUser(std::string userId,
                     NDVector& u,
                     std::vector<bool> userMentions,
                     double userAverage
                     );


    int numUserEvaluations(){return totalUsersEvaluated;}
    int numCoinsEvaluations(){return totalCryptosEvaluated;}
    double getMAE(){return MAE / totalCryptosEvaluated;}


    ProximitySearch *search;
    UserVectorDataset *dataset;
    std::vector<std::pair<double,int>> (*ratings_function)(const std::string &userId, NDVector &u,
                                                           std::vector<bool>& coinsMentioned,
                                                           std::vector<std::pair<std::string, double>>& neighbors,
                                                           UserVectorDataset &dataset);

private:
    int totalUsersEvaluated;
    int totalCryptosEvaluated;
    double MAE;

};



#endif //ERGASIA3_VALIDATIONS_HPP
