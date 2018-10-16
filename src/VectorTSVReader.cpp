#include "VectorTSVReader.h"

std::pair<std::string, std::vector<double>> VectorTSVReader::parseNextLine(){

    std::vector<double> v;
    std::string line;
    std::string part;
    std::string vectorId;
    double x;

    if(this->inputFile.is_open()) {

        try {

            if (getline(this->inputFile, line)) {   // get a whole line

                std::stringstream ss(line);
                getline(ss, part, '\t');

                vectorId = part;

                while (getline(ss, part, '\t')) {
                    std::stringstream xstream(part);
                    xstream >> x;
                    v.push_back(x);
                }
            }

        }catch(std::exception& e) {
            throw std::runtime_error("Invalid format");
        }
    }

    return std::make_pair(vectorId, v);
}

Dataset VectorTSVReader::readDataset() {

    this->inputFile.open(this->filename, std::ios::in);

    parseFirstLine();

    Dataset dataset;
    std::pair<std::string, std::vector<double>> nextVector;

    nextVector = parseNextLine();


    while(!nextVector.first.empty()){

        NDVector v(nextVector.second);

        if (! this->vectorDim) this->vectorDim = (int)v.dim();

        if (v.dim() != this->vectorDim) throw std::runtime_error("\""+filename+"\" dataset contains vector of varying dimensions.");

        dataset[nextVector.first] = v;

        nextVector = parseNextLine();
    }

    this->inputFile.close();
    return dataset;
}

void DatasetReader::parseFirstLine() {
    std::string line;

    if (this->inputFile.is_open()) {

        getline(this->inputFile, line);                                             // get first line of file
        while(line.empty() || std::all_of(line.begin(), line.end(), isspace)) {     // throw away empty lines
            getline(this->inputFile, line);
        }

        if (getline(this->inputFile, line)){                                        // get actual first line

            int pos = (int)line.find("euclidean");                                  // search for "euclidean"

            if (pos != std::string::npos){                                          // metric was "euclidean"
                this->metric = supported_metrics::Euclidean;
                return;

            }else{
                pos = (int)line.find("cosine");                                     // search for "cosine"

                if (pos != std::string::npos) {                                     // which was found
                    this->metric = supported_metrics::Cosine;
                }else{                                                              // metric is missing
                    this->metric = supported_metrics::Euclidean;                    // assume Euclidean
                }
            }

        }else{
            throw std::runtime_error("File \""+this->filename+"\" is empty");
        }

    }else{
        throw std::runtime_error("Cannot open \""+this->filename+"\" file.");
    }
}

void QuerysetReader::parseFirstLine(){

    std::string part;

    if (this->inputFile.is_open()) {

        try {

            getline(inputFile, part, ' ');
            if (part != "Radius:") throw std::runtime_error("Invalid first line of query set");

            getline(inputFile, part);
            this->R = stod(part);


            if (this->R < 0) {
                throw std::runtime_error("Invalid negative value of R");
            }

        } catch (std::exception &e) {

            throw std::runtime_error("Invalid first line of query set");
        }

    }else{
        throw std::runtime_error("Cannot open \""+this->filename+"\" file.");
    }
}