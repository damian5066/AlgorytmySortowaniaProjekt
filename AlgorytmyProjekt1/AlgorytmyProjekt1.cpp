#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <iomanip>
#include <functional>
#include <numeric>
#include "sorting.h"
#include "data_generator.h"


using Clock = std::chrono::high_resolution_clock;

// Returns average time in microseconds over 'runs' repetitions
double benchmark(std::function<void(std::vector<int>&)> sortFn,
    int n, DataType dt, int runs = 100) {
    double total = 0.0;
    for (int r = 0; r < runs; ++r) {
        auto arr = generateData(n, dt);
        auto t0 = Clock::now();
        sortFn(arr);
        auto t1 = Clock::now();
        total += std::chrono::duration<double, std::micro>(t1 - t0).count();
    }
    return total / runs;
}

int main() {
    const std::vector<int> sizes = { 100, 500, 1000, 5000, 10000,
                                    50000, 100000, 500000, 1000000 };

    const std::vector<DataType> dataTypes = {
        DataType::RANDOM,
        DataType::SORTED_25,
        DataType::SORTED_50,
        DataType::SORTED_75,
        DataType::SORTED_95,
        DataType::SORTED_99,
        DataType::SORTED_997,
        DataType::REVERSE_SORTED
    };

    struct Algorithm {
        std::string name;
        std::function<void(std::vector<int>&)> fn;
    };

    std::vector<Algorithm> algorithms = {
        {"MergeSort",  [](std::vector<int>& a) { mergeSort(a); }},
        {"QuickSort",  [](std::vector<int>& a) { quickSort(a); }},
        {"IntroSort",  [](std::vector<int>& a) { introSort(a); }},
    };

    // CSV output
    std::ofstream csv("results.csv");
    csv << "algorithm,data_type,size,avg_time_us\n";

    // Also pretty-print to stdout
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::left
        << std::setw(12) << "Algorithm"
        << std::setw(14) << "DataType"
        << std::setw(10) << "Size"
        << "AvgTime(us)\n";
    std::cout << std::string(50, '-') << "\n";

    for (auto& alg : algorithms) {
        for (auto dt : dataTypes) {
            std::string dtName = dataTypeName(dt);
            for (int n : sizes) {
                // For large n with slow data types, reduce runs
                int runs = 100;
                if (n >= 100000)  runs = 10;
                if (n >= 500000)  runs = 5;
                if (n >= 1000000)  runs = 5;

                double avgUs = benchmark(alg.fn, n, dt, runs);

                csv << alg.name << "," << dtName << "," << n << ","
                    << std::fixed << std::setprecision(2) << avgUs << "\n";

                std::cout << std::left
                    << std::setw(12) << alg.name
                    << std::setw(14) << dtName
                    << std::setw(10) << n
                    << avgUs << " us\n";
                std::cout.flush();
            }
        }
    }

    csv.close();
    std::cout << "\nWyniki zapisano do results.csv\n";
    return 0;
}
