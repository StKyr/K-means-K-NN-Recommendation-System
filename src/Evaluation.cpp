#include "../include/Evaluation.hpp"

int countTrue(std::vector<bool> vector){ int cnt=0; for (auto value: vector) if (value) cnt++; return cnt;}

void Evaluation::evaluateUser(std::string userId,
                              NDVector& u,
                              std::vector<bool> userMentions,
                              double userAverage){

    int numCointsMentioned = countTrue(userMentions);
    if (!numCointsMentioned || numCointsMentioned == 1) return;

    for (int i=0; i<userMentions.size(); i++){
        if (! userMentions[i]) continue;

        double originalSentiment = u[i];

        std::vector<bool> currMentions(userMentions.begin(), userMentions.end());
        currMentions[i] = false;

        NDVector currU = u;
        u[i] = userAverage;

        auto nearest_neighbors = (*search)(userId, currU);
        auto currRatings = ratings_function(userId, currU, currMentions, nearest_neighbors, *dataset);

        if (currRatings.empty()) {std::cerr<<"*"; continue;}

        double currCryptoRating=NAN;
        for (auto R_i: currRatings) if (R_i.second == i) currCryptoRating = R_i.first;
        double currMAE = (originalSentiment - currCryptoRating < 0) ? -(originalSentiment-currCryptoRating) : originalSentiment-currCryptoRating;

        this->MAE += currMAE;
        this->totalCryptosEvaluated++;
    }

    this->totalUsersEvaluated++;
}
