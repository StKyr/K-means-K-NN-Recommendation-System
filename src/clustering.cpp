#include <string>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include <initializing.h>
#include <updating.h>
#include <assigning.h>
#include <kmeans.hpp>
#include <Silhouette.h>
#include <StoppingCriterion.h>
#include <DistancesTable.h>
#include <ApproximateNeighborSearch/similaritySearch.h>
#include <ApproximateNeighborSearch/EuclideanSpaceLSH.h>
#include <ApproximateNeighborSearch/HypercubeProjection.h>
#include "parameters.hpp"

#include <boost/program_options.hpp>
#include <chrono>

namespace Arguments {
    std::string input_file; // = "../datasets/twitter_dataset_small_v2.csv";
    std::string output_file;// = "../output.txt";
    std::string config_file;
    bool complete = false;
}
struct HyperParams hyperParams;

void parse_command_line_arguments(int argc, char *argv[]);


Criterion* create_criteria(Dataset& X){

    Criterion *c1 = new IteratorCounter(50);
    Criterion *c3 = new ToleranceCentroidsDisplacement(0.00000001);
    Criterion *c4 = new ObjectiveFunctionMinimization(0.0000001, X); //TODO: implement squared distance

    CriteriaOrchestrator *or1 = new OrCriteriaOrchestrator();
    or1->add_criterion(c1);
    or1->add_criterion(c3);
    or1->add_criterion(c4);

    return or1;
}



void get_configuration(int i, int j, int k, Dataset& X, supported_metrics metric, int dim, KMeansParams** pparams){

    if (*pparams == nullptr){
        *pparams = new KMeansParams();
        (*pparams)->stopping_criterion = create_criteria(X);
        (*pparams)->update = nullptr;
        (*pparams)->assign = nullptr;
        (*pparams)->initialize = nullptr;
    }

    if (i==0){
        (*pparams)->initialize = new RandomInitializer();
    }else if (i==1){
        delete (*pparams)->initialize;
        (*pparams)->initialize = new KMeansPlusPlus();
    }

    if (j==0){
        (*pparams)->assign = new LloydAssignment();
    }else if (j==1){
        delete (*pparams)->assign;
        LSH *searchIndex;
        if (metric == Euclidean) searchIndex = new EuclideanSpaceLSH((int)hyperParams.L_LSH, (int)(X.size() * hyperParams.tableSize_LSH), (int)hyperParams.k_LSH, dim, (int)hyperParams.w_LSH);
        else                     searchIndex = new CosineSimilarityLSH((int)hyperParams.L_LSH, (int)hyperParams.k_LSH, dim);
        searchIndex->insertDataset(X);
        (*pparams)->assign = new ReverseANNAssignment(*searchIndex);
    }else if (j==2){
        delete (*pparams)->assign;
        HypercubeProjection *searchIndex;
        if (metric == Euclidean) searchIndex = new EuclideanHypercubeProjection((int)hyperParams.k_CUBE,(int)hyperParams.M_CUBE,(int)hyperParams.probes_CUBE,(int)X.size(),(int)hyperParams.w_LSH,dim);
        else                     searchIndex = new CosineHypercubeProjection((int)hyperParams.k_CUBE,(int)hyperParams.M_CUBE,(int)hyperParams.probes_CUBE,(int)X.size(),dim);
        searchIndex->insertDataset(X);
        (*pparams)->assign = new ReverseANNAssignment(*searchIndex);
    }

    if (k==0){
        (*pparams)->update = new KMeansUpdate(dim);
    }else if (k==1){
        delete (*pparams)->update;
        (*pparams)->update = new PAMalaLloydUpdate();
    }
    if (i==-1 && j==-1 && k==-1){
        delete (*pparams)->initialize;
        delete (*pparams)->assign;
        delete (*pparams)->update;
        delete (*pparams)->stopping_criterion;
        delete *pparams;
    }
}


