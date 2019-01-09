#ifndef ERGASIA3_RECOMMENDATIONS_HPP
#define ERGASIA3_RECOMMENDATIONS_HPP
#include <string>
#include <vector>
#include "Lexicon.hpp"
#include "ApproximateNeighborSearch/VectorCSVReader.h"
#include "UserVectors.hpp"
#include "Evaluation.hpp"


enum SuggestionType {nearest_neighbor, clustering};

std::vector<std::vector<std::string>> recommend(int num, UserVectorDataset &U, int P, CryptoLexicon &K, enum SuggestionType suggestionType, Evaluation *evaluation);
std::vector<std::vector<std::string>> recommend(int num, UserVectorDataset &U, VirtualUserVectorDataset &C, int P, CryptoLexicon &K, enum SuggestionType suggestionType, Evaluation *evaluation);

std::vector<std::pair<double,int>> rate(const std::string &userId, NDVector &u, std::vector<bool>& coinsMentioned,
                                        std::vector<std::pair<std::string, double>>& neighbors, UserVectorDataset &dataset);

#endif //ERGASIA3_RECOMMENDATIONS_HPP
