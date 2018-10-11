#ifndef ERGASIA1_DATASETREADER_H
#define ERGASIA1_DATASETREADER_H


#include <metrics.h>
#include "VectorTSVReader.h"

class DatasetReader : public VectorTSVReader{
public:

    explicit DatasetReader(std::string filename) :VectorTSVReader(filename), metric(_NULL_) {}
    void parseFirstLine();

    // member
    supported_metrics metric;

};


#endif //ERGASIA1_DATASETREADER_H
