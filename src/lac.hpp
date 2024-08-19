#ifndef LAC_HPP
#define LAC_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

struct pairHash
{
    template <class T1, class T2>
     size_t operator()(const pair<T1, T2> &pair) const
    {
        return hash<T1>()(pair.first) ^ hash<T2>()(pair.second);
    }
};

struct vector_pair_hash
{
    template <class T>
    size_t operator()(const vector<pair<T, T>> &v) const
    {
        size_t seed = 0;
        for (const auto &p : v)
        {
            seed ^= pairHash{}(p) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
 
struct vector_pair_equal
{
    template <class T>
    bool operator()(const vector< pair<T, T>> &lhs, const  vector<pair<T, T>> &rhs) const
    {
        return lhs == rhs;
    }
};

class Lac
{
private:
    // unordered_map<pair<int, float>, vector<int>, pairHash> features;
    unordered_map<pair<int, int>, vector<int>, pairHash> features;
    unordered_map<int, vector<int>> classes;
    unordered_map<vector<std::pair<int, int>>,vector<int>, vector_pair_hash, vector_pair_equal> cache;

public:
    // Lac(unordered_map<pair<int, float>, vector<int>, pairHash> features, unordered_map<int, vector<int>> classes);
    Lac(unordered_map<pair<int, int>, vector<int>, pairHash> features, unordered_map<int, vector<int>> classes);
    void readFile(string path);
    void training(string path);
    void testing(string path);
    int intersection(vector<int> first, vector<int> second);
    vector<int> intersectionAll(vector<vector<int>> list);
    int findMaxIndex(double *arr, int size);
    // vector<float> splitString(string line);
    vector<int> splitString(string line);
    unordered_map<pair<int, int>, vector<int>, pairHash> getFeatures();
    // unordered_map<pair<int, float>, vector<int>, pairHash> getFeatures();
    unordered_map<int, vector<int>> getClasses();
    void imprimirFeatures();
    void imprimirClasses();
};

#endif