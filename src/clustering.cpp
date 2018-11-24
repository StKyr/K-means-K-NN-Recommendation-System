#include <string>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include <initializing.h>
#include <updating.h>
#include <assigning.h>
#include <kmeans.hpp>
#include <Silhouette.h>

namespace Params {
    std::string input_file = "../datasets/twitter_dataset_small_v2.csv";
    std::string output_file = "../output.txt";
    int K = 50;
    int W;
    int L;
    int K_LSH;
    int K_RPH;
    int probes;
}


Criterion* create_criteria(Dataset& X, double (*dist_sq)(NDVector&, NDVector&)){

    CriteriaOrchestrator *orchestrator = new OrCriteriaOrchestrator();

    Criterion *c1 = new IteratorCounter(20);
    Criterion *c2 = new CentroidsDisplacement();
    Criterion *c3 = new ToleranceCentroidsDisplacement(0.1);
    Criterion *c4 = new ObjectiveFunctionMinimization(0.01, X, dist_sq); //TODO: implement squared distance

    orchestrator->add_criterion(c1);
    orchestrator->add_criterion(c2);
    orchestrator->add_criterion(c3);
    orchestrator->add_criterion(c4);

    return orchestrator; //TODO: memory leak here
}


int main(int argc, char *argv[]){

    DatasetReader reader(Params::input_file);
    Dataset *X = reader.readDataset();
    double (*distance)(NDVector&, NDVector&) = (reader.metric == supported_metrics::Euclidean) ?
            metrics::euclidean_distance : metrics::cosine_distance;


    Initializer *initializing_method = new RandomInitializer();
    Assignment  *assignment_method   = new LloydAssignment(distance);
    Update      *updating_method     = new KMeansUpdate(reader.vectorDim, distance);
    Criterion   *criterion           = create_criteria(*X, distance);
    KMeansParams k_means_params(*initializing_method, *assignment_method, *updating_method, *criterion);

    std::vector<Cluster> results = k_means_clustering(*X, Params::K, k_means_params);
    std::cout << "k-means finished. (" << criterion->what() << ")" << std::endl;

    auto estimation = compute_silhouettes(*X, results, distance);
    //for (int i=0; i<Params::K; i++) std::cout << "Silhouette of cluster "<<i+1<<": "<<estimation.perClusterSilhouette[i]<<std::endl;
    std::cout<<"Average silhouette: "<<estimation.averageSilhouette << std::endl;


    delete X;
    delete (RandomInitializer*)initializing_method;
    delete (LloydAssignment*)assignment_method;
    delete (KMeansUpdate*)updating_method;
    return 0;
}

