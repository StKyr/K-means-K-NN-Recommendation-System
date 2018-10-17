#include <iostream>
#include <NDVector.h>
#include <boost/program_options.hpp>
#include <NearestNeighborSearch.h>
#include <EuclideanSpaceLSH.h>
#include <metrics.h>
#include <VectorTSVReader.h>
#include <chrono>


void parse_command_line_arguments(int argc, char *argv[]);
std::unordered_map<std::string, NDVector> read_dataset(VectorTSVReader &reader);


namespace HyperParams {
    int M = 4;  //TODO: check value
    int w = 4;
}


namespace Params {
    std::string input_file;
    std::string query_file;
    int k = 3;
    int L = 1;
    std::string output_file;
}

int main(int argc, char *argv[]) {
    parse_command_line_arguments(argc, argv);

    //TODO: READERS SHOULD WORK WITH \t

    Dataset                                   X;
    Dataset                                   Y;
    supported_metrics                         metric = _NULL;
    double                                    R;
    int                                       d;
    NDVector                                  q;
    int                                       N;
    auto                                    & output = std::cout;

    std::set<std::string>                     possibleNeighborIds;
    std::vector<std::string>                  neighborIds;
    std::unordered_map<std::string, NDVector> possibleNeighbors;
    std::pair<std::string, double>            lshNN;
    std::pair<std::string, double>            trueNN;

    double                                    maxApproxRatio   = 0;
    double                                    currApproxRatio;
    double                                    currApproxTime;
    double                                    sumApproxTime    = 0;
    double                                    currDistanceTime;
    double                                    sumDistanceTime  = 0;
    std::chrono::steady_clock::time_point     begin;
    std::chrono::steady_clock::time_point     end;

    double (*distance)(NDVector, NDVector);


    DatasetReader dr(Params::input_file);
    X = dr.readDataset();
    metric = dr.metric;
    d = dr.vectorDim;

    QuerysetReader qr(Params::query_file);
    Y = qr.readDataset();
    R = qr.R;

    if (dr.vectorDim != qr.vectorDim) throw std::runtime_error("Input datasets have different dimensions.");

    N = (int)X.size();

    if (metric == Euclidean) distance = metrics::euclidean_distance;
    else                     distance = metrics::cosine_distance;

    HyperParams::w = 400;
    Params::k = 4;
    Params::L = 5;
    R = 100;


    EuclideanSpaceLSH lsh (Params::L, N/2, Params::k, d, HyperParams::w);
    lsh.insertDataset(X);



    //extern std::set<std::string> allKeys;




    int i=0;

    for (auto item : Y){
 //if (i++ == 10) break;
        begin = std::chrono::steady_clock::now();


        NDVector q = item.second;

        possibleNeighborIds = lsh.retrieveNeighbors(q);
        possibleNeighbors.clear();

        for (auto id : possibleNeighborIds){
            possibleNeighbors[id] =  X[id] ;
        }



        lshNN            = nearestNeighbor(q, possibleNeighbors, distance);

        end              = std::chrono::steady_clock::now();
        currApproxTime   = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();



        begin            = std::chrono::steady_clock::now();

        trueNN           = nearestNeighbor(q, X, metrics::euclidean_distance);

        end              = std::chrono::steady_clock::now();
        currDistanceTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();


        neighborIds      = range_nearestNeighbors(q, R, possibleNeighbors, metrics::euclidean_distance);



        if (trueNN.second != 0 && !isnan(maxApproxRatio)){
            currApproxRatio = lshNN.second / trueNN.second;
            if (currApproxRatio > maxApproxRatio) maxApproxRatio = currApproxRatio;
        }else if (lshNN.second == 0){
            maxApproxRatio = 1;
        }else{
            maxApproxRatio = nan("");
        }


        output << "Query: " << item.first << std::endl;
        output << "=== " << possibleNeighborIds.size() << " vectors with same key ===" << std::endl;
        output << "R-near neighbors:" << std::endl;
        for (auto &id: neighborIds) output << id << std::endl;
        output << "Nearest neighbor: " << lshNN.first << std::endl;
        output << "distanceLSH: " << lshNN.second <<std::endl;
        output << "distanceTrue: " << trueNN.second <<std::endl;
        output << "tLSH: " << currApproxTime << " (ms)" <<std::endl;
        output << "tTrue: " << currDistanceTime << " (ms)" <<std::endl;

        std::cout << "-------------------------\n" <<std::endl;

        sumApproxTime += currApproxTime;
        sumDistanceTime += currDistanceTime;
    }



    output << "----------------------------------------------" << std::endl;
    output << "Maximum approximation ratio: " << maxApproxRatio << std::endl;
    output << "Mean approximate-NN time: " << (sumApproxTime / N) << " (ms)" << std::endl;
    output << "Mean actual NN time: " << (sumDistanceTime / N) << " (ms)" << std::endl;

    return 0;
}

void parse_command_line_arguments(int argc, char *argv[]){

    using namespace boost::program_options;
    options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("-d", value<std::string>(),                   "set input file")
            ("-q", value<std::string>(),                   "set query file")
            ("-o", value<std::string>(),                   "set output file")
            ("-k", value<int>()->default_value(4),         "set number of LSH hashes")
            ("-L", value<int>()->default_value(5),         "set number of LSH hash tables")
        //("M", value<int>(),                           "set maximum number of points for inspection")
        //("probes", value<int>(),                      "set maximum number of Hypercube points for inspection")
            ;

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(0);
    }


    if (vm.count("-d")) Params::input_file = vm["-d"].as<std::string>();
    else {std::cout<< "Parameter -d is mandatory" << std::endl; exit(1);}

    if (vm.count("-q")) Params::query_file = vm["-q"].as<std::string>();
    else {std::cout<< "Parameter -q is mandatory" << std::endl; exit(1);}

    if (vm.count("-o")) Params::output_file = vm["-o"].as<std::string>();
    else {std::cout<< "Parameter -o is mandatory" << std::endl; exit(1);}

    Params::k = vm["-k"].as<int>(); if (Params::k <= 0) {std::cout<< "Invalid value of parameter -k" << std::endl; exit(1);}
    Params::L = vm["-L"].as<int>(); if (Params::L <= 0) {std::cout<< "Invalid value of parameter -L" << std::endl; exit(1);}
}

