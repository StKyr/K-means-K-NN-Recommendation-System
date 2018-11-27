#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include <utils.hpp>

namespace ID{
    static size_t cnt=0;
    size_t get() {return cnt++;}
}

std::pair<std::string, std::vector<double>> VectorCSVReader::parseNextLine(){

    std::vector<double> v;
    std::string line;
    std::string part;
    std::string vectorId;
    double x;

    if(this->inputFile.is_open()) {

        try {

            if (getline(this->inputFile, line)) {   // get a whole line

                std::stringstream ss(line);
                getline(ss, part, delimiter);

                vectorId = part;

                try {
                    while (getline(ss, part, delimiter)) {
                        x = stod(part);
                        v.push_back(x);
                    }

                }catch (std::exception &e){ // possible '\r' character before new line
                    if (part != "\r"){
                        throw e;
                    }
                }
            }

        }catch(std::exception& e) {
            throw std::runtime_error("Invalid format of "+filename+". (Do lines have ID and are they TAB-separated?)");
        }
    }

    return std::make_pair(vectorId, v);
}

Dataset* VectorCSVReader::readDataset() {

    this->inputFile.open(this->filename, std::ios::in);

    parseFirstLine();

    Dataset* dataset = new std::unordered_map<std::string, NDVector>();

    std::pair<std::string, std::vector<double>> nextVector;

    nextVector = parseNextLine();


    while(!nextVector.first.empty()){

        NDVector v(nextVector.second, ID::get());

        if (! this->vectorDim) this->vectorDim = (int)v.dim();

        if (v.dim() != this->vectorDim) throw std::runtime_error("\""+filename+"\" dataset contains vector of varying dimensions.");

        (*dataset)[nextVector.first] = v;

        nextVector = parseNextLine();
    }

    this->inputFile.close();
    return dataset;
}

OrderedDataset* VectorCSVReader::readOrderedDataset() {

    this->inputFile.open(this->filename, std::ios::in);

    parseFirstLine();

    OrderedDataset* dataset = new std::map<std::string, NDVector>();

    std::pair<std::string, std::vector<double>> nextVector;

    nextVector = parseNextLine();


    while(!nextVector.first.empty()){

        NDVector v(nextVector.second,-1);

        if (! this->vectorDim) this->vectorDim = (int)v.dim();

        if (v.dim() != this->vectorDim) throw std::runtime_error("\""+filename+"\" dataset contains vector of varying dimensions.");

        (*dataset)[nextVector.first] = v;

        nextVector = parseNextLine();
    }

    this->inputFile.close();
    return dataset;
}


void DatasetReader::parseFirstLine() {
    std::string line;

    if (this->inputFile.is_open()) {

        if (!getline(this->inputFile, line)) {                                      // get first line of file
            throw std::runtime_error("File \""+this->filename+"\" is empty");
        }


        while(line.empty() || std::all_of(line.begin(), line.end(), isspace)) {     // throw away empty lines
            getline(this->inputFile, line);
        }

        int pos = (int)ltrim(line).find("euclidean");                               // search for "euclidean"

        if (pos == 0){                                                              // "euclidean" found at the beginning of line
            this->metric = supported_metrics::Euclidean;
            return;

        }else if (pos == std::string::npos){
            pos = (int)ltrim(line).find("cosine");                                  // search for "cosine"

            if (pos == 0) {                                                         // which was found at the beginning of file
                this->metric = supported_metrics::Cosine;
            }else if (pos == std::string::npos){                                    // metric is missing
                this->metric = supported_metrics::Euclidean;                        // assume Euclidean
            }else{                                                                  // "cosine" was found somewhere in the line

                throw std::runtime_error("Invalid first line of file \""+this->filename+"\"");
            }
        }else{                                                                      // "euclidean was found somewhere in the line

            throw std::runtime_error("Invalid first line of file \""+this->filename+"\"");
        }


    }else{
        throw std::runtime_error("Cannot open \""+this->filename+"\" file.");
    }
}