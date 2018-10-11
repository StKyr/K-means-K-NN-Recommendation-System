#include "QuerysetReader.h"


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
        throw std::runtime_error("File is not open");
    }
}