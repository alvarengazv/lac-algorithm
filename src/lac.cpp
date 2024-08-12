#include "lac.hpp"

Lac::Lac(unordered_map<pair<int, int>, vector<int>, pairHash> features, unordered_map<int, vector<int>> classes){
    this->features = features;
    this->classes = classes;
}

void Lac::readFile(string path){
    
}

void Lac::training(string path){
    ifstream file(path);
    if(!file.is_open()){
        return ;    
    }

    string line;
    int j = 1;

    while (getline(file, line)){
        vector<int> values = splitString(line);
        int i = 0;
        for(auto v: values){
            pair<int, int> feature(++i, v);
            
            if(features.find(feature) != features.end()){
                features[feature].push_back(j);
            } else {
                vector<int> lines;
                lines.push_back(j);
                features.insert(pair<pair<int, int>, vector<int>>(feature, lines));
            }
        }

        if(classes.find(values.back()) != classes.end()){
            classes[values.back()].push_back(j);
        } else{
            vector<int> lines;
            lines.push_back(j);
            classes.insert(pair<int, vector<int>>(values.back(), lines));
        }

        j++;
    }
    
    file.close();
}

vector<int> Lac::splitString(string line){
    vector<int> result;
    string delimiter = ",";
    size_t pos = 0;
    string token;
    while ((pos = line.find(delimiter)) != string::npos) {
        token = line.substr(0, pos);
        result.push_back(stoi(token));
        line.erase(0, pos + delimiter.length());
    }
    result.push_back(stoi(line));
    return result;
}

//gets

unordered_map<pair<int, int>, vector<int>, pairHash> Lac::getFeatures(){
    return features;
}

unordered_map<int, vector<int>> Lac::getClasses(){
    return classes;
}