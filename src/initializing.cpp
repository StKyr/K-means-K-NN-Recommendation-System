#include "initializing.h"
#include <limits>
#include <random>
#include <DistancesTable.h>
#include <set>

std::vector<Cluster> RandomInitializer::operator () (Dataset& X, int k){

    std::uniform_int_distribution<> unif(0, (int)X.size()-1);
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<Cluster> clusters;
    clusters.reserve(k);

    std::set<NDVector> centroids;
    while (centroids.size() < k){
        auto it = X.begin();
        std::advance(it, unif(gen));
        centroids.insert((*it).second);
    }

    for (auto c: centroids){
        clusters.emplace_back(Cluster(c));
    }
    return clusters;
}





int binary_probability_search(std::vector<double> arr, int l, int r, double x){
    if (x==0) return 1;
    if (r == l+1){
        return r;
    }else{
        int mid = l + (r - l)/2;
        if (x  < arr[mid]) return binary_probability_search(arr, l,  mid, x);
        if (x >= arr[mid]) return binary_probability_search(arr, mid, r, x);
    }
    return -1;
}


/*
int linear_probability_search(std::vector<double> arr, double x){
    for (int i=0; i<arr.size()-1; i++) if (x >= arr[i] && x < arr[i+1]) return i;
    return (int)arr.size()-1;
}
*/


std::string select_random_centroid(std::vector<std::pair<std::string, double>> distances, double D){
    for (int i=0; i<distances.size(); i++) distances[i] = std::make_pair(distances[i].first, distances[i].second / D);  // normalize distances with max{D(i)}

    std::vector<double> P;                                                                                              // store all probabilities here for binary search
    P.reserve(distances.size()+1);

    for (int r=0; r<distances.size(); r++){                                                                             // construct each cell with the additive probability (distance) of this point

        double sum_squares = 0;
        for (int i=0; i<r; i++) sum_squares += distances[i].second * distances[i].second;
        P.push_back(sum_squares);
    }

    std::uniform_real_distribution<double> unif(0, *(P.end()));
    std::random_device rd;
    std::mt19937 gen(rd());


    double x = unif(gen);

    int index = binary_probability_search(P, 0, P.size(), x)-1; //TODO: bug here
    return distances[index].first;
}


std::vector<Cluster> KMeansPlusPlus::operator()(Dataset& X, int k) {



    std::vector<NDVector> initial_centers; initial_centers.reserve(k);                                                  // insert all centers here

    auto is_chosen_centroid = [&](NDVector i)-> bool {
        for (auto&c : initial_centers) if (c == i) return true; return false;};                                         // lambda for a more verbose code


    std::uniform_int_distribution<int> unif(0, (int)X.size()-1);                                                        // choose first centroid at random
    std::random_device rd;
    std::mt19937 gen(rd());
    auto it = X.begin();
    std::advance(it, unif(gen));
    initial_centers.emplace_back((*it).second);



    for (int t=1; t<k; t++){                                                                                            // repeat k-1 times

        std::vector<std::pair<std::string, double>> D; D.reserve(X.size()-t+1);                                         // store all min distances ( D(i) ) to some centroid here

        double maxD_i = std::numeric_limits<double>::min();                                                             // save max D(i) for normalization

        for (auto& item : X){                                                                                           // for every point
            if (is_chosen_centroid(item.second)) continue;                                                              // point should not be already chosen

            double D_i = std::numeric_limits<double>::max();


            for (int i=0; i<t; i++){                                                                                    // for each chosen centroid
                double d = DistancesTable::getInstance().distance(item.second, initial_centers[i]);                     // compute its distance
                D_i = std::min(D_i, d);                                                                                 // keep the minimum
            }

            D.emplace_back(std::make_pair(item.first, D_i));                                                            // store minimum distance

            maxD_i = std::max(maxD_i, D_i);                                                                             // update max overall distance
        }

        std::string new_centroid_id = select_random_centroid(D, maxD_i);                                                // choose random centroid with some probability
        initial_centers.push_back(X[new_centroid_id]);
    }

    std::vector<Cluster> clusters(k);
    std::transform(initial_centers.begin(), initial_centers.end(), clusters.begin(),                                    // create clusters from centroids
            [](NDVector c)->Cluster{ return Cluster(c); });

    return clusters;
}



