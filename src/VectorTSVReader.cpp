#include "VectorTSVReader.h"


static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}


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

void QuerysetReader::parseFirstLine(){

    std::string part;

    if (this->inputFile.is_open()) {

        if (!getline(this->inputFile, part)) {                                      // get first line of file
            throw std::runtime_error("File \""+this->filename+"\" is empty");
        }

        try {

            getline(inputFile, part, ' ');
            if (ltrim(part) != "Radius:") throw std::runtime_error("Invalid first line of query set");

            getline(inputFile, part);
            this->R = stod(ltrim(part));


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