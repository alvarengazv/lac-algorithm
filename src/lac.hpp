#ifndef LAC_HPP
#define LAC_HPP

#include <bits/stdc++.h>
// Pra usar:
// pthread.h
// cmath
// fstream
// iostream
// string
// unordered_map
// unordered_set
// vector
// chrono

#define MIN_SUPORTE 0
#define DECREASE_CARDINALITY 1
#define USE_COSINE_SIMILARITY 1
#define THRESHOLD 0.85
#define MAX_COMBS 3

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
    seed ^= hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct pairHash {
    template <typename T, typename U>
    size_t operator()(const pair<T, U>& rhs) const {
        size_t retval = hash<T>()(rhs.first);
        hashCombine(retval, rhs.second);
        return retval;
    }
};

struct unorderedSetPairHash {
    size_t operator()(const unordered_set<pair<int, int>, pairHashSimilarity>& v) const {
        size_t seed = 0;
        for (const auto& p : v) {
            seed ^= pairHashSimilarity{}(p) + 0x9e3779b9;
        }
        return seed;
    }
};

struct unorderedSetPairEqual {
    bool operator()(const unordered_set<pair<int, int>, pairHashSimilarity>& lhs, const unordered_set<pair<int, int>, pairHashSimilarity>& rhs) const {
        return lhs == rhs;
    }
};

struct vectorPairHash {
    size_t operator()(const vector<pair<int, int>>& v) const {
        size_t seed = 0;
        for (const auto& p : v) {
            seed ^= pairHash{}(p) + 0x9e3779b9;
        }
        return seed;
    }
};

struct vectorPairEqual {
    bool operator()(const vector<pair<int, int>>& lhs, const vector<pair<int, int>>& rhs) const {
        return lhs == rhs;
    }
};

using cacheKey = vector<pair<int, int>>;
using cacheValue = vector<double>;

struct ThreadData {
    vector<unordered_set<pair<int, int>, pairHash>>* combinationsFeatures;
    unordered_map<pair<int, int>, unordered_set<int>, pairHash>* features;
    unordered_map<int, unordered_set<int>>* classes;
    unordered_map<cacheKey, cacheValue, vectorPairHash, vectorPairEqual>* similarityCache;
    bool* shouldStop;
    int start;
    int end;
    double* result;
};

class Lac {
   private:
    unordered_map<pair<int, int>, unordered_set<int>, pairHash> features;
    unordered_map<int, unordered_set<int>> classes;

   public:
    Lac(unordered_map<pair<int, int>, unordered_set<int>, pairHash> features, unordered_map<int, unordered_set<int>> classes);
    void training(string path);
    float testing(string path);
    static unordered_set<int> intersectionAll(vector<unordered_set<int>> list);
    int findMaxIndex(double* arr, int size);
    vector<int> splitString(string line);
    vector<unordered_set<pair<int, int>, pairHash>> combinations(const vector<pair<int, int>>& c, int k);
    void populateCache(cacheKey lineFeatures, cacheValue classesSupport);
    static pair<vector<double>, double> checkSimilarity(cacheKey lineFeatures);
    static double cosineSimilarity(const vector<pair<int, int>>& set1, const vector<pair<int, int>>& set2);
    static void* threadIntersection(void* arg);
    static int INTERSECTION_LIMIT;
    static unordered_map<cacheKey, cacheValue, vectorPairHash, vectorPairEqual> similarityCache;
};

#endif