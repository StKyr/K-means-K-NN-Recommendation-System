#ifndef ERGASIA3_ANALYSIS_HPP
#define ERGASIA3_ANALYSIS_HPP


#include <vector>
#include <unordered_map>
#include "ApproximateNeighborSearch/NDVector.h"
#include "Lexicon.hpp"
#include "Datasets.hpp"
#include "ApproximateNeighborSearch/VectorCSVReader.h"
#include "clustering/cluster.hpp"


Dataset *createUsersVectors(TokenizedDataset& T, SentimentLexicon& A, CryptoLexicon& K);
Dataset *createCentroidsVectors(std::vector<Cluster>& clusters, TokenizedDataset& T, SentimentLexicon& A, CryptoLexicon& K);
Dataset *normalizeDataset(Dataset &X);
Dataset * filterOutZeros(Dataset &X);



#endif //ERGASIA3_ANALYSIS_HPP
