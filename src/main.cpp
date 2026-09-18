#include <iostream>
#include "SmrtPtr.hpp"
#include <memory>

int main() 
{
    std::cout << "int* = " << sizeof(int*) << std::endl;
    
    //
    
    std::cout << "SmrtPtr = " << sizeof(SmrtPtr<int>) << std::endl;
    std::cout << "Storage = " << sizeof(Storage) << std::endl;

    //

    std::cout << "shared_ptr = " << sizeof(std::shared_ptr<int>) << std::endl;

    return 0;
}