#include "lac.hpp"

int main(){
    unordered_map<pair<int, int>, vector<int>, pairHash> features;
    unordered_map<int, vector<int>> classes;

    Lac lac = Lac(features, classes);

    //treino
    // lac.training("datasets/iris.data");
    lac.training("datasets/poker-hand-training.data");

    //teste
    auto start = chrono::high_resolution_clock::now();

    lac.testing("datasets/poker-hand-testing.data");
    // lac.testing("datasets/iris-training.data");

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Tempo de execução: " << duration.count() / 60000 << " minutos, " << (duration.count() % 60000) / 1000 << " segundos e " << (duration.count() % 60000) % 1000 << " milissegundos" << endl;

    return 0;
}


/*auto map = lac.getFeatures();
auto map2 = lac.getClasses();
//print all features in map
cout << map.size() << endl;

for(auto it = map.begin(); it != map.end(); it++){
    cout << "(" << it->first.first << ", " << it->first.second <<  ")" << " -> ";
    for(auto i: it->second){
        cout << i << " ";
    }
    cout << endl;
}

//print all classes in map2
cout << map2.size() << endl;

for(auto it = map2.begin(); it != map2.end(); it++){
    cout << it->first << " -> ";
    for(auto i: it->second){
        cout << i << " ";
    }
    cout << endl;
}*/