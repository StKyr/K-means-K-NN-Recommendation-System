#include <string>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include <initializing.h>
#include <updating.h>
#include <assigning.h>
#include <kmeans.hpp>

namespace Params {
    std::string input_file;
    std::string output_file;
    int K;
    int W;
    int L;
    int K_LSH;
    int K_RPH;
    int probes;
}



int main(int argc, char *argv[]){

    DatasetReader reader(Params::input_file);
    Dataset *X = reader.readDataset();
    double (*distance)(NDVector&, NDVector&) = (reader.metric == supported_metrics::Euclidean) ?
            metrics::euclidean_distance : metrics::cosine_distance;


    Initializer *initializing_method = new RandomInitializer();
    Assignment  *assignment_method   = new LloydAssignment(distance);
    Update      *updating_method     = new KMeansUpdate(reader.vectorDim, distance);
    KMeansParams k_means_params(*initializing_method, *assignment_method, *updating_method);

    clustering_results results = k_means_clustering(*X, Params::K, k_means_params, distance);

    delete X;
    delete (RandomInitializer*)initializing_method;
    delete (LloydAssignment*)assignment_method;
    delete (KMeansUpdate*)updating_method;
    return 0;
}