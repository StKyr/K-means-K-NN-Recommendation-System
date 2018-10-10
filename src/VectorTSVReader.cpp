#include "VectorTSVReader.h"

std::vector<double> VectorTSVReader::parseNextLine(std::string *vectorId){

    std::vector<double> v;
    std::string line;
    std::string part;
    double x;

    if(this->inputFile.is_open()) {

        if (getline(this->inputFile, line)) {   // get a whole line

            std::stringstream ss(line);
            getline(ss, (*vectorId), '\t');

            while (getline(ss, part, '\t')) {
                std::stringstream xstream(part);
                xstream >> x;
                v.push_back(x);
            }
        }
    }

    return v;
}


std::vector<double> VectorTSVReader::parseNextLine(int *vectorId){

    std::vector<double> v;
    std::string line;
    std::string stringId;
    std::string part;
    double x;

    if(this->inputFile.is_open()) {

        if (getline(this->inputFile, line)) {   // get a whole line

            std::stringstream ss(line);

            getline(ss, stringId, '\t');
            std::stringstream stream(stringId);
            stream >> (*vectorId);

            while (getline(ss, part, '\t')) {
                std::stringstream xstream(part);
                xstream >> x;
                v.push_back(x);
            }
        }
    }

    return v;
}
