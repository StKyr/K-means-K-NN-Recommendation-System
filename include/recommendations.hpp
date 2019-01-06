#ifndef ERGASIA3_RECOMMENDATIONS_HPP
#define ERGASIA3_RECOMMENDATIONS_HPP
#include <string>
#include <vector>
#include "Lexicon.hpp"
#include "ApproximateNeighborSearch/VectorCSVReader.h"
#include "UserVectors.hpp"

std::vector<std::vector<std::string>> userBasedSuggestions(int num, Dataset &U, Dataset &U_normed, int P, CryptoLexicon &K);

std::vector<std::vector<std::string>> userBasedSuggestions(int num, UserVectorDataset &dataset, int P, CryptoLexicon &K);




#endif //ERGASIA3_RECOMMENDATIONS_HPP
