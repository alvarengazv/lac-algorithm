#ifndef LAC_HPP
#define LAC_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include "tokenizer.hpp"

using namespace std;

struct pairHash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

class Lac{
    private:
        unordered_map<pair<int, int>, vector<int>, pairHash> features;
        unordered_map<int, vector<int>> classes;

    public:
        Lac(unordered_map<pair<int, int>, vector<int>, pairHash> features, unordered_map<int, vector<int>> classes);
        void readFile(string path);
        void training(string path);
        void testing();
        vector<int> splitString(string line);
        unordered_map<pair<int, int>, vector<int>, pairHash> getFeatures();
        unordered_map<int, vector<int>> getClasses();
        


};

#endif