void print_results(int i, int j, int k, std::vector<Cluster> results, Dataset& X, SilhouetteResults silhouettes, supported_metrics metric, double time){
    std::ofstream fout;

    if (i==0 && j==0 && k==0) fout.open(Arguments::output_file, std::ios_base::out);
    else fout.open(Arguments::output_file, std::ios_base::app);

    if (fout.is_open()){

        fout << "Algorithm: ";
        if (i==0) fout << "Random Initialization x ";
        else if (i==1) fout << "K-means++ x ";

        if (j==0) fout << "Lloyd's Assignment x ";
        if (j==1) fout << "LSH Reverse Assignment x ";
        if (j==2) fout << "Hypercube Reverse Assignment x ";

        if (k==0) fout << "K-means Update" <<std::endl;
        if (k==1) fout << "PAM Update" <<std::endl;


        fout << "Metric: ";
        if (metric == Euclidean) fout <<  "Euclidean" <<std::endl;
        else if (metric == Cosine) fout << "cosine" <<std::endl;

        for (int c=0; c<results.size(); c++){
            fout << "CLUSTER-"<<c+1<<" {size: "<<results[c].num_points()<<", centroid: "<<results[c].get_centroid()<<"}"<<std::endl;
        }

        fout <<std::endl<<"clustering_time: " << time <<std::endl;
        fout<<"Silhouette: [";
        for (int s=0; s<silhouettes.perClusterSilhouette.size(); s++) fout<< silhouettes.perClusterSilhouette[s]<<", ";
        fout << silhouettes.averageSilhouette <<"]" <<std::endl;


        if (Arguments::complete){
            for (int c=0; c<results.size(); c++){
                fout << "CLUSTER-"<<c+1<<" {";
                for (auto &id: results[c].get_points()) fout<<id<<", ";
                fout<<"}"<<std::endl;
            }
        }
        fout.close();
    }else{

    }

}






int main(int argc, char *argv[]){

    parse_command_line_arguments(argc, argv);
    hyperParams = parse_config_params(Arguments::config_file);

    DatasetReader reader(Arguments::input_file);
    Dataset *X = reader.readDataset();
    double (*distance)(NDVector&, NDVector&) = (reader.metric == supported_metrics::Euclidean) ?
            metrics::euclidean_distance : metrics::cosine_distance;

    DistancesTable::getInstance().initialize(X->size(), distance);
    KMeansParams *k_means_params = nullptr;

    for (int i=1; i<2; i++){
        for (int j=1; j<2; j++){
            for (int k=0; k<1; k++){
                auto begin = std::chrono::steady_clock::now();
                get_configuration(i,j,k, *X, reader.metric, reader.vectorDim, &k_means_params);
                std::vector<Cluster> results = k_means_clustering(*X, hyperParams.K, *k_means_params, true);
                auto end = std::chrono::steady_clock::now();
                auto estimation = compute_silhouettes(*X, results);
                print_results(i,j,k, results, *X, estimation, reader.metric,std::chrono::duration_cast<std::chrono::seconds>(end - begin).count());
            }
        }
    }

    get_configuration(-1,-1,-1, *X, Euclidean, 0, &k_means_params);
    delete X;

    return 0;

/*

    Initializer *initializing_method = new KMeansPlusPlus();//new RandomInitializer();

    AbstractSimilaritySearch* searcher = new EuclideanSpaceLSH(3, X->size()/8, 2, reader.vectorDim, 2);
    //AbstractSimilaritySearch* searcher = new EuclideanHypercubeProjection(5, 500, 20, 10, (int)(1.5*reader.vectorDim), 4);
    searcher->insertDataset(*X);


    Assignment  *assignment_method   = new ReverseANNAssignment(*searcher);
    Update      *updating_method     = new KMeansUpdate(reader.vectorDim);
    Criterion   *criterion           = create_criteria(*X);
    KMeansParams k_means_params(*initializing_method, *assignment_method, *updating_method, *criterion);

    std::cout << "starting k-means" <<std::endl;
    std::vector<Cluster> results = k_means_clustering(*X, Arguments::K, k_means_params, false);
    std::cout << "k-means finished. (" << criterion->what() << ")" << std::endl;
    std::cout << k_means_params.stopping_criterion.state() << std::endl;

    auto estimation = compute_silhouettes(*X, results);
    for (int i=0; i<Arguments::K; i++) std::cout << "Silhouette of cluster "<<i+1<<": "<<estimation.perClusterSilhouette[i]<<std::endl;
    std::cout<<"Average silhouette: "<<estimation.averageSilhouette << std::endl;


    delete X;
    delete (RandomInitializer*)initializing_method;
    delete (LloydAssignment*)assignment_method;
    delete (KMeansUpdate*)updating_method;
    //return 0;
*/



}

void parse_command_line_arguments(int argc, char *argv[]){
    using namespace boost::program_options;
    options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("-i", value<std::string>(),                   "set input file")
            ("-c", value<std::string>(),                   "set configuration file")
            ("-o", value<std::string>(),                   "set output file")
            ("complete", "print vector ids assigned to clusters");
    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);
    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(0);
    }
    if (vm.count("-i")) Arguments::input_file = vm["-i"].as<std::string>();
    else {std::cout<< "Parameter -i is mandatory" << std::endl; exit(1);}
    if (vm.count("-c")) Arguments::config_file = vm["-c"].as<std::string>();
    else {std::cout<< "Parameter -c is mandatory" << std::endl; exit(1);}
    if (vm.count("-o")) Arguments::output_file = vm["-o"].as<std::string>();
    else {std::cout<< "Parameter -o is mandatory" << std::endl; exit(1);}

    if (vm.count("complete")) Arguments::complete = true;
    else Arguments::complete = false;
}

