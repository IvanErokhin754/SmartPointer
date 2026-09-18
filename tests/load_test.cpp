#include <iostream>
#include <chrono>
#include <vector>
#include "SmrtPtr.hpp"
#include <memory>

void TestCreate(size_t n) {
    std::cout << "TEST CREATE (num of elemets - " << n << ")\n";

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < n; i++) {
        SmrtPtr<int> ptr(new int(10));
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SmrtPtr: " << time << " us\n";


    volatile long long sum = 0;

    start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < n; i++) {
        int *ptr = new int(static_cast<int>(i));

        sum += *ptr;

        delete ptr;
    }

    end = std::chrono::high_resolution_clock::now();

    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "RawPtr: " << time << " us\n";


    start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < n; i++) {
        std::shared_ptr<int> ptr(new int(10));        
    }

    end = std::chrono::high_resolution_clock::now();

    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SharedPtr: " << time << " us\n";

}


void TestCopyConstructor(size_t n) {

    std::cout << "TEST COPY CONSTRUCTOR (num of elemets - " << n << ")\n";

    auto start = std::chrono::high_resolution_clock::now();

    SmrtPtr<int> a(new int(10));
    for (size_t i = 0; i < n; i++) {
        SmrtPtr<int> ptr(a);
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SmrtPtr: " << time << " us\n";


    start = std::chrono::high_resolution_clock::now();
    
    std::shared_ptr<int> sh_ptr(new int(10));
    for (size_t i = 0; i < n; i++) {
        std::shared_ptr<int> ptr(sh_ptr);
    }

    end = std::chrono::high_resolution_clock::now();


    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SharedPtr: " << time << " us\n";
}


void TestCopyAssignment(size_t n) {

    std::cout << "TEST COPY ASSIGNMENT (num of elemets - " << n << ")\n";

    auto start = std::chrono::high_resolution_clock::now();

    SmrtPtr<int> source(new int(10));
    SmrtPtr<int> target(new int(20));

    for (size_t i = 0; i < n; i++) {
        target = source;
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SmrtPtr: " << time << " us\n";


    start = std::chrono::high_resolution_clock::now();

    std::shared_ptr<int> source_sh(new int(10));
    std::shared_ptr<int> target_sh(new int(20));

    for (size_t i = 0; i < n; i++) {
        target_sh = source_sh;
    }

    end = std::chrono::high_resolution_clock::now();

    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SharedPtr: " << time << " us\n";

}

void TestMoveAssignment(size_t n) {
    std::cout << "TEST MOVE ASSIGNMENT (num of elemets - " << n << ")\n";

    std::vector<SmrtPtr<int>> sources;
    std::vector<SmrtPtr<int>> targets;

    sources.reserve(n);
    targets.reserve(n);

    for (size_t i = 0; i < n; i++) {
        sources.emplace_back(new int(10));
        targets.emplace_back(new int(20));
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < n; i++) {
        targets[i] = std::move(sources[i]);
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SmrtPtr: " << time << " us\n";

    std::vector<std::shared_ptr<int>> sources_sh;
    std::vector<std::shared_ptr<int>> targets_sh;

    sources_sh.reserve(n);
    targets_sh.reserve(n);

    for (size_t i = 0; i < n; i++) {
        sources_sh.emplace_back(new int(10));
        targets_sh.emplace_back(new int(20));
    }

    start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < n; i++) {
        targets_sh[i] = std::move(sources_sh[i]);
    }

    end = std::chrono::high_resolution_clock::now();

    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SharedPtr: " << time << " us\n";

}

void TestMoveConstructor(size_t n) {

    std::cout << "TEST MOVE CONSTRUCTOR (num of elemets - " << n << ")\n";

    std::vector<SmrtPtr<int>> sources;
    sources.reserve(n);

    for (size_t i = 0; i < n; i++) {
        sources.emplace_back(new int(10));
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < n; i++) {
        SmrtPtr<int> tmp(std::move(sources[i]));
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SmrtPtr: " << time << " us\n";

    std::vector<std::shared_ptr<int>> sources_sh;
    sources_sh.reserve(n);

    for (size_t i = 0; i < n; i++) {
        sources_sh.emplace_back(new int(10));
    }

    start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < n; i++) {
        std::shared_ptr<int> tmp(std::move(sources_sh[i]));
    }

    end = std::chrono::high_resolution_clock::now();


    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SharedPtr: " << time << " us\n";
}

void TestRawMemory(size_t n) {

    std::vector<int*> ptrs;
    ptrs.reserve(n);

    for (size_t i = 0; i < n; i++) {
        ptrs.push_back(new int(5));
    }


    for (auto ptr : ptrs) {
        delete ptr;
    }
}

void TestSmrtPtrMemory(size_t n) {
    std::vector<SmrtPtr<int>> ptrs;
    ptrs.reserve(n);

    for (size_t i = 0; i < n; i++) {
        ptrs.emplace_back(new int(5));
    }
}

void TestSharedPtrMemory(size_t n) {
    std::vector<std::shared_ptr<int>> ptrs;
    ptrs.reserve(n);

    for (size_t i = 0; i < n; i++) {
        ptrs.emplace_back(new int(5));
    }
}

int main() 
{
    //  TestCreate(1000);
    //  TestCreate(10'000);
    //  TestCreate(100'000);
    //  TestCreate(1'000'000);
    
    // TestCopyConstructor(1000);
    // TestCopyConstructor(10'000);
    // TestCopyConstructor(100'000);
    // TestCopyConstructor(1'000'000);

    TestCopyAssignment(1000);
    TestCopyAssignment(10'000);
    TestCopyAssignment(100'000);
    TestCopyAssignment(1'000'000);

    // TestMoveConstructor(1000);
    // TestMoveConstructor(10'000);
    // TestMoveConstructor(100'000);
    // TestMoveConstructor(1'000'000);

    // TestMoveAssignment(1000);
    // TestMoveAssignment(10'000);
    // TestMoveAssignment(100'000);
    // TestMoveAssignment(1'000'000);


    
    //TestRawMemory(1000);
    //TestSmrtPtrMemory(1000);
    //TestSharedPtrMemory(1000);

    //TestRawMemory(10'000);
    //TestSmrtPtrMemory(10'000);
    //TestSharedPtrMemory(10'000);

    //TestRawMemory(100'000);
    //TestSmrtPtrMemory(100'000);
    //TestSharedPtrMemory(100'000);

    // TestRawMemory(1'000'000);
    // TestSmrtPtrMemory(1'000'000);
    // TestSharedPtrMemory(1'000'000);





}



