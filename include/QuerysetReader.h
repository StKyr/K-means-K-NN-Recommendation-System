#ifndef ERGASIA1_QUERYSETREADER_H
#define ERGASIA1_QUERYSETREADER_H

#include <string>
#include "VectorTSVReader.h"


//TODO: move class definition to single .h file

class QuerysetReader :public VectorTSVReader {

public:
    explicit QuerysetReader(std::string filename) :VectorTSVReader(filename), R(-1) {}
    void parseFirstLine();


    double R;
};


#endif //ERGASIA1_QUERYSETREADER_H
