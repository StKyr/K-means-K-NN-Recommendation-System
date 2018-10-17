#include <iostream>
#include <NDVector.h>
#include <boost/program_options.hpp>
#include <NearestNeighborSearch.h>
#include <EuclideanSpaceLSH.h>
#include <metrics.h>
#include <VectorTSVReader.h>
#include <chrono>
#include <HypercubeProjection.h>


void parse_lsh_arguments(int argc, char *argv[]);
void parse_rph_arguments(int argc, char *argv[]);
void similaritySearch(AbstractSimilaritySearch *searchStructure, Dataset& X, Dataset& Y, double R,
                        supported_metrics metric, std::string name);


namespace HyperParams {
    int M = 4;  //TODO: check value
    int w = 800;
}


namespace Params {
    std::string input_file;
    std::string query_file;
    int k = 3;
    int L = 1;
    std::string output_file;
    int M;
    int probes;
}


#define RUN_LSH
//#define RUN_RPH

int main(int argc, char *argv[]) {

#ifdef RUN_LSH
    parse_lsh_arguments(argc, argv);
#endif

#ifdef RUN_RPH
    parse_rph_arguments(argc, argv);
#endif


    Dataset X;
    Dataset Y;
    supported_metrics metric;
    double R;

    auto & output = std::cout;

    DatasetReader dr(Params::input_file);
    X = dr.readDataset();
    metric = dr.metric;

    QuerysetReader qr(Params::query_file);
    Y = qr.readDataset();
    R = qr.R;

    if (dr.vectorDim != qr.vectorDim) throw std::runtime_error("Input datasets have different dimensions.");



#ifdef RUN_LSH
    AbstractSimilaritySearch *LSH = new EuclideanSpaceLSH(Params::L, (int)X.size()/2, Params::k, dr.vectorDim, HyperParams::w);
    similaritySearch(LSH, X, Y, R, metric, "LSH");
    delete (EuclideanSpaceLSH *)LSH;
#endif


#ifdef RUN_RPH
    if (metric == Euclidean) {
        AbstractSimilaritySearch *RPH = new EuclideanHypercubeProjection(Params::k, Params::M, Params::probes, X.size(), HyperParams::w, dr.vectorDim);
        similaritySearch(RPH, X, Y, R, metric, "RPH");
        delete (EuclideanHypercubeProjection *) RPH;
    }else if (metric == Cosine) {
        AbstractSimilaritySearch *RPH = new CosineHypercubeProjection(Params::k, Params::M, Params::probes, X.size(), dr.vectorDim);
        similaritySearch(RPH, X, Y, R, metric, "RPH");
        delete (CosineHypercubeProjection *) RPH;
    }
#endif

    return 0;

}



void similaritySearch(AbstractSimilaritySearch *searchStructure, Dataset& X, Dataset& Y, double R, supported_metrics metric, std::string name){
    //TODO: READERS SHOULD WORK WITH \t
    auto &output = std::cout;

    NDVector                                  q;
    int                                       N;
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

    N = (int)X.size();

    if (metric == Euclidean) distance = metrics::euclidean_distance;
    else                     distance = metrics::cosine_distance;


    searchStructure->insertDataset(X);

    int i=0;

    for (auto item : Y){
        if (i++ == 5) break;

        begin = std::chrono::steady_clock::now();

        NDVector q = item.second;

        possibleNeighborIds = searchStructure->retrieveNeighbors(q);

        possibleNeighbors.clear();
        for (auto id : possibleNeighborIds) possibleNeighbors[id] =  X[id] ;

        lshNN            = nearestNeighbor(q, possibleNeighbors, distance);

        end              = std::chrono::steady_clock::now();
        currApproxTime   = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

        begin            = std::chrono::steady_clock::now();

        trueNN           = nearestNeighbor(q, X, distance);

        end              = std::chrono::steady_clock::now();
        currDistanceTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

        neighborIds      = range_nearestNeighbors(q, R, possibleNeighbors, distance);


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
        output << "distance"+name+": " << lshNN.second <<std::endl;
        output << "distanceTrue: " << trueNN.second <<std::endl;
        output << "t"+name+": " << currApproxTime << " (ms)" <<std::endl;
        output << "tTrue: " << currDistanceTime << " (ms)" <<std::endl;

        std::cout << "-------------------------\n" <<std::endl;

        sumApproxTime += currApproxTime;
        sumDistanceTime += currDistanceTime;
    }

    output << "----------------------------------------------" << std::endl;
    output << "Maximum approximation ratio: " << maxApproxRatio << std::endl;
    output << "Mean approximate-NN time: " << (sumApproxTime / N) << " (ms)" << std::endl;
    output << "Mean actual NN time: " << (sumDistanceTime / N) << " (ms)" << std::endl;
}





void parse_lsh_arguments(int argc, char *argv[]){
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


void parse_rph_arguments(int argc, char *argv[]){
    using namespace boost::program_options;
    options_description desc("Allowed options");
    desc.add_options()
            ("help",   "produce help message")
            ("-d",     value<std::string>(),                   "set input file")
            ("-q",     value<std::string>(),                   "set query file")
            ("-o",     value<std::string>(),                   "set output file")
            ("-k",     value<int>()->default_value(3),         "set number of bits in hypercube representation")
            ("-M",      value<int>()->default_value(10),       "set maximum number of points for inspection")
            ("-probes", value<int>()->default_value(10),       "set maximum number of hypercube vertices for inspection")
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
    Params::L = vm["-M"].as<int>(); if (Params::L <= 0) {std::cout<< "Invalid value of parameter -M" << std::endl; exit(1);}
    Params::probes = vm["-probes"].as<int>(); if (Params::probes <= 0) {std::cout<< "Invalid value of parameter -probes" << std::endl; exit(1);}
}
