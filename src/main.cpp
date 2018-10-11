#include <iostream>
#include <NDVector.h>
#include <DatasetReader.h>
#include <QuerysetReader.h>
#include <boost/program_options.hpp>
#include <NearestNeighborSearch.h>

void parse_command_line_arguments(int argc, char *argv[]);
namespace Params {
    std::string input_file;
    std::string query_file;
    int k;
    int L;
    std::string output_file;
}

int main(int argc, char *argv[]) {

    parse_command_line_arguments(argc, argv);

    std::cout << "Hello, World!" << std::endl;

    QuerysetReader reader ("../input/queryset1.tsv");
    //DatasetReader reader ("../input/input1.tsv");

    reader.initialize();
    reader.parseFirstLine();

    std::cout << reader.R <<std::endl;
    //std::cout << reader.metric << std::endl;


    std::string s;
    std::vector<double> xs;
    int i;
    xs = reader.parseNextLine(&s);

    std::unordered_map<std::string, NDVector> X;

    while (! xs.empty()) {
        NDVector v(xs);
        std::cout << v << std::endl;
        xs = reader.parseNextLine(&s);
        X[s] = v;
    }

    std::pair<std::string, double> result;
    result = nearestNeighbor(NDVector({0,0,0,0}), X, metrics::euclidean_distance);

    std::cout << result.first << std::endl;
    std::cout << result.second << std::endl;

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

