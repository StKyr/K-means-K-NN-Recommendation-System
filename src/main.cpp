#include <iostream>
#include <NDVector.h>
#include <DatasetReader.h>
#include <QuerysetReader.h>
#include <boost/program_options.hpp>
#include <NearestNeighborSearch.h>
#include <EuclideanSpaceLSH.h>


void parse_command_line_arguments(int argc, char *argv[]);
std::unordered_map<std::string, NDVector> read_dataset(VectorTSVReader &reader);


namespace HyperParams {
    int M = 4;  //TODO: check value
    int w = 4;
}


namespace Params {
    std::string input_file;
    std::string query_file;
    int k;
    int L;
    std::string output_file;
}

int main(int argc, char *argv[]) {
    parse_command_line_arguments(argc, argv);

    std::unordered_map<std::string, NDVector> X;
    std::unordered_map<std::string, NDVector> Y;

    DatasetReader dr ("../input/input1.tsv");
    dr.initialize();
    dr.parseFirstLine();
    X = read_dataset(dr);
    dr.finalize();

    QuerysetReader qr ("../input/queryset1.tsv");
    qr.initialize();
    qr.parseFirstLine();
    Y = read_dataset(qr);
    qr.finalize();

    int N = (int)X.size();
    int d = 4; //TODO: have parsers save the dimension -- MUST

    EuclideanSpaceLSH lsh (Params::L, N/2, HyperParams::M, Params::k, d, HyperParams::w);

    lsh.insertDataset(X);
    std::set<std::string> possibleNeighborIds;
    std::vector<std::string> neighborIds;
    std::unordered_map<std::string, NDVector> possibleNeighbors;
    std::vector<NDVector> neighbors;

    NDVector q, neighbor;

    for (auto item : Y){
        NDVector q = item.second;

        std::cout << q << std::endl;

        possibleNeighborIds = lsh.retrieveNeighbors(q);
        possibleNeighbors.clear();
        for (auto id : possibleNeighborIds){
            possibleNeighbors[id] =  X[id] ;
        }

        neighborIds = k_nearestNeighbors(q, 2, possibleNeighbors, metrics::euclidean_distance); //TODO: correct call
        neighbors.clear();
        for (auto id: neighborIds){
            std::cout << possibleNeighbors[id] << std::endl;
        }
    }




    return 0;

}



std::unordered_map<std::string, NDVector> read_dataset(VectorTSVReader &reader){

    std::string s;
    std::vector<double> xs;
    std::unordered_map<std::string, NDVector> X;

    xs = reader.parseNextLine(&s);
    while (! xs.empty()) {
        NDVector v(xs);
        xs = reader.parseNextLine(&s);
        X[s] = v;
    }

    return X;
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

