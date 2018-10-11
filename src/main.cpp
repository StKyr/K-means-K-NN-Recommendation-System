#include <iostream>
#include <NDVector.h>
#include <DatasetReader.h>
#include <QuerysetReader.h>

int main() {
    std::cout << "Hello, World!" << std::endl;

    QuerysetReader reader ("../input/queryset1.tsv");
    //DatasetReader reader ("../input/input1.tsv");

    reader.initialize();
    reader.parseFirstLine();

    std::cout << reader.R <<std::endl;
    //std::cout << reader.metric << std::endl;


    std::string s;
    std::vector<double> xs;
    int i;
    xs = reader.parseNextLine(&s);
    while (! xs.empty()) {
        NDVector v(xs);
        std::cout << v << std::endl;
        xs = reader.parseNextLine(&s);
    }
    return 0;

}