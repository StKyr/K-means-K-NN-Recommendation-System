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
void similaritySearch(AbstractSimilaritySearch *searchStructure, Dataset& X, OrderedDataset& Y, double R,
                        supported_metrics metric, std::string name, std::ofstream &output);


namespace HyperParams {
    int M = 4;
    int w = 4;
}


namespace Params {
    std::string input_file;
    std::string query_file;
    int k = 3;
    int L = 1;
    std::string output_file;
    int M;
    int probes;
    double t;
}


int main(int argc, char *argv[]) {

#ifdef RUN_LSH
    parse_lsh_arguments(argc, argv);
#endif

#ifdef RUN_RPH
    parse_rph_arguments(argc, argv);
#endif


    Dataset *X;
    OrderedDataset *Y;
    supported_metrics metric;
    double R;

    auto & output = std::cout;

    DatasetReader dr(Params::input_file);
    X = dr.readDataset();
    metric = dr.metric;
    std::cout << "Done reading input dataset (" << X->size() << "x" << dr.vectorDim <<")" <<std::endl;


    QuerysetReader qr(Params::query_file);
    Y = qr.readOrderedDataset();
    R = qr.R;
    std::cout << "Done reading query dataset (" << Y->size() << "x" << qr.vectorDim <<")" <<std::endl;

    if (dr.vectorDim != qr.vectorDim) throw std::runtime_error("Input datasets have different dimensions.");


    HyperParams::w = (int)floor(Params::t * dr.vectorDim);

    std::ofstream fout;
    fout.open(Params::output_file);


#ifdef RUN_LSH
    AbstractSimilaritySearch *LSH;

    if (metric == Euclidean) {

        LSH = new EuclideanSpaceLSH(Params::L, (int) X->size() / 2, Params::k, dr.vectorDim, HyperParams::w);

        similaritySearch(LSH, *X, *Y, R, metric, "LSH", fout);

        delete (EuclideanSpaceLSH *) LSH;

    }else if (metric == Cosine) {
        LSH = new CosineSimilarityLSH(Params::L, Params::k, dr.vectorDim);

        similaritySearch(LSH, *X, *Y, R, metric, "LSH", fout);

        delete (CosineSimilarityLSH *) LSH;
    }
#endif


#ifdef RUN_RPH
    if (metric == Euclidean) {
        AbstractSimilaritySearch *RPH = new EuclideanHypercubeProjection(Params::k, Params::M, Params::probes, X->size(), HyperParams::w, dr.vectorDim);
        similaritySearch(RPH, *X, *Y, R, metric, "RPH", fout);
        delete (EuclideanHypercubeProjection *) RPH;
    }else if (metric == Cosine) {
        AbstractSimilaritySearch *RPH = new CosineHypercubeProjection(Params::k, Params::M, Params::probes, X->size(), dr.vectorDim);
        similaritySearch(RPH, *X, *Y, R, metric, "RPH", fout);
        delete (CosineHypercubeProjection *) RPH;
    }
#endif

    fout.close();
    delete X;
    delete Y;

    return 0;

}



void similaritySearch(AbstractSimilaritySearch *searchStructure, Dataset& X, OrderedDataset& Y, double R, supported_metrics metric, std::string name, std::ofstream &output){
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

    double                                    sumApproxRatio = 0;
    int                                       sumPointsSearched = 0;
    int n_found = 0;
    int n_searched = 0;
    std::string maxRatioId;

    double (*distance)(NDVector&, NDVector&);

    N = (int)X.size();

    if (metric == Euclidean) distance = metrics::euclidean_distance;
    else                     distance = metrics::cosine_distance;


    searchStructure->insertDataset(X);
    std::cout << "Done inserting vectors in " + name << std::endl;

    int i=0;

    for (auto &item : Y){
        i++;                                                                 // a useful counter for debugging

        begin = std::chrono::steady_clock::now();

        NDVector &q = item.second;

        possibleNeighborIds = searchStructure->retrieveNeighbors(q);

        possibleNeighbors.clear();
        for (auto &id : possibleNeighborIds) possibleNeighbors[id] =  X[id] ;

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
            if (currApproxRatio > maxApproxRatio){ maxApproxRatio = currApproxRatio; maxRatioId = item.first;}
            sumApproxRatio += currApproxRatio;
        }else if (lshNN.second == 0){
            maxApproxRatio = 1;
            sumApproxRatio += 1;
        }else{
            maxApproxRatio = nan("");
        }
        n_searched++;
        sumPointsSearched += possibleNeighborIds.size();

        if (lshNN.first == trueNN.first) n_found ++;

        output << "Query: " << item.first << std::endl;
        output << "R-near neighbors:" << std::endl;
        for (auto &id: neighborIds) output << id << std::endl;
        output << "Nearest neighbor: " << lshNN.first << std::endl;
        output << "distance"+name+": " << lshNN.second <<std::endl;
        output << "distanceTrue: " << trueNN.second <<std::endl;
        output << "t"+name+": " << currApproxTime << " (ms)" <<std::endl;
        output << "tTrue: " << currDistanceTime << " (ms)" <<std::endl;
        output<<std::endl;

        sumApproxTime += currApproxTime;
        sumDistanceTime += currDistanceTime;
    }

    std::cout << "Wrote results in output file.\n" << std::endl;

    std::cout << "-Maximum approximation ratio: " << maxApproxRatio << " (" << maxRatioId << ")" << std::endl;
    std::cout << "-Mean approximation ratio: " << sumApproxRatio / n_searched << std::endl;
    std::cout << "-Mean approximate Nearest Neighbor time: " << (sumApproxTime / i) << " (ms)" << std::endl;
    std::cout << "-Mean actual Nearest Neighbor time: " << (sumDistanceTime / i) << " (ms)" << std::endl;
    std::cout << "-Mean number of vectors searched exhaustively per query: " << (sumPointsSearched / (float)i) << std::endl;
    std::cout << "-Number of true Nearest Neighbors found: " << n_found << std::endl;
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
            ("-t", value<double>()->default_value(3.5),    "fine grained or coarse vs coarse grained discretization")
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
    Params::t = vm["-t"].as<double>(); if (Params::t <= 0) {std::cout<< "Invalid value of parameter -t" << std::endl; exit(1);}
}


void parse_rph_arguments(int argc, char *argv[]){
    using namespace boost::program_options;
    options_description desc("Allowed options");
    desc.add_options()
            ("help",                                           "produce help message")
            ("-d",     value<std::string>(),                   "set input file")
            ("-q",     value<std::string>(),                   "set query file")
            ("-o",     value<std::string>(),                   "set output file")
            ("-k",     value<int>()->default_value(3),         "set number of bits in hypercube representation")
            ("-M",     value<int>()->default_value(10),        "set maximum number of points for inspection")
            ("probes", value<int>()->default_value(10),        "set maximum number of hypercube vertices for inspection")
            ("-t",     value<double>()->default_value(3.5),    "fine grained or coarse vs coarse grained discretization")
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
    Params::M = vm["-M"].as<int>(); if (Params::M <= 0) {std::cout<< "Invalid value of parameter -M" << std::endl; exit(1);}
    Params::probes = vm["probes"].as<int>(); if (Params::probes <= 0) {std::cout<< "Invalid value of parameter --probes" << std::endl; exit(1);}
    Params::t = vm["-t"].as<double>(); if (Params::t <= 0) {std::cout<< "Invalid value of parameter -t" << std::endl; exit(1);}

}
