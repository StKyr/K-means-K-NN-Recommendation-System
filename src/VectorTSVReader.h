#ifndef ERGASIA1_VECTORTSVREADER_H
#define ERGASIA1_VECTORTSVREADER_H


#include <string>
#include <NDVector.h>
#include <fstream>

class VectorTSVReader {

                                VectorTSVReader (std::string flename)   :filename(flename) {}
            void                initialize      ()                      {this->inputFile.open(this->filename, std::ios::in);}
    virtual void                parseFirstLine  ()                      = 0;
            std::vector<double> parseNextLine   (std::string *vectorId);
            std::vector<double> parseNextLine   (int *vectorId);
            void                finalize        ()                      {this->inputFile.close();}
                                ~VectorTSVReader()                      {this->inputFile.close();}

protected:
    std::string filename;
    std::ifstream inputFile;

};


#endif //ERGASIA1_VECTORTSVREADER_H
