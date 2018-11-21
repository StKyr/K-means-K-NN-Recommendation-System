#ifndef ERGASIA1_VECTORTSVREADER_H
#define ERGASIA1_VECTORTSVREADER_H


#include <string>
#include <fstream>
#include <unordered_map>
#include <map>
#include "metrics.h"


typedef std::unordered_map<std::string, NDVector> Dataset;
typedef std::map<std::string, NDVector> OrderedDataset;

class VectorCSVReader {
public:

    explicit        VectorCSVReader   (std::string& flename, char delimiter=',') :filename(flename), vectorDim(0), delimiter(delimiter) {}
    Dataset*        readDataset       ();
    OrderedDataset* readOrderedDataset();
                    ~VectorCSVReader  ()                    {if (this->inputFile.is_open()) this->inputFile.close();}

    int vectorDim;

protected:
    std::string   filename;
    std::ifstream inputFile;
    char delimiter;

    std::pair<std::string, std::vector<double>> parseNextLine();
    virtual void                                parseFirstLine() = 0;


};


class DatasetReader : public VectorCSVReader{
public:

    explicit         DatasetReader(std::string filename) :VectorCSVReader(filename), metric(_NULL) {}



    // member
    supported_metrics metric;

private:
    void parseFirstLine() override;
};














#endif //ERGASIA1_VECTORTSVREADER_H
