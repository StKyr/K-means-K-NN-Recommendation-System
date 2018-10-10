#include "DatasetReader.h"

void DatasetReader::parseFirstLine(){

    std::string line;
    std::string metricName;
    std::size_t pos1;
    std::size_t comment_pos;

    if(this->inputFile.is_open()) {
        if (getline(this->inputFile, line)) {

            pos1 = line.find("@metric ");
            if (pos1 == 0){
                comment_pos = line.find("//");
                if (comment_pos == std::string::npos){ // no comment
                    metricName = line.substr(8);
                }else{
                    metricName = line.substr(8, comment_pos-8);
                }
                metricName.erase(metricName.find_last_not_of(" \n\r\t")+1); // trim spaces

                if (metricName == "euclidean"){
                    this->metric = Euclidean;
                }else if (metricName == "cosine"){
                    this->metric = Cosine;
                }else{
                    throw std::logic_error("Invalid metric name");
                }

            }else{
                throw std::logic_error("Invalid Input format");
            }
        }
    }
}