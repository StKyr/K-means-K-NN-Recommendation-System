#include <iostream>
#include <NDVector.h>

int main() {
    std::cout << "Hello, World!" << std::endl;


    NDVector v1 = NDVector({1, 1});


    std::cout << v1.norm() << std::endl;


    return 0;

}