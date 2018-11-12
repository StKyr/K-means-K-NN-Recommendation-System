#include "initializing.h"
#include <limits>
#include <random>

std::vector<NDVector> RandomInitializer::operator () (std::map<std::string, NDVector>& X, int k, int l){

    std::uniform_int_distribution<int> unif(0, (int)X.size());
    std::default_random_engine re;


    std::vector<NDVector> representatives;
    representatives.reserve(k);
    for (int i=0; i<k; i++){
        auto it = X.begin();
        std::advance(it, unif(re));
        representatives.emplace_back((*it).second);
    }
    return representatives;
}



/*
int binary_probability_search(std::vector<double> arr, int l, int r, int x){
        if (r >= l){
            int mid = l + (r - l)/2;
            if (x < arr[mid]) return binary_probability_search(arr, l, mid)
            if (arr[mid] > x) return binary_search(arr, l, mid-1, x);
            return binary_search(arr, mid+1, r, x);
        }
        return -1;
}
*/


int linear_probability_search(std::vector<double> arr, double x){
    for (int i=0; i<arr.size()-1; i++) if (x >= arr[i] && x < arr[i+1]) return i;
    return (int)arr.size()-1;
}


std::string select_random_centroid(std::vector<std::pair<std::string&, double>> distances, double D){
    for (int i=0; i<distances.size(); i++) distances[i] = std::make_pair(distances[i].first, distances[i].second / D);

    std::vector<double> P;
    P.reserve(distances.size()+1);

    for (int r=0; r<distances.size(); r++){

        double sum_squares = 0;
        for (int i=0; i<r; i++){
            sum_squares += distances[i].second * distances[i].second;
        }
        P.push_back(sum_squares);
    }

    std::uniform_real_distribution<double> unif(0, *(P.end()));
    std::default_random_engine re;

    //int index = binary_probability_search(P, 0, P.size(), x);
    int index = linear_probability_search(P, unif(re));
    return distances[index].first;
}


std::vector<NDVector> KMeansPlusPlus::operator()(std::map<std::string, NDVector>& X, int k, int l) {


    std::vector<NDVector> initial_centers;
    initial_centers.reserve(k);


    std::uniform_int_distribution<int> unif(0, (int)X.size());
    std::default_random_engine re;
    auto it = X.begin();
    std::advance(it, unif(re));
    initial_centers.emplace_back((*it).second);

    for (int t=1; t<k; t++){

        std::vector<std::pair<std::string&, double>> all_distances;
        all_distances.reserve(X.size()-t+1);
        double min_distance = std::numeric_limits<double>::max();

        for (auto& item : X){

            double Dr = std::numeric_limits<double>::max();
            for (int i=0; i<t; i++){
                double d = dist(item.second, initial_centers[i]);
                if (d < Dr) Dr = d;
            }
            all_distances.emplace_back(std::make_pair(item.first, Dr));

            if (Dr < min_distance) min_distance = Dr;
        }

        std::string new_centroid_id = select_random_centroid(all_distances, min_distance);
        initial_centers.push_back(X[new_centroid_id]);
    }

    return initial_centers;
}