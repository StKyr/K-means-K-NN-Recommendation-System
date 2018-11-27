#include <string>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include <initializing.h>
#include <updating.h>
#include <assigning.h>
#include <kmeans.hpp>
#include <Silhouette.h>
#include <StoppingCriterion.h>
#include <DistancesIndex.h>

namespace Params {
    std::string input_file = "../datasets/twitter_dataset_small_v2.csv";
    std::string output_file = "../output.txt";
    int K = 5;
    int W;
    int L;
    int K_LSH;
    int K_RPH;
    int probes;
}


Criterion* create_criteria(Dataset& X){

    CriteriaOrchestrator *orchestrator = new OrCriteriaOrchestrator();

    Criterion *c1 = new IteratorCounter(50);
    Criterion *c2 = new CentroidsDisplacement();
    Criterion *c3 = new ToleranceCentroidsDisplacement(0.1);
    Criterion *c4 = new ObjectiveFunctionMinimization(0.0001, X); //TODO: implement squared distance
    Criterion *c5 = new AssignmentChanges();

    orchestrator->add_criterion(c1);
    //orchestrator->add_criterion(c2);
    orchestrator->add_criterion(c3);
    orchestrator->add_criterion(c4);
    orchestrator->add_criterion(c5);

    return orchestrator; //TODO: memory leak here
}


int main(int argc, char *argv[]){

    DatasetReader reader(Params::input_file);
    Dataset *X = reader.readDataset();
    double (*distance)(NDVector&, NDVector&) = (reader.metric == supported_metrics::Euclidean) ?
            metrics::euclidean_distance : metrics::cosine_distance;


    DistancesIndex::getInstance().initialize(distance);

    Initializer *initializing_method = new RandomInitializer();
    Assignment  *assignment_method   = new LloydAssignment();
    Update      *updating_method     = new KMeansUpdate(reader.vectorDim);
    Criterion   *criterion           = create_criteria(*X);
    KMeansParams k_means_params(*initializing_method, *assignment_method, *updating_method, *criterion);

    std::cout << "starting k-means" <<std::endl;
    std::vector<Cluster> results = k_means_clustering(*X, Params::K, k_means_params, true);
    std::cout << "k-means finished. (" << criterion->what() << ")" << std::endl;

    auto estimation = compute_silhouettes(*X, results);
    for (int i=0; i<Params::K; i++) std::cout << "Silhouette of cluster "<<i+1<<": "<<estimation.perClusterSilhouette[i]<<std::endl;
    std::cout<<"Average silhouette: "<<estimation.averageSilhouette << std::endl;


    delete X;
    delete (RandomInitializer*)initializing_method;
    delete (LloydAssignment*)assignment_method;
    delete (KMeansUpdate*)updating_method;
    return 0;
}

