#ifndef LAC_HPP
#define LAC_HPP

#include <bits/stdc++.h>

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define MIN_SUPORTE 0
#define INTERSECTION_LIMIT 10

using namespace std;

template <typename T>
void hashCombine(size_t& seed, T const& v) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct pairHash {
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U>& rhs) const {
        size_t retval = std::hash<T>()(rhs.first);
        hashCombine(retval, rhs.second);
        return retval;
    }
};

struct unordered_set_pair_hash {
    size_t operator()(const std::unordered_set<std::pair<int, int>, pairHash>& v) const {
        size_t seed = 0;
        for (const auto& p : v) {
            seed ^= pairHash{}(p) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct unordered_set_pair_equal {
    bool operator()(const std::unordered_set<std::pair<int, int>, pairHash>& lhs, const std::unordered_set<std::pair<int, int>, pairHash>& rhs) const {
        return lhs == rhs;
    }
};

using CacheKey = std::unordered_set<std::pair<int, int>, pairHash>;
using CacheValue = double[10];

class Lac {
   private:
    unordered_map<pair<int, int>, unordered_set<int>, pairHash> features;
    unordered_map<int, unordered_set<int>> classes;
    std::unordered_map<CacheKey, CacheValue, unordered_set_pair_hash, unordered_set_pair_equal> cacheResults;

   public:
    Lac(unordered_map<pair<int, int>, unordered_set<int>, pairHash> features, unordered_map<int, unordered_set<int>> classes);
    void readFile(string path);
    void training(string path);
    float testing(string path);
    int intersection(unordered_set<int> first, unordered_set<int> second);
    unordered_set<int> intersectionAll(vector<unordered_set<int>> list);
    int findMaxIndex(double* arr, int size);
    // vector<float> splitString(string line);
    vector<int> splitString(string line);
    unordered_map<pair<int, int>, unordered_set<int>, pairHash> getFeatures();
    // unordered_map<pair<int, float>, vector<int>, pairHash> getFeatures();
    unordered_map<int, unordered_set<int>> getClasses();
    void imprimirFeatures();
    void imprimirClasses();
    // vector<vector<pair<int, float>>> combo(const vector<pair<int, float>> &c, int k);
    vector<unordered_set<pair<int, int>, pairHash>> combo(const vector<pair<int, int>>& c, int k);
    // void pretty_print(const vector<pair<int, float>> &c, int combo);
    void pretty_print(const vector<pair<int, int>>& c, int combo);
};

#endif