#ifndef LAC_HPP
#define LAC_HPP

#include <bits/stdc++.h>
#include <pthread.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define MIN_SUPORTE 0
#define INTERSECTION_LIMIT 0
#define THRESHOLD 0.95

using namespace std;

struct pairHashSimilarity {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

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

struct unorderedSetPairHash {
    size_t operator()(const std::unordered_set<std::pair<int, int>, pairHashSimilarity>& v) const {
        size_t seed = 0;
        for (const auto& p : v) {
            seed ^= pairHashSimilarity{}(p) + 0x9e3779b9;
        }
        return seed;
    }
};

struct unorderedSetPairEqual {
    bool operator()(const std::unordered_set<std::pair<int, int>, pairHashSimilarity>& lhs, const std::unordered_set<std::pair<int, int>, pairHashSimilarity>& rhs) const {
        return lhs == rhs;
    }
};

struct vectorPairHash {
    size_t operator()(const std::vector<std::pair<int, int>>& v) const {
        size_t seed = 0;
        for (const auto& p : v) {
            seed ^= pairHash{}(p) + 0x9e3779b9;
        }
        return seed;
    }
};

struct vectorPairEqual {
    bool operator()(const std::vector<std::pair<int, int>>& lhs, const std::vector<std::pair<int, int>>& rhs) const {
        return lhs == rhs;
    }
};

using cacheKey = std::unordered_set<std::pair<int, int>, pairHashSimilarity>;
using cacheValue = double[10];

struct ThreadData {
    vector<unordered_set<pair<int, int>, pairHash>>* combinacoesFeatures;
    unordered_map<pair<int, int>, unordered_set<int>, pairHash>* features;
    unordered_map<int, unordered_set<int>>* classes;
    int start;
    int end;
    double* result;
    unordered_map<cacheKey, cacheValue, unorderedSetPairHash, unorderedSetPairEqual>* cacheResults;
};

class Lac {
   private:
    unordered_map<pair<int, int>, unordered_set<int>, pairHash> features;
    unordered_map<int, unordered_set<int>> classes;
    std::unordered_map<cacheKey, cacheValue, unorderedSetPairHash, unorderedSetPairEqual> cacheResults;
    std::unordered_map<vector<pair<int, int>>, int, vectorPairHash, vectorPairEqual> lshBuckets;

   public:
    Lac(unordered_map<pair<int, int>, unordered_set<int>, pairHash> features, unordered_map<int, unordered_set<int>> classes);
    void training(string path);
    float testing(string path);
    int intersection(unordered_set<pair<int, int>, pairHash> first, unordered_set<pair<int, int>, pairHash> second);
    static unordered_set<int> intersectionAll(vector<unordered_set<int>> list);
    int findMaxIndex(double* arr, int size);
    vector<int> splitString(string line);
    unordered_map<pair<int, int>, unordered_set<int>, pairHash> getFeatures();
    unordered_map<int, unordered_set<int>> getClasses();
    void imprimirFeatures();
    void imprimirClasses();
    vector<unordered_set<pair<int, int>, pairHash>> combo(const vector<pair<int, int>>& c, int k);
    void generateLSH(vector<pair<int, int>> lineFeatures, int classBucket);
    pair<int, double> checkSimilarity(vector<pair<int, int>> lineFeatures);
    double cosineSimilarity(const vector<pair<int, int>>& set1, const vector<pair<int, int>>& set2);
    double jaccardSimilarity(const vector<pair<int, int>>& vec1, const vector<pair<int, int>>& vec2);
    int intersectionSize(const unordered_set<int>& set1, const unordered_set<int>& set2);
    static void* threadIntersecao(void* arg);
};

#endif