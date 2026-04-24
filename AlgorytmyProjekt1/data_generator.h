#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>

// Types of data distributions to generate
enum class DataType {
    RANDOM,
    SORTED_25,
    SORTED_50,
    SORTED_75,
    SORTED_95,
    SORTED_99,
    SORTED_997,
    REVERSE_SORTED
};
// static random number generator
static std::mt19937 rng(std::random_device{}());

// Generates a vector of 'n' integers according to the specified DataType
std::vector<int> generateData(int n, DataType type) {
    std::vector<int> arr(n);
    std::uniform_int_distribution<int> dist(0, 10 * n);

    switch (type) {
    case DataType::RANDOM: {
        for (auto& x : arr) x = dist(rng);
        break;
    }
    case DataType::REVERSE_SORTED: {
        std::iota(arr.begin(), arr.end(), 1);
        std::reverse(arr.begin(), arr.end());
        break;
    }
    default: {
        // percentage of sorted prefix
        double pct = 0.0;
        if (type == DataType::SORTED_25)  pct = 0.25;
        if (type == DataType::SORTED_50)  pct = 0.50;
        if (type == DataType::SORTED_75)  pct = 0.75;
        if (type == DataType::SORTED_95)  pct = 0.95;
        if (type == DataType::SORTED_99)  pct = 0.99;
        if (type == DataType::SORTED_997) pct = 0.997;

        int sortedCount = (int)(n * pct);
        for (auto& x : arr) x = dist(rng);
        std::sort(arr.begin(), arr.begin() + sortedCount);
        break;
    }
    }
    return arr;
}

// helper to convert DataType to string for output
std::string dataTypeName(DataType dt) {
    switch (dt) {
    case DataType::RANDOM:        return "random";
    case DataType::SORTED_25:     return "sorted_25";
    case DataType::SORTED_50:     return "sorted_50";
    case DataType::SORTED_75:     return "sorted_75";
    case DataType::SORTED_95:     return "sorted_95";
    case DataType::SORTED_99:     return "sorted_99";
    case DataType::SORTED_997:    return "sorted_997";
    case DataType::REVERSE_SORTED:return "reverse";
    }
    return "";
}
