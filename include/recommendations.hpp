#ifndef ERGASIA3_RECOMMENDATIONS_HPP
#define ERGASIA3_RECOMMENDATIONS_HPP
#include <string>
#include <vector>
#include "Lexicon.hpp"
#include "ApproximateNeighborSearch/VectorCSVReader.h"

std::vector<std::vector<std::string>> userBasedSuggestions(int num, Dataset &U, Dataset &U_normed, int P, CryptoLexicon &K);


#endif //ERGASIA3_RECOMMENDATIONS_HPP